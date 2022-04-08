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
        static const unsigned int SAMPLE_RATE, BUFFER_SIZE;

        AudioInput();
        ~AudioInput();

        void start();

        void setMultiplier(float _newMultiplier);
        float getMultiplier();

        std::vector<double> bufferData;

    private:
        static bool runThread;
        static void threadFunc(ALCdevice *device, std::vector<double> *bufferData, float *multiplier);

        ALCdevice *device = nullptr;

        float multiplier = 1.0f;
        std::thread *thread = nullptr;
    };
}

#endif