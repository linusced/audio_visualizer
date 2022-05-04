#include "../../header/audio_visualizer/audio_visualizer.hpp"

void audio_visualizer::App::loadLyrics(std::vector<std::string> &lyricsFilePaths)
{
    for (auto &str : lyricsFilePaths)
    {
        std::ifstream file(str);
        if (file.is_open())
        {
            try
            {
                std::vector<Lyrics> _lyrics;

                std::string name;
                std::getline(file, name);

                std::string input;
                double dInput;
                do
                {
                    std::getline(file, input);
                    dInput = std::stod(input);

                    std::getline(file, input);

                    _lyrics.push_back({input, dInput});

                } while (!file.eof());

                trackLyrics[name] = _lyrics;
            }
            catch (...)
            {
                std::cout << "\033[1;31mFailed to load lyrics: \"" << str << "\"\033[0m\n";
                continue;
            }
        }
    }
}