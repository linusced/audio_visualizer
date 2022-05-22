#include "../../header/audio_visualizer/audio_visualizer.hpp"

void audio_visualizer::App::loop()
{
    logData();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    input->start();

    while (!*stop && !window->shouldClose())
    {
        window->update();

        if (window->hasResized())
            guiRenderer->resize();

        glm::vec3 currentHSV = HSV;
        double colorTransitionCurrentTime = window->getCurrentTime() - colorTransitionStartTime;
        if (colorTransitionCurrentTime < COLOR_TRANSITION_DURATION)
            currentHSV = glm::mix(prevHSV, HSV, colorTransitionCurrentTime / COLOR_TRANSITION_DURATION);

        glm::vec3 color = glm::rgbColor(currentHSV);

        audio_visualizer::drawWaveform(waveformTextureBytes, waveformTextureWidth, waveformTextureHeight, color, input->bufferData);
        textures.back()->update(waveformTextureBytes.data(), waveformTextureWidth, waveformTextureHeight, GL_RGBA);

        double imageTransitionCurrentTime = window->getCurrentTime() - imageTransitionStartTime;
        if (imageTransitionCurrentTime < IMAGE_TRANSITION_DURATION)
        {
            if (imageTransitionCurrentTime < IMAGE_TRANSITION_DURATION / 2.0)
            {
                bgOverlayColor->value.a = (imageTransitionCurrentTime / (IMAGE_TRANSITION_DURATION / 2.0));
            }
            else
            {
                if (!isImageTransitionNewImageSet)
                {
                    imageElements[0]->setImage(textures[imageIndex]);
                    isImageTransitionNewImageSet = true;
                }
                bgOverlayColor->value.a = 1.0 - ((imageTransitionCurrentTime - (IMAGE_TRANSITION_DURATION / 2.0)) / (IMAGE_TRANSITION_DURATION / 2.0));
            }
            elements[0]->styleChange = true;
        }

        textColor->value = glm::vec4(color, 1.0f);
        textElements[0]->styleChange = true;

        if (timerDuration != 0.0)
        {
            double timerCurrentTime = window->getCurrentTime() - timerStart;
            if (timerCurrentTime < timerDuration)
            {
                timerCurrentTime = timerDuration - timerCurrentTime;

                double dMinutes = timerCurrentTime / 60.0;
                int minutes = dMinutes,
                    seconds = (dMinutes - minutes) * 60;

                if (timerPrevSeconds == -1)
                    timerPrevSeconds = seconds;
                else if (seconds != timerPrevSeconds)
                {
                    std::string str;
                    if (minutes < 10)
                        str += '0';
                    str += std::to_string(minutes) + ':';
                    if (seconds < 10)
                        str += '0';
                    str += std::to_string(seconds);

                    textElements[0]->setText(str);

                    timerPrevSeconds = seconds;
                }
            }
            else
            {
                timerDuration = 0.0;
                textElements[0]->setText("");
                logData();
            }
        }

        guiRenderer->update();

        glClear(GL_COLOR_BUFFER_BIT);

        guiRenderer->draw();

        window->swapBuffers();
    }
}