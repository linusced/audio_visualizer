#include "../../header/audio_visualizer/audio_visualizer.hpp"

const double audio_visualizer::App::COLOR_TRANSITION_DURATION = 4.0,
             audio_visualizer::App::IMAGE_TRANSITION_DURATION = 1.0;

audio_visualizer::App::App(bool *stop, std::string cssCode, std::vector<std::string> bgImageFilePaths, opengl_gui::Window *window)
    : opengl_gui::DrawLoop(window)
{
    this->stop = stop;
    input = new AudioInput();

    prevAudioPeak.resize(60, 0.0);

    std::cout << "Loading image data\n";

    std::vector<opengl_gui::TextureData *> imageData;

    for (auto &str : bgImageFilePaths)
        imageData.push_back(new opengl_gui::TextureData(str));

    std::cout << "Creating image textures\n";

    for (auto &data : imageData)
    {
        textures.push_back(new opengl_gui::Texture(data->bytes, data->width, data->height, data->format));
        delete data;
    }

    std::cout << "Image textures created\n";

    waveformTextureWidth = input->BUFFER_SIZE;
    waveformTextureHeight = 2160 * 0.3f;
    waveformTextureBytes.resize((waveformTextureWidth * waveformTextureHeight) * 4);

    std::cout << "waveformTextureWidth: " << waveformTextureWidth << " waveformTextureHeight: " << waveformTextureHeight << '\n';

    textures.push_back(new opengl_gui::Texture(waveformTextureBytes.data(), waveformTextureWidth, waveformTextureHeight, GL_RGBA));

    guiRenderer = new opengl_gui::Renderer(cssCode, window);

    imageElements.push_back(new opengl_gui::ImageElement(textures[0], "fullscreen"));
    guiRenderer->addElement(imageElements[0]);

    elements.push_back(new opengl_gui::Element("fullscreen"));
    guiRenderer->addElement(elements[0]);

    bgOverlayColor = &elements[0]->elementStyle.colorProperties["background-color"];
    bgOverlayColor->isSet = true;

    imageElements.push_back(new opengl_gui::ImageElement(textures.back(), "waveform"));
    guiRenderer->addElement(imageElements[1]);

    frequencyComplex = (fftw_complex *)calloc(input->BUFFER_SIZE / 2 + 1, sizeof(fftw_complex));
    frequencyDomain.resize(input->BUFFER_SIZE / 4);
}

void audio_visualizer::App::terminate()
{
    fftw_free(frequencyComplex);

    delete input;

    if (guiRenderer)
        delete guiRenderer;

    for (auto &i : fonts)
        delete i;
    for (auto &i : textures)
        delete i;

    for (auto &i : elements)
        delete i;
    for (auto &i : buttonElements)
        delete i;
    for (auto &i : imageElements)
        delete i;
    for (auto &i : sliderElements)
        delete i;
    for (auto &i : textElements)
        delete i;
    for (auto &i : textInputElements)
        delete i;
}

void audio_visualizer::App::loop()
{
    logData();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    input->start();

    while (!*stop && !window->shouldClose())
    {
        window->update();

        if (window->hasResized())
            guiRenderer->resize();

        glm::vec3 currentHSV = HSV;
        double colorTransitionCurrentTime = window->getCurrentTime() - colorTransitionStartTime;
        if (colorTransitionCurrentTime < COLOR_TRANSITION_DURATION)
            currentHSV = glm::mix(prevHSV, HSV, colorTransitionCurrentTime / COLOR_TRANSITION_DURATION);

        glm::vec3 color = glm::rgbColor(currentHSV);

        audio_visualizer::drawWaveform(waveformTextureBytes, waveformTextureWidth, waveformTextureHeight, color, input->bufferData);
        textures.back()->update(waveformTextureBytes.data(), waveformTextureWidth, waveformTextureHeight, GL_RGBA);

        double imageTransitionCurrentTime = window->getCurrentTime() - imageTransitionStartTime;
        if (imageTransitionCurrentTime < IMAGE_TRANSITION_DURATION)
        {
            if (imageTransitionCurrentTime < IMAGE_TRANSITION_DURATION / 2.0)
            {
                bgOverlayColor->value.a = (imageTransitionCurrentTime / (IMAGE_TRANSITION_DURATION / 2.0)) * 0.7 + 0.3;
            }
            else
            {
                if (!isImageTransitionNewImageSet)
                {
                    imageElements[0]->setImage(textures[imageIndex]);
                    isImageTransitionNewImageSet = true;
                }
                bgOverlayColor->value.a = 1.0 - ((imageTransitionCurrentTime - (IMAGE_TRANSITION_DURATION / 2.0)) / (IMAGE_TRANSITION_DURATION / 2.0)) * 0.7;
            }
        }
        else
        {
            double audioPeak = 0.0;
            if (window->isLmbDown())
            {
                for (int i = 0; i < input->BUFFER_SIZE; i++)
                {
                    double _abs = abs(input->bufferData[i]);
                    if (_abs > audioPeak)
                        audioPeak = _abs;
                }
            }
            else
            {
                frequencyPlan = fftw_plan_dft_r2c_1d(input->BUFFER_SIZE, input->bufferData.data(), frequencyComplex, FFTW_ESTIMATE);
                fftw_execute(frequencyPlan);

                for (size_t i = 0; i < frequencyDomain.size(); i++)
                    frequencyDomain[i] = 20.0 * log10(sqrt(pow(frequencyComplex[i + 1][0], 2) + pow(frequencyComplex[i + 1][1], 2)));

                fftw_destroy_plan(frequencyPlan);

                for (int i = 0; i < 10; i++)
                    if (frequencyDomain[i] > audioPeak)
                        audioPeak = frequencyDomain[i];

                audioPeak /= 50.0;
            }

            double prevAudioPeakAverage = 0.0;
            for (int i = 0; i < prevAudioPeak.size(); i++)
                prevAudioPeakAverage += prevAudioPeak[i];

            prevAudioPeakAverage /= prevAudioPeak.size();

            double audioPeakOutput = (audioPeak - prevAudioPeakAverage) / (1.0 - prevAudioPeakAverage);
            if (audioPeakOutput > 1.0)
                audioPeakOutput = 1.0;
            else if (audioPeakOutput < 0.0)
                audioPeakOutput = 0.0;

            if (audioPeakOutput - prevAudioPeakOutput < -0.1)
                audioPeakOutput = prevAudioPeakOutput - 0.1;

            bgOverlayColor->value.a = (1.0 - audioPeakOutput * 0.3) - 0.7;

            prevAudioPeak.erase(prevAudioPeak.begin());
            prevAudioPeak.push_back(audioPeak);
        }

        elements[0]->styleChange = true;

        guiRenderer->update();

        glClear(GL_COLOR_BUFFER_BIT);

        guiRenderer->draw();

        window->swapBuffers();
    }
}

void audio_visualizer::App::setHSV(glm::vec3 _newHSV)
{
    prevHSV = HSV;
    HSV = _newHSV;
    colorTransitionStartTime = window->getCurrentTime();
    logData();
}
void audio_visualizer::App::setMultiplier(float _newMultiplier)
{
    input->setMultiplier(_newMultiplier);
    logData();
}
void audio_visualizer::App::setImage(int _newImageIndex)
{
    if (_newImageIndex >= 0 && _newImageIndex < textures.size() - 1 && _newImageIndex != imageIndex)
    {
        isImageTransitionNewImageSet = false;
        imageTransitionStartTime = window->getCurrentTime();
        imageIndex = _newImageIndex;
        logData();
    }
}

void audio_visualizer::App::logData()
{
    std::cout << "\033[1;34m--------\033[0m\n"
              << "HSV = " << HSV[0] << ", " << HSV[1] << ", " << HSV[2] << '\n'
              << "Multiplier = " << input->getMultiplier() << '\n'
              << "Image = " << imageIndex + 1 << '\n'
              << "\033[1;34m--------\033[0m\n";
}