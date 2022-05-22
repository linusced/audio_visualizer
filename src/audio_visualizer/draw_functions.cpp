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

void audio_visualizer::drawGradient(std::vector<unsigned char> &textureBytes, int textureWidth, int textureHeight, float multiplier)
{
    auto drawPixel = [&](int x, int y, float alpha)
    {
        size_t i = (y * (size_t)textureWidth + x) * 4UL;

        textureBytes[i] = textureBytes[i + 1] = textureBytes[i + 2] = 0;
        textureBytes[i + 3] = alpha * 255;
    };

    float alpha,
        gradientHeight = textureHeight / 3.0f, topGradientStart = textureHeight - gradientHeight;

    for (int y = 0; y < textureHeight; y++)
        for (int x = 0; x < textureWidth; x++)
        {
            if (y < gradientHeight)
                alpha = (y / gradientHeight) * multiplier;
            else if (y > topGradientStart)
                alpha = multiplier - ((y - topGradientStart) / gradientHeight) * multiplier;
            else
                alpha = multiplier;

            drawPixel(x, y, alpha);
        }
}