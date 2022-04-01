#ifndef AUDIO_VISUALIZER_DRAW_WAVEFORM
#define AUDIO_VISUALIZER_DRAW_WAVEFORM

#include <iostream>
#include <cmath>
#include <vector>
#include <glm/glm.hpp>

namespace audio_visualizer
{
    void drawWaveform(std::vector<unsigned char> &textureBytes, int textureWidth, int textureHeight, glm::vec3 color, const std::vector<int16_t> &audioData);
}

#endif