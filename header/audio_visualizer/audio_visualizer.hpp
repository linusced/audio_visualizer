#ifndef AUDIO_VISUALIZER
#define AUDIO_VISUALIZER

#include "../opengl_gui/opengl_gui.hpp"
#include "draw_waveform.hpp"
#include "audio_input.hpp"
#include <glm/gtx/color_space.hpp>

namespace audio_visualizer
{
    class App : public opengl_gui::DrawLoop
    {
    public:
        App(bool *stop, std::string cssCode, std::string fontFilePath, std::string bgImageFilePath, opengl_gui::Window *window);

        void loop() override;

        void setHSV(glm::vec3 _newHSV);
        void setText(std::string _newText);

    private:
        static const double COLOR_TRANSITION_DURATION;

        bool *stop = nullptr;
        AudioInput input;

        std::vector<unsigned char> waveformTextureBytes;
        int waveformTextureWidth, waveformTextureHeight;

        glm::vec3 HSV = glm::vec3(0.0f, 0.0f, 1.0f), prevHSV;
        double colorTransitionStartTime = -100.0;

        float prevAudioPeak = 0.0f;

        opengl_gui::Style::COLOR *textColor = nullptr;
        opengl_gui::Style::COLOR *bgOverlayColor = nullptr;
    };
}

#endif