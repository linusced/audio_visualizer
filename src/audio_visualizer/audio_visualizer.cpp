#include "../../header/audio_visualizer/audio_visualizer.hpp"

const double audio_visualizer::App::COLOR_TRANSITION_DURATION = 4.0;

audio_visualizer::App::App(bool *stop, std::string cssCode, std::vector<std::string> bgImageFilePaths, opengl_gui::Window *window)
    : opengl_gui::DrawLoop(window)
{
    this->stop = stop;
    input = new AudioInput();
    beatTrack = new BTrack(input->BUFFER_SIZE / 2, input->BUFFER_SIZE);

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
}

void audio_visualizer::App::terminate()
{
    delete input;
    delete beatTrack;

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

        beatTrack->processAudioFrame(input->bufferData.data());

        float audioPeak = beatTrack->beatDueInCurrentFrame();
        if (audioPeak - prevAudioPeak < -0.1f)
            audioPeak = prevAudioPeak - 0.1f;

        prevAudioPeak = audioPeak;

        bgOverlayColor->value.a = (1.0f - audioPeak * 0.3f) - 0.7f;
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
    if (_newImageIndex >= 0 && _newImageIndex < textures.size() - 1)
    {
        imageElements[0]->setImage(textures[_newImageIndex]);
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