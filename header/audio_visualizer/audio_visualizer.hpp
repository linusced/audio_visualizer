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
        App(bool *stop, std::string cssCode, std::vector<std::string> bgImageFilePaths, opengl_gui::Window *window);

        void terminate() override;

        void loop() override;

        void setHSV(glm::vec3 _newHSV);
        void setMultiplier(float _newMultiplier);
        void setImage(int _newImageIndex);

    private:
        static const double COLOR_TRANSITION_DURATION, IMAGE_TRANSITION_DURATION;

        bool *stop = nullptr;
        AudioInput *input = nullptr;

        std::vector<unsigned char> waveformTextureBytes;
        int waveformTextureWidth, waveformTextureHeight;

        glm::vec3 HSV = glm::vec3(0.0f, 0.0f, 1.0f), prevHSV;
        double colorTransitionStartTime = -100.0;

        std::vector<double> prevAudioPeak;
        double prevAudioPeakOutput = 0.0;

        opengl_gui::Style::COLOR *bgOverlayColor = nullptr;

        int imageIndex = 0;
        double imageTransitionStartTime = -100.0;
        bool isImageTransitionNewImageSet = false;

        std::vector<double> frequencyDomain;
        fftw_complex *frequencyComplex = nullptr;
        fftw_plan frequencyPlan;

        void logData();
    };
}

#endif