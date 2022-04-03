#include "../../header/audio_visualizer/audio_visualizer.hpp"

const double audio_visualizer::App::COLOR_TRANSITION_DURATION = 4.0;

audio_visualizer::App::App(bool *stop, std::string cssCode, std::string fontFilePath, std::vector<std::string> bgImageFilePaths, opengl_gui::Window *window)
    : opengl_gui::DrawLoop(window)
{
    this->stop = stop;
    input = new AudioInput();

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

    waveformTextureWidth = opengl_gui::Window::getMonitorWidth();
    waveformTextureHeight = opengl_gui::Window::getMonitorHeight() * 0.3f;
    waveformTextureBytes.resize((waveformTextureWidth * waveformTextureHeight) * 4);

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

    fonts.push_back(new opengl_gui::Font(fontFilePath));

    textElements.push_back(new opengl_gui::TextElement(fonts[0], "", "text-1", false));
    guiRenderer->addElement(textElements[0]);

    textColor1 = &textElements[0]->elementStyle.colorProperties["color"];
    textColor1->isSet = true;

    textElements.push_back(new opengl_gui::TextElement(fonts[0], "", "text-2", false));
    guiRenderer->addElement(textElements[1]);

    textColor2 = &textElements[1]->elementStyle.colorProperties["color"];
    textColor2->isSet = true;
}

void audio_visualizer::App::terminate()
{
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

    while (!*stop && !window->shouldClose())
    {
        window->update();

        if (window->hasResized())
            guiRenderer->resize();

        input->update();

        glm::vec3 currentHSV = HSV;
        double colorTransitionCurrentTime = window->getCurrentTime() - colorTransitionStartTime;

        if (colorTransitionCurrentTime < COLOR_TRANSITION_DURATION)
            currentHSV = glm::mix(prevHSV, HSV, colorTransitionCurrentTime / COLOR_TRANSITION_DURATION);

        glm::vec3 color = glm::rgbColor(currentHSV);

        audio_visualizer::drawWaveform(waveformTextureBytes, waveformTextureWidth, waveformTextureHeight, color, input->getAudioData());
        textures.back()->update(waveformTextureBytes.data(), waveformTextureWidth, waveformTextureHeight, GL_RGBA);

        textColor1->value = textColor2->value = glm::vec4(color, 1.0f);
        textElements[0]->styleChange = textElements[1]->styleChange = true;

        int audioPeak = 0;
        for (int i = 0; i < input->getAudioData().size(); i++)
            if (abs(input->getAudioData()[i]) > audioPeak)
                audioPeak = abs(input->getAudioData()[i]);

        float fAudioPeak = audioPeak / (powf(2, 16) / 2.0f - 1.0f);
        if (fAudioPeak - prevAudioPeak > 0.4f)
            fAudioPeak = prevAudioPeak + 0.4f;
        else if (fAudioPeak - prevAudioPeak < -0.05f)
            fAudioPeak = prevAudioPeak - 0.05f;

        prevAudioPeak = fAudioPeak;

        bgOverlayColor->value.a = (1.0f - fAudioPeak * 0.3f) - 0.7f;
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
void audio_visualizer::App::setText(std::string _newText)
{
    size_t lineSplit = _newText.find('\n');
    textElements[0]->setText(_newText.substr(0, lineSplit));
    textElements[1]->setText(lineSplit == std::string::npos ? "" : _newText.substr(lineSplit + 1));
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
              << "Text = " << textElements[0]->getText() << "\t" << textElements[1]->getText() << '\n'
              << "Multiplier = " << input->getMultiplier() << '\n'
              << "Image = " << imageIndex + 1 << '\n'
              << "\033[1;34m--------\033[0m\n";
}