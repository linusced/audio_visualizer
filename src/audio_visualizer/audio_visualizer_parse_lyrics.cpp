#include "../../header/audio_visualizer/audio_visualizer.hpp"

void audio_visualizer::App::parseLyrics(std::string lyricsFileStr)
{
    std::stringstream stream(lyricsFileStr);
    TrackLyrics *lyrics = new TrackLyrics();

    std::string lyricsName;
    std::getline(stream, lyricsName);

    try
    {
        std::string input;
        while (!stream.eof())
        {
            LyricsLine line;

            std::getline(stream, input);
            line.time = std::stod(input);

            std::getline(stream, input);
            line.str = input;

            lyrics->data.push_back(line);
        }
    }
    catch (...)
    {
        throw std::invalid_argument("Failed to parse lyrics file \"" + lyricsName + '\"');
    }

    trackLyricsMap[lyricsName] = lyrics;
}