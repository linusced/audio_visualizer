#include "../../header/audio_visualizer/audio_visualizer.hpp"

const double audio_visualizer::App::COLOR_TRANSITION_DURATION = 4.0,
             audio_visualizer::App::IMAGE_TRANSITION_DURATION = 1.0;

audio_visualizer::App::App(bool *stop, std::string cssCode, std::string fontFilePath, std::vector<std::string> &bgImageFilePaths, std::vector<std::string> &lyricsFilePaths, opengl_gui::Window *window)
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

    audioPeakFrequencySize = ceil((150.0 / (double)(input->SAMPLE_RATE / 2)) * frequencyDomain.size());

    fonts.push_back(new opengl_gui::Font(fontFilePath));
    textElements.push_back(new opengl_gui::TextElement(fonts[0], "", "text", false));
    guiRenderer->addElement(textElements[0]);

    textColor = &textElements[0]->elementStyle.colorProperties["color"];
    textColor->isSet = true;
    textColor->value = glm::vec4(1.0f);

    std::cout << "Parsing lyrics\n";

    for (auto &str : lyricsFilePaths)
        parseLyrics(opengl_gui::textFileData(str));
}

void audio_visualizer::App::terminate()
{
    fftw_free(frequencyComplex);

    delete input;

    for (auto &i : trackLyricsMap)
        delete i.second;

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

void audio_visualizer::App::setHSV(glm::vec3 HSV)
{
    prevHSV = this->HSV;
    this->HSV = HSV;
    colorTransitionStartTime = window->getCurrentTime();
    logData();
}
void audio_visualizer::App::setMultiplier(float multiplier)
{
    input->setMultiplier(multiplier);
    logData();
}
void audio_visualizer::App::setImage(int imageIndex)
{
    if (imageIndex >= 0 && imageIndex < textures.size() - 1 && imageIndex != imageIndex)
    {
        isImageTransitionNewImageSet = false;
        imageTransitionStartTime = window->getCurrentTime();
        this->imageIndex = imageIndex;
        logData();
    }
}
void audio_visualizer::App::setLyrics(std::string name)
{
    activeTrackLyricsName = name;
    if (activeTrackLyricsName.size() > 0)
    {
        auto it = trackLyricsMap.find(activeTrackLyricsName);
        if (it != trackLyricsMap.end())
        {
            activeTrackLyrics = it->second;
            activeTrackLyrics->timeOffset = window->getCurrentTime();
            activeTrackLyrics->lyricsIndex = 0;
        }
        else
            activeTrackLyricsName.clear();
    }
    logData();

    timerDuration = 0.0;
    clearText = true;
}
void audio_visualizer::App::setLyricsTime(double time)
{
    if (activeTrackLyricsName.size() != 0)
    {
        activeTrackLyrics->timeOffset = window->getCurrentTime() - time;
        for (int i = 0; i < activeTrackLyrics->data.size(); i++)
            if (activeTrackLyrics->data[i].time > time)
            {
                activeTrackLyrics->lyricsIndex = i - 1;
                break;
            }

        if (activeTrackLyrics->lyricsIndex == -1)
            activeTrackLyrics->lyricsIndex = 0;

        clearText = true;
    }
}
void audio_visualizer::App::setTimer(double time)
{
    timerStart = window->getCurrentTime();
    timerDuration = abs(time);

    activeTrackLyricsName.clear();
    if (timerDuration == 0.0)
        clearText = true;

    logData();
}

void audio_visualizer::App::logData()
{
    std::cout << "\033[1;34m--------\033[0m\n"
              << "HSV = " << HSV[0] << ", " << HSV[1] << ", " << HSV[2] << '\n'
              << "Multiplier = " << input->getMultiplier() << '\n'
              << "Image = " << imageIndex + 1 << '\n'
              << "Track Lyrics = " << activeTrackLyricsName << '\n'
              << "Timer = " << timerDuration << '\n'
              << "\033[1;34m--------\033[0m\n";
}