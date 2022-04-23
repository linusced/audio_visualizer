# audio_visualizer
A simple program which draws an audio waveform on the screen and makes the background image brightness change depending on how strong the lower frequencies are.
It uses the computers microphone to get an audio buffer. The background images, colors and text-content can be changed through Terminal (console) when running the app executable file.
It uses OpenGL for graphics (with a simple GUI library called [opengl_gui](https://github.com/linusced/opengl_gui/)), OpenAL for the microphone audio input and FFTW for Fast Furier Transform.

# Download
In the future I might add a way for users to download the .app file/directory (for mac only).
