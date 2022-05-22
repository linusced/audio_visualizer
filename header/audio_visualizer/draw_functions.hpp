#ifndef AUDIO_VISUALIZER_DRAW_FUNCTIONS
#define AUDIO_VISUALIZER_DRAW_FUNCTIONS

#include <iostream>
#include <cmath>
#include <vector>
#include <glm/glm.hpp>

namespace audio_visualizer
{
    void drawWaveform(std::vector<unsigned char> &textureBytes, int textureWidth, int textureHeight, glm::vec3 color, const std::vector<double> &audioData);

    void drawGradient(std::vector<unsigned char> &textureBytes, int textureWidth, int textureHeight, float multiplier);
}

#endif