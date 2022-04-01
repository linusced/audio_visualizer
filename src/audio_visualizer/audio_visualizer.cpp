#include "../../header/audio_visualizer/audio_visualizer.hpp"

audio_visualizer::App::App(std::string cssCode, opengl_gui::Window *window)
    : opengl_gui::DrawLoop(window)
{
    waveformTextureWidth = opengl_gui::Window::getMonitorWidth();
    waveformTextureHeight = opengl_gui::Window::getMonitorHeight();
    waveformTextureBytes.resize((waveformTextureWidth * waveformTextureHeight) * 4);

    waveformTexture = new opengl_gui::Texture(waveformTextureBytes.data(), waveformTextureWidth, waveformTextureHeight, GL_RGBA);
    textures.push_back(waveformTexture);

    guiRenderer = new opengl_gui::Renderer(cssCode, window);

    for (int i = 0; i < 12; i++)
    {
        lights.push_back(new opengl_gui::Element("light light-" + std::to_string(i)));
        lights.back()->elementStyle.colorProperties["background-color"].isSet = true;
        lightColors.push_back(&lights.back()->elementStyle.colorProperties["background-color"].value);
        elements.push_back(lights.back());
        guiRenderer->addElement(lights.back());
    }

    elements.push_back(new opengl_gui::ImageElement(textures.back(), "waveform"));
    guiRenderer->addElement(elements.back());
}

void audio_visualizer::App::terminate()
{
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

    if (frequencyComplex)
        fftw_free(frequencyComplex);
}

void audio_visualizer::App::loop()
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    while (!window->shouldClose())
    {
        window->update();
        if (window->hasResized())
            guiRenderer->resize();

        input.update();

        if (glfwGetKey(*window, GLFW_KEY_ENTER))
            glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        else if (glfwGetKey(*window, GLFW_KEY_ESCAPE))
            glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        glm::vec3 color = glm::rgbColor(glm::vec3((int)(window->getCurrentTime() * 5.0) % 360, 1.0f, 1.0f));

        audio_visualizer::drawWaveform(waveformTextureBytes, waveformTextureWidth, waveformTextureHeight, color, input.getAudioData());
        waveformTexture->update(waveformTextureBytes.data(), waveformTextureWidth, waveformTextureHeight, GL_RGBA);

        if (timeDomain.size() != input.getAudioData().size())
            timeDomain.resize(input.getAudioData().size());

        for (size_t i = 0; i < input.getAudioData().size(); i++)
            timeDomain[i] = input.getAudioData()[i] / (double)(pow(2, 16) / 2 - 1);

        if (frequencyDomain.size() != timeDomain.size() / 2 + 1)
        {
            frequencyDomain.resize(timeDomain.size() / 2 + 1);

            if (frequencyComplex)
                fftw_free(frequencyComplex);
            frequencyComplex = (fftw_complex *)calloc(frequencyDomain.size(), sizeof(fftw_complex));
        }

        frequencyPlan = fftw_plan_dft_r2c_1d(timeDomain.size(), timeDomain.data(), frequencyComplex, FFTW_ESTIMATE);
        fftw_execute(frequencyPlan);

        for (size_t i = 0; i < frequencyDomain.size(); i++)
            frequencyDomain[i] = 20.0 * log10(sqrt(pow(frequencyComplex[i][0], 2) + pow(frequencyComplex[i][1], 2)));

        int startIndex = 1;
        double value = getFrequencyArrayAverage(startIndex, (frequencyDomain.size() - 1) * (100.0f / 20000.0f), &startIndex);
        *lightColors[8] = *lightColors[9] = glm::vec4(color, value / 30.0f);

        value = getFrequencyArrayAverage(startIndex, (frequencyDomain.size() - 1) * (200.0f / 20000.0f), &startIndex);
        *lightColors[7] = *lightColors[10] = glm::vec4(color, value / 30.0f);

        value = getFrequencyArrayAverage(startIndex, (frequencyDomain.size() - 1) * (200.0f / 20000.0f), &startIndex);
        *lightColors[6] = *lightColors[11] = glm::vec4(color, value / 30.0f);

        value = getFrequencyArrayAverage(startIndex, (frequencyDomain.size() - 1) * (1000.0f / 20000.0f), &startIndex);
        *lightColors[0] = *lightColors[5] = glm::vec4(color, value / 30.0f);

        value = getFrequencyArrayAverage(startIndex, (frequencyDomain.size() - 1) * (2000.0f / 20000.0f), &startIndex);
        *lightColors[1] = *lightColors[4] = glm::vec4(color, value / 30.0f);

        value = getFrequencyArrayAverage(startIndex, (frequencyDomain.size() - 1) * (2000.0f / 20000.0f), &startIndex);
        *lightColors[2] = *lightColors[3] = glm::vec4(color, value / 30.0f);

        for (auto &l : lights)
            l->styleChange = true;

        fftw_destroy_plan(frequencyPlan);

        guiRenderer->update();

        glClear(GL_COLOR_BUFFER_BIT);

        guiRenderer->draw();

        window->swapBuffers();
    }
}

double audio_visualizer::App::getFrequencyArrayAverage(int startIndex, int size, int *nextStartIndex)
{
    double average = 0.0;

    for (int i = startIndex; i < startIndex + size; i++)
        average += frequencyDomain[i];

    average /= size;

    *nextStartIndex = startIndex + size;

    return average;
}