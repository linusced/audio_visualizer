#ifndef AUDIO_VISUALIZER
#define AUDIO_VISUALIZER

#include "../opengl_gui/opengl_gui.hpp"
#include "draw_waveform.hpp"
#include "audio_input.hpp"

#include <glm/gtx/color_space.hpp>
#include <fftw3.h>

namespace audio_visualizer
{
    class App : public opengl_gui::DrawLoop
    {
    public:
        App(std::string cssCode, opengl_gui::Window *window);

        void terminate() override;

        void loop() override;

    private:
        std::vector<unsigned char> waveformTextureBytes;
        int waveformTextureWidth, waveformTextureHeight;
        opengl_gui::Texture *waveformTexture = nullptr;

        AudioInput input;

        std::vector<double> timeDomain, frequencyDomain;
        fftw_complex *frequencyComplex = nullptr;
        fftw_plan frequencyPlan;

        std::vector<opengl_gui::Element *> lights;
        std::vector<glm::vec4 *> lightColors;

        double getFrequencyArrayAverage(int startIndex, int size, int *nextStartIndex);
    };
}

#endif