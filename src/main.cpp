#include "../header/audio_visualizer/audio_visualizer.hpp"
#include "../header/files.hpp"

void consoleInputThreadFunc(bool *stop, audio_visualizer::App *app)
{
    std::string consoleInput;

    while (!*stop)
    {
        std::getline(std::cin, consoleInput);

        try
        {
            if (consoleInput == "q" || consoleInput == "c")
            {
                *stop = true;
            }
            else if (consoleInput.substr(0, 3) == "hsv")
            {
                std::string hsvStr = consoleInput.substr(3);
                glm::vec3 hsv;

                if (hsvStr[0] < '0' || hsvStr[0] > '9')
                    hsvStr.erase(hsvStr.begin());

                std::string numChars = "0123456789.";
                size_t begin = 0, size = -1;
                for (int i = 0; i < 3; i++)
                {
                    begin += size + 1;
                    begin = hsvStr.find_first_of(numChars, begin);
                    size = hsvStr.find_first_not_of(numChars, begin);
                    if (size == std::string::npos && i < 2)
                        throw "";
                    size -= begin;

                    hsv[i] = std::stof(hsvStr.substr(begin, size));
                }

                app->setHSV(hsv);
            }
            else if (consoleInput[0] == 'm')
            {
                std::string multiplier = consoleInput.substr(1);
                if (multiplier[0] < '0' || multiplier[0] > '9')
                    multiplier.erase(multiplier.begin());

                float fMultiplier = std::stof(multiplier);
                app->setMultiplier(fMultiplier);
            }
            else if (consoleInput[0] == 'i')
            {
                std::string index = consoleInput.substr(1);
                if (index[0] < '0' || index[0] > '9')
                    index.erase(index.begin());

                int iIndex = std::stof(index) - 1;
                app->setImage(iIndex);
            }
            else
                throw "";
        }
        catch (...)
        {
            std::cout << "\033[1;31mInvalid Input\033[0m\n";
            continue;
        }
    }
}

int main()
{
    opengl_gui::init();

    /* if (opengl_gui::Window::getContentScale() == 1)
        glfwWindowHint(GLFW_SAMPLES, 8); */

    opengl_gui::Window window(-1, -1, 1280, 720, "Audio Visualizer");

    std::string cssCode = opengl_gui::textFileData(filePath("style.css"));

    bool stop = false;

    std::vector<std::string> imagePaths;
    auto imageFilesIterator = std::filesystem::directory_iterator(filePath("images"));
    for (auto &i : imageFilesIterator)
        if (i.path().extension() == ".jpg" || i.path().extension() == ".png")
            imagePaths.push_back(i.path());

    std::sort(imagePaths.begin(), imagePaths.end());

    audio_visualizer::App app(&stop, cssCode, filePath("Neonballroom.ttf"), imagePaths, &window);

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