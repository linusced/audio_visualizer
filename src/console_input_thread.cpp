#include "../header/console_input_thread.hpp"

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
                if ((multiplier[0] < '0' || multiplier[0] > '9') && multiplier[0] != '.')
                    multiplier.erase(multiplier.begin());

                float fMultiplier = std::stof(multiplier);
                app->setMultiplier(fMultiplier);
            }
            else if (consoleInput[0] == 'i')
            {
                std::string index = consoleInput.substr(1);
                if (index[0] < '0' || index[0] > '9')
                    index.erase(index.begin());

                int iIndex = std::stoi(index) - 1;
                app->setImage(iIndex);
            }
            else if (consoleInput.substr(0, 2) == "t-")
            {
                if (consoleInput.size() == 2)
                    app->setTimer(0);
                else
                {
                    std::string time = consoleInput.substr(2);
                    if ((time[0] < '0' || time[0] > '9') && time[0] != '.')
                        time.erase(time.begin());

                    double dTime = std::stod(time);
                    app->setTimer(dTime);
                }
            }
            else if (consoleInput[0] == 't')
            {
                app->setText(consoleInput.substr(1));
            }
            else if (consoleInput[0] == 'b')
            {
                std::string intensity = consoleInput.substr(1);
                if ((intensity[0] < '0' || intensity[0] > '9') && intensity[0] != '.')
                    intensity.erase(intensity.begin());

                float fIntensity = std::stof(intensity);
                app->setBeatIntensity(fIntensity);
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