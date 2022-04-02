#include "../../header/audio_visualizer/audio_visualizer.hpp"

const double audio_visualizer::App::COLOR_TRANSITION_DURATION = 4.0;

audio_visualizer::App::App(bool *stop, std::string cssCode, std::string fontFilePath, std::string bgImageFilePath, opengl_gui::Window *window)
    : opengl_gui::DrawLoop(window)
{
    this->stop = stop;

    waveformTextureWidth = opengl_gui::Window::getMonitorWidth();
    waveformTextureHeight = opengl_gui::Window::getMonitorHeight() * 0.3f;
    waveformTextureBytes.resize((waveformTextureWidth * waveformTextureHeight) * 4);

    opengl_gui::TextureData bgImageData(bgImageFilePath);

    textures.push_back(new opengl_gui::Texture(bgImageData.bytes, bgImageData.width, bgImageData.height, bgImageData.format));
    textures.push_back(new opengl_gui::Texture(waveformTextureBytes.data(), waveformTextureWidth, waveformTextureHeight, GL_RGBA));

    guiRenderer = new opengl_gui::Renderer(cssCode, window);

    imageElements.push_back(new opengl_gui::ImageElement(textures[0], "bg-image"));
    guiRenderer->addElement(imageElements[0]);

    bgTranslateX = &imageElements[0]->elementStyle.sizeProperties["translate-x"];
    bgTranslateX->isSet = true;
    bgTranslateX->unit = opengl_gui::Style::PERCENTAGE;

    bgTranslateY = &imageElements[0]->elementStyle.sizeProperties["translate-y"];
    bgTranslateY->isSet = true;
    bgTranslateY->unit = opengl_gui::Style::PERCENTAGE;

    elements.push_back(new opengl_gui::Element("fullscreen"));
    guiRenderer->addElement(elements[0]);

    bgOverlayColor = &elements[0]->elementStyle.colorProperties["background-color"];
    bgOverlayColor->isSet = true;

    imageElements.push_back(new opengl_gui::ImageElement(textures[1], "waveform"));
    guiRenderer->addElement(imageElements[1]);

    fonts.push_back(new opengl_gui::Font(fontFilePath));

    textElements.push_back(new opengl_gui::TextElement(fonts[0], "", "text"));
    guiRenderer->addElement(textElements[0]);

    textColor = &textElements[0]->elementStyle.colorProperties["color"];
    textColor->isSet = true;
    textColor->value = glm::vec4(glm::rgbColor(HSV), 1.0f);
}

void audio_visualizer::App::loop()
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    while (!*stop)
    {
        window->update();
        if (window->hasResized())
            guiRenderer->resize();

        input.update();

        glm::vec3 currentHSV = HSV;
        double colorTransitionCurrentTime = window->getCurrentTime() - colorTransitionStartTime;

        if (colorTransitionCurrentTime < COLOR_TRANSITION_DURATION)
            currentHSV = glm::mix(prevHSV, HSV, colorTransitionCurrentTime / COLOR_TRANSITION_DURATION);

        glm::vec3 color = glm::rgbColor(currentHSV);

        audio_visualizer::drawWaveform(waveformTextureBytes, waveformTextureWidth, waveformTextureHeight, color, input.getAudioData());
        textures[1]->update(waveformTextureBytes.data(), waveformTextureWidth, waveformTextureHeight, GL_RGBA);

        textColor->value = glm::vec4(color, 1.0f);
        textElements[0]->styleChange = true;

        int audioPeak = 0;
        for (int i = 0; i < input.getAudioData().size(); i++)
            if (abs(input.getAudioData()[i]) > audioPeak)
                audioPeak = abs(input.getAudioData()[i]);

        float fAudioPeak = audioPeak / (powf(2, 16) / 2.0f - 1.0f);
        if (fAudioPeak - prevAudioPeak > 0.4f)
            fAudioPeak = prevAudioPeak + 0.4f;
        else if (fAudioPeak - prevAudioPeak < -0.05f)
            fAudioPeak = prevAudioPeak - 0.05f;

        prevAudioPeak = fAudioPeak;

        bgOverlayColor->value = glm::vec4(color * 0.1f, 1.0f - fAudioPeak * 0.5f);
        elements[0]->styleChange = true;

        float translateValue = window->getCurrentTime() * 0.3f;
        imageElements[0]->styleChange = true;
        bgTranslateX->value = -(sinf(translateValue) * 0.5f + 0.5f) * 10.0f;
        bgTranslateY->value = -(cosf(translateValue) * 0.5f + 0.5f) * 10.0f;

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
}
void audio_visualizer::App::setText(std::string _newText)
{
    textElements[0]->setText(_newText);
}
void audio_visualizer::App::setMultiplier(float _newMultiplier)
{
    input.setMultiplier(_newMultiplier);
}