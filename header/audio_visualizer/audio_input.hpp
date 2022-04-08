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
        static const unsigned int SAMPLE_RATE, OUTPUT_BUFFER_SIZE, INPUT_BUFFER_SIZE;

        AudioInput();
        ~AudioInput();

        void start();

        const std::vector<int16_t> &getAudioData();

        void setMultiplier(float _newMultiplier);
        float getMultiplier();

    private:
        static bool runThread;
        static void threadFunc(ALCdevice *device, std::vector<int16_t> *bufferData, float *multiplier);

        ALCdevice *device = nullptr;
        std::vector<int16_t> bufferData;

        float multiplier = 1.0f;
        std::thread *thread = nullptr;
    };
}

#endif