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
                bgOverlayColor->value.a = (imageTransitionCurrentTime / (IMAGE_TRANSITION_DURATION / 2.0)) * (1 - beatIntensity) + beatIntensity;
            }
            else
            {
                if (!isImageTransitionNewImageSet)
                {
                    imageElements[0]->setImage(textures[imageIndex]);
                    isImageTransitionNewImageSet = true;
                }
                bgOverlayColor->value.a = 1.0 - ((imageTransitionCurrentTime - (IMAGE_TRANSITION_DURATION / 2.0)) / (IMAGE_TRANSITION_DURATION / 2.0)) * (1 - beatIntensity);
            }
        }
        else
        {
            double audioPeak = 0.0;
            frequencyPlan = fftw_plan_dft_r2c_1d(input->BUFFER_SIZE, input->bufferData.data(), frequencyComplex, FFTW_ESTIMATE);
            fftw_execute(frequencyPlan);

            for (size_t i = 0; i < frequencyDomain.size(); i++)
                frequencyDomain[i] = 20.0 * log10(sqrt(pow(frequencyComplex[i + 1][0], 2) + pow(frequencyComplex[i + 1][1], 2)));

            fftw_destroy_plan(frequencyPlan);

            for (int i = 0; i < audioPeakFrequencySize; i++)
                if (frequencyDomain[i] > audioPeak)
                    audioPeak = frequencyDomain[i];

            audioPeak /= 50.0;

            double prevAudioPeakAverage = 0.0;
            for (int i = 0; i < prevAudioPeak.size(); i++)
                prevAudioPeakAverage += prevAudioPeak[i];

            prevAudioPeakAverage /= prevAudioPeak.size();

            double audioPeakOutput = (audioPeak - prevAudioPeakAverage) / (1.0 - prevAudioPeakAverage);
            if (audioPeakOutput > 1.0)
                audioPeakOutput = 1.0;
            else if (audioPeakOutput < 0.0)
                audioPeakOutput = 0.0;

            if (audioPeakOutput - prevAudioPeakOutput < -0.03)
                audioPeakOutput = prevAudioPeakOutput - 0.03;

            bgOverlayColor->value.a = (1.0 - audioPeakOutput * beatIntensity) - (1 - beatIntensity);

            prevAudioPeak.erase(prevAudioPeak.begin());
            prevAudioPeak.push_back(audioPeak);
        }

        elements[0]->styleChange = true;

        textColor->value = glm::vec4(color, 1.0f);
        textElements[0]->styleChange = true;

        if (clearText)
        {
            textElements[0]->setText("");
            clearText = false;
        }
        else if (timerDuration != 0.0)
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
        else if (!activeLyricsName.empty())
        {
            double lyricsTime = window->getCurrentTime() - activeLyricsStartTime;
            if (activeLyricsIndex < activeLyrics->size() && activeLyrics->at(activeLyricsIndex + 1).time < lyricsTime)
            {
                activeLyricsIndex++;
                textElements[0]->setText(activeLyrics->at(activeLyricsIndex).str);
            }
        }

        guiRenderer->update();

        glClear(GL_COLOR_BUFFER_BIT);

        guiRenderer->draw();

        window->swapBuffers();
    }
}