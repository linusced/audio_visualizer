#ifndef AUDIO_VISUALIZER
#define AUDIO_VISUALIZER

#include "../opengl_gui/opengl_gui.hpp"
#include "draw_functions.hpp"
#include "audio_input.hpp"
#include <glm/gtx/color_space.hpp>
#include <fftw3.h>

namespace audio_visualizer
{
    class App : public opengl_gui::DrawLoop
    {
    public:
        App(bool *stop, std::string cssCode, std::string fontFilePath, std::vector<std::string> &bgImageFilePaths, std::vector<std::string> &lyricsFilePaths, opengl_gui::Window *window);

        void terminate() override;

        void loop() override;

        void setHSV(glm::vec3 HSV);
        void setMultiplier(float multiplier);
        void setImage(int imageIndex);
        void setLyrics(std::string name);
        void setLyricsTime(double time);
        void setTimer(double time);

    private:
        static const double COLOR_TRANSITION_DURATION, IMAGE_TRANSITION_DURATION;

        struct LyricsLine
        {
            double time;
            std::string str;
        };
        struct TrackLyrics
        {
            double timeOffset;
            int lyricsIndex;
            std::vector<LyricsLine> data;
        };

        void logData();

        void parseLyrics(std::string lyricsFileStr);

        bool *stop = nullptr;
        AudioInput *input = nullptr;

        std::vector<unsigned char> waveformTextureBytes;
        int waveformTextureWidth, waveformTextureHeight;

        glm::vec3 HSV = glm::vec3(0.0f, 0.0f, 1.0f), prevHSV;
        double colorTransitionStartTime = -100.0;

        std::vector<double> prevAudioPeak;
        double prevAudioPeakOutput = 0.0;

        opengl_gui::Style::COLOR *bgOverlayColor = nullptr, *textColor = nullptr;

        int imageIndex = 0;
        double imageTransitionStartTime = -100.0;
        bool isImageTransitionNewImageSet = false;

        std::vector<double> frequencyDomain;
        fftw_complex *frequencyComplex = nullptr;
        fftw_plan frequencyPlan;
        int audioPeakFrequencySize;

        bool clearText = false;

        std::map<std::string, TrackLyrics *> trackLyricsMap;
        TrackLyrics *activeTrackLyrics = nullptr;
        std::string activeTrackLyricsName;

        double timerStart = 0.0, timerDuration = 0.0;
        int timerPrevSeconds = -1;
    };
}

#endif