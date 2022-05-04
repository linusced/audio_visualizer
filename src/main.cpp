#include "../header/audio_visualizer/audio_visualizer.hpp"
#include "../header/files.hpp"
#include "../header/console_input_thread.hpp"

int main()
{
    opengl_gui::init();
    opengl_gui::Window window(-1, -1, 1280, 720, "Audio Visualizer");

    std::string cssCode = opengl_gui::textFileData(filePath("style.css"));

    bool stop = false;

    std::vector<std::string> imagePaths;
    auto imageFilesIterator = std::filesystem::directory_iterator(filePath("images"));
    for (auto &i : imageFilesIterator)
        if (i.path().extension() == ".jpg" || i.path().extension() == ".png")
            imagePaths.push_back(i.path());

    std::sort(imagePaths.begin(), imagePaths.end());

    std::vector<std::string> lyricsPaths;
    auto lyricsFileIterator = std::filesystem::directory_iterator(filePath("lyrics"));
    for (auto &i : lyricsFileIterator)
        if (i.path().extension() == ".dat")
            lyricsPaths.push_back(i.path());

    audio_visualizer::App app(&stop, cssCode, filePath("font.ttf"), imagePaths, lyricsPaths, &window);

    std::thread consoleInputThread(consoleInputThreadFunc, &stop, &app);

    std::cout << "\033[1;32mProgram Successfully Initiated!\033[0m\n";
    app.loop();
    std::cout << "\033[1;32mTerminating Program!\033[0m\n";

    if (stop)
        consoleInputThread.join();

    app.terminate();

    window.terminate();
    opengl_gui::terminate();
    return 0; // if (!stop) consoleInputThread will be terminated and may cause memory leaks... Always close program using console command
}