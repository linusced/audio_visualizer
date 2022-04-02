#include "../../header/audio_visualizer/audio_input.hpp"

const unsigned int audio_visualizer::AudioInput::SAMPLE_RATE = 44100,
                   audio_visualizer::AudioInput::BUFFER_SIZE = 2048;

audio_visualizer::AudioInput::AudioInput()
{
    bufferData.resize(BUFFER_SIZE);

    std::cout << "AudioInput()\n";
    device = alcCaptureOpenDevice(nullptr, SAMPLE_RATE, AL_FORMAT_MONO16, BUFFER_SIZE);
    alcCaptureStart(device);
    std::cout << "AudioInput() - complete\n";

    nextSample = 0;
}

audio_visualizer::AudioInput::~AudioInput()
{
    std::cout << "destruct AudioInput()\n";
    alcCaptureStop(device);
    alcCaptureCloseDevice(device);
    std::cout << "destruct AudioInput() - complete\n";
}

void audio_visualizer::AudioInput::update()
{
    alcGetIntegerv(device, ALC_CAPTURE_SAMPLES, 1, &numSamples);

    if (numSamples == 0)
        return;
    else if (numSamples == BUFFER_SIZE)
        nextSample = 0;

    if (nextSample + numSamples > BUFFER_SIZE)
    {
        int prevNumSamples = numSamples;
        numSamples -= BUFFER_SIZE - nextSample;
        alcCaptureSamples(device, bufferData.data() + nextSample, numSamples);

        for (int i = nextSample; i < nextSample + numSamples; i++)
            bufferData[i] *= multiplier;

        numSamples = prevNumSamples - numSamples;
        nextSample = 0;
    }

    alcCaptureSamples(device, bufferData.data() + nextSample, numSamples);

    for (int i = nextSample; i < nextSample + numSamples; i++)
        bufferData[i] *= multiplier;

    nextSample += numSamples;
}

const std::vector<int16_t> &audio_visualizer::AudioInput::getAudioData()
{
    return bufferData;
}

void audio_visualizer::AudioInput::setMultiplier(float _newMultiplier)
{
    multiplier = _newMultiplier;
}