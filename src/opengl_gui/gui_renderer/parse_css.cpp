#include "../../../header/opengl_gui/gui_renderer.hpp"

void opengl_gui::Renderer::parseCSS(std::string css)
{
    css.erase(std::remove_if(css.begin(), css.end(), ::isspace), css.end());

    size_t currentPos = 0, lineEnd;
    std::string line, key, value;

    while (css[currentPos] == '.' && currentPos < css.size())
    {
        lineEnd = css.find('{', currentPos);
        Style cssStyle;
        cssStyle.name = css.substr(++currentPos, lineEnd - currentPos);

        currentPos = lineEnd + 1;

        while (css[currentPos] != '}' && currentPos < css.size())
        {
            lineEnd = css.find(';', currentPos);
            if (lineEnd == std::string::npos)
                lineEnd = css.find('}', currentPos);

            line = css.substr(currentPos, lineEnd - currentPos);
            key = line.substr(0, line.find(':'));
            value = line.substr(line.find(':') + 1);

            bool keyExists = false;
            for (auto &sizePair : cssStyle.sizeProperties)
                if (key == sizePair.first)
                {
                    getStyleSize(sizePair.second, value);
                    keyExists = true;
                    break;
                }

            if (!keyExists)
                for (auto &colorPair : cssStyle.colorProperties)
                    if (key == colorPair.first)
                    {
                        getStyleColor(colorPair.second, value);
                        keyExists = true;
                        break;
                    }

            if (!keyExists)
                for (auto &miscPair : cssStyle.miscProperties)
                    if (key == miscPair.first)
                    {
                        if (value.size() > 10 && value.substr(value.size() - 10) == "!important")
                        {
                            value = value.substr(0, value.size() - 10);
                            miscPair.second.isImportant = true;
                        }
                        miscPair.second.value = value;
                        miscPair.second.isSet = true;

                        keyExists = true;
                        break;
                    }

            if (css[currentPos] != '}')
                currentPos = lineEnd + 1;
        }

        currentPos++;
        styles.push_back(cssStyle);
    }
}

void opengl_gui::Renderer::getStyleSize(Style::SIZE &output, std::string &str)
{
    if (str.size() > 10 && str.substr(str.size() - 10) == "!important")
    {
        str = str.substr(0, str.size() - 10);
        output.isImportant = true;
    }

    if (str.back() >= '0' && str.back() <= '9')
    {
        output.unit = Style::PIXELS;
        output.value = std::stof(str);
    }
    else if (str.back() == '%')
    {
        output.unit = Style::PERCENTAGE;
        output.value = std::stof(str.substr(0, str.size() - 1));
    }
    else
    {
        std::string unitStr = str.substr(str.size() - 2);
        if (unitStr == "px")
            output.unit = Style::PIXELS;
        else if (unitStr == "vw")
            output.unit = Style::VIEW_WIDTH;
        else if (unitStr == "vh")
            output.unit = Style::VIEW_HEIGHT;

        output.value = std::stof(str.substr(0, str.size() - 2));
    }
    output.isSet = true;
};

void opengl_gui::Renderer::getStyleColor(Style::COLOR &output, std::string &str)
{
    if (str.size() > 10 && str.substr(str.size() - 10) == "!important")
    {
        str = str.substr(0, str.size() - 10);
        output.isImportant = true;
    }

    if (str[0] == '#')
    {
        str = str.substr(1);

        if (str.size() == 6)
        {
            str += "ff";
        }
        else if (str.size() == 4)
        {
            str.insert(str.begin() + 1, str[0]);
            str.insert(str.begin() + 3, str[2]);
            str.insert(str.begin() + 5, str[4]);
            str.insert(str.begin() + 7, str[6]);
        }
        else if (str.size() == 3)
        {
            str.insert(str.begin() + 1, str[0]);
            str.insert(str.begin() + 3, str[2]);
            str.insert(str.begin() + 5, str[4]);
            str += "ff";
        }

        if (str.size() != 8)
        {
            output.isImportant = output.isSet = false;
            return;
        }

        for (int i = 0; i < 4; i++)
            output.value[i] = std::stoi(str.substr(i * 2, 2), (std::size_t *)0, 16) / 255.0f;

        output.isSet = true;
    }
    else if (str.substr(0, 3) == "rgb")
    {
        size_t currentPos = str.find('(') + 1, endPos = str.find(',', currentPos);
        output.value.r = std::stoi(str.substr(currentPos, endPos - currentPos)) / 255.0f;

        currentPos = endPos + 1;
        endPos = str.find(',', currentPos);
        output.value.g = std::stoi(str.substr(currentPos, endPos - currentPos)) / 255.0f;

        currentPos = endPos + 1;
        endPos = str.find(str.at(3) == 'a' ? ',' : ')', currentPos);
        output.value.b = std::stoi(str.substr(currentPos, endPos - currentPos)) / 255.0f;

        if (str.at(3) == 'a')
        {
            currentPos = endPos + 1;
            endPos = str.find(')', currentPos);
            output.value.a = std::stof(str.substr(currentPos, endPos - currentPos));
        }
        else
            output.value.a = 1.0f;

        output.isSet = true;
    }
};