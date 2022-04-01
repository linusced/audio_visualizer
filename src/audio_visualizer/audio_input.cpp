#include "../../header/audio_visualizer/audio_input.hpp"

const unsigned int audio_visualizer::AudioInput::SAMPLE_RATE = 44100,
                   audio_visualizer::AudioInput::BUFFER_SIZE = 2048;

audio_visualizer::AudioInput::AudioInput()
{
    bufferData.resize(BUFFER_SIZE);

    device = alcCaptureOpenDevice(nullptr, SAMPLE_RATE, AL_FORMAT_MONO16, BUFFER_SIZE);
    alcCaptureStart(device);

    nextSample = 0;
}

audio_visualizer::AudioInput::~AudioInput()
{
    alcCaptureStop(device);
    alcCaptureCloseDevice(device);
}

void audio_visualizer::AudioInput::update()
{
    alcGetIntegerv(device, ALC_CAPTURE_SAMPLES, 1, &numSamples);

    if (nextSample + numSamples > BUFFER_SIZE)
    {
        int prevNumSamples = numSamples;
        numSamples -= BUFFER_SIZE - nextSample;
        alcCaptureSamples(device, bufferData.data() + nextSample, numSamples);

        numSamples = prevNumSamples - numSamples;
        nextSample = 0;
    }

    alcCaptureSamples(device, bufferData.data() + nextSample, numSamples);

    nextSample += numSamples;
}

const std::vector<int16_t> &audio_visualizer::AudioInput::getAudioData()
{
    return bufferData;
}