#include "../../header/audio_visualizer/draw_functions.hpp"

void audio_visualizer::drawWaveform(std::vector<unsigned char> &textureBytes, int textureWidth, int textureHeight, glm::vec3 color, const std::vector<double> &audioData)
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

    int prevY = -1;

    for (int x = 0; x < textureWidth; x++)
    {
        int y = (audioData[x] * 0.5 + 0.5) * textureHeight;

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

void audio_visualizer::drawFrequencies(std::vector<unsigned char> &textureBytes, int textureWidth, int textureHeight, glm::vec3 color, const std::vector<double> &frequencyData)
{
    for (size_t i = 3; i < textureBytes.size(); i += 4)
        textureBytes[i] = 0;

    auto fillRect = [&](int x, int y, int w, int h)
    {
        int x1 = x, y1 = y, x2 = x + w, y2 = y + h;
        for (x = x1; x < x2; x++)
            for (y = y1; y < y2; y++)
            {
                size_t i = (y * (size_t)textureWidth + x) * 4UL;
                if (i + 4 < textureBytes.size())
                {
                    textureBytes[i] = color.r * 255;
                    textureBytes[i + 1] = color.g * 255;
                    textureBytes[i + 2] = color.b * 255;
                    textureBytes[i + 3] = 255;
                }
            }
    };

    for (int x = 0; x < textureWidth; x++)
        fillRect(x, 0, 1, (frequencyData[x] / 50.0) * textureHeight);
}