#include "../../header/audio_visualizer/audio_input.hpp"

const unsigned int audio_visualizer::AudioInput::SAMPLE_RATE = 44100,
                   audio_visualizer::AudioInput::OUTPUT_BUFFER_SIZE = 4096,
                   audio_visualizer::AudioInput::INPUT_BUFFER_SIZE = 512;

audio_visualizer::AudioInput::AudioInput()
{
    bufferData.resize(OUTPUT_BUFFER_SIZE);

    device = alcCaptureOpenDevice(nullptr, SAMPLE_RATE, AL_FORMAT_MONO16, INPUT_BUFFER_SIZE);
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

const std::vector<int16_t> &audio_visualizer::AudioInput::getAudioData()
{
    return bufferData;
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

void audio_visualizer::AudioInput::threadFunc(ALCdevice *device, std::vector<int16_t> *bufferData, float *multiplier)
{
    int numSamples;
    std::vector<int16_t> inputBufferData(INPUT_BUFFER_SIZE);

    while (runThread)
    {
        alcGetIntegerv(device, ALC_CAPTURE_SAMPLES, 1, &numSamples);
        if (numSamples != 0)
        {
            alcCaptureSamples(device, inputBufferData.data(), numSamples);

            for (auto &i : inputBufferData)
                i *= *multiplier;

            bufferData->erase(bufferData->begin(), bufferData->begin() + numSamples);
            bufferData->insert(bufferData->end(), inputBufferData.begin(), inputBufferData.begin() + numSamples);
        }
    }
}