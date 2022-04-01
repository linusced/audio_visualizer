#ifndef AUDIO_VISUALIZER_AUDIO_INPUT
#define AUDIO_VISUALIZER_AUDIO_INPUT

#include <cmath>
#include <iostream>
#include <vector>
#include <thread>

#include <OpenAL/al.h>
#include <OpenAL/alc.h>

namespace audio_visualizer
{
    class AudioInput
    {
    public:
        AudioInput();
        ~AudioInput();

        void update();

        const std::vector<int16_t> &getAudioData();

    private:
        static const unsigned int SAMPLE_RATE, BUFFER_SIZE;

        int nextSample, numSamples;

        ALCdevice *device = nullptr;
        std::vector<int16_t> bufferData;
    };
}

#endif