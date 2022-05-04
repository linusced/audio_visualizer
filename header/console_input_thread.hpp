#ifndef CONSOLE_INPUT_THREAD
#define CONSOLE_INPUT_THREAD

#include "audio_visualizer/audio_visualizer.hpp"

void consoleInputThreadFunc(bool *stop, audio_visualizer::App *app);

#endif