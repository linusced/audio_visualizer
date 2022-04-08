#include "../../header/audio_visualizer/audio_input.hpp"

const unsigned int audio_visualizer::AudioInput::SAMPLE_RATE = 44100,
                   audio_visualizer::AudioInput::BUFFER_SIZE = 4096;

audio_visualizer::AudioInput::AudioInput()
{
    bufferData.resize(BUFFER_SIZE);

    device = alcCaptureOpenDevice(nullptr, SAMPLE_RATE, AL_FORMAT_MONO16, BUFFER_SIZE);
}

audio_visualizer::AudioInput::~AudioInput()
{
    if (thread)
    {
        runThread = false;
        thread->join();
        delete thread;
    }

    alcCaptureStop(device);
    alcCaptureCloseDevice(device);
}

void audio_visualizer::AudioInput::start()
{
    alcCaptureStart(device);

    runThread = true;
    thread = new std::thread(threadFunc, device, &bufferData, &multiplier);
}

void audio_visualizer::AudioInput::setMultiplier(float _newMultiplier)
{
    multiplier = _newMultiplier;
}
float audio_visualizer::AudioInput::getMultiplier()
{
    return multiplier;
}

bool audio_visualizer::AudioInput::runThread = true;

void audio_visualizer::AudioInput::threadFunc(ALCdevice *device, std::vector<double> *bufferData, float *multiplier)
{
    const double maxValue = pow(2, 16) / 2.0 - 1.0;
    int numSamples;
    std::vector<int16_t> inputBufferData(BUFFER_SIZE);
    std::vector<double> inputBufferDataDouble(BUFFER_SIZE);
    int i;

    while (runThread)
    {
        alcGetIntegerv(device, ALC_CAPTURE_SAMPLES, 1, &numSamples);
        if (numSamples != 0)
        {
            alcCaptureSamples(device, inputBufferData.data(), numSamples);

            if (numSamples == BUFFER_SIZE)
            {
                for (i = 0; i < numSamples; i++)
                    bufferData->at(i) = (inputBufferData[i] * *multiplier) / maxValue;
            }
            else
            {
                for (i = 0; i < numSamples; i++)
                    inputBufferDataDouble[i] = (inputBufferData[i] * *multiplier) / maxValue;

                bufferData->erase(bufferData->begin(), bufferData->begin() + numSamples);
                bufferData->insert(bufferData->end(), inputBufferDataDouble.begin(), inputBufferDataDouble.begin() + numSamples);
            }
        }
    }
}