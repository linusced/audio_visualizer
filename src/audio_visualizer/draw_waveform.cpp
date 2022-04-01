#include "../../header/audio_visualizer/draw_waveform.hpp"

void audio_visualizer::drawWaveform(std::vector<unsigned char> &textureBytes, int textureWidth, int textureHeight, glm::vec3 color, const std::vector<int16_t> &audioData)
{
    for (size_t i = 3; i < textureBytes.size(); i += 4)
        textureBytes[i] = 0;

    auto drawPixel = [&](int x, int y)
    {
        size_t i = (y * (size_t)textureWidth + x) * 4UL;

        textureBytes[i] = color.r * 255;
        textureBytes[i + 1] = color.g * 255;
        textureBytes[i + 2] = color.b * 255;
        textureBytes[i + 3] = 255;
    };

    float step = audioData.size() / (float)textureWidth,
          maxValue = powf(2, 16) / 2 - 1;

    int prevY = -1;

    for (int x = 0; x < textureWidth; x++)
    {
        int y = ((audioData[x * step] / maxValue) * 0.5f + 0.5f) * textureHeight;

        if (y == prevY || prevY == -1)
        {
            y += (y < textureHeight / 2) ? 2 : -2;
            prevY = y;
        }

        bool greaterThan = y >= prevY;

        for (int y2 = y; (greaterThan ? (y2 >= prevY) : (y2 <= prevY)); y2 += (greaterThan ? -1 : 1))
        {
            drawPixel(x, y2);
        }

        prevY = y;
    }
}