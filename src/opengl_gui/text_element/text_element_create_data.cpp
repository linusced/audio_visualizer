#include "../../../header/opengl_gui/basic_elements/text_element.hpp"

void opengl_gui::TextElement::thread_createData(glm::vec4 color, int fontSize, int maxWidth, glm::ivec2 windowSize, std::string text, bool isMultiline, Font *font, std::vector<unsigned char> *textBytes, int *textWidth, int *textHeight, std::vector<int> *charWidth, bool *completed)
{
    *completed = false;

    if (text.size() == 0)
        text.push_back(' ');

    std::vector<std::string> lines;
    if (isMultiline)
    {
        int lineCount = std::count(text.begin(), text.end(), '\n') + 1;
        for (int i = 0; i < lineCount; i++)
        {
            lines.push_back(text.substr(0, text.find('\n')));
            if (i + 1 < lineCount)
                text = text.substr(text.find('\n') + 1);
        }
    }
    else
        lines.push_back(text);

    stbtt_fontinfo info;
    stbtt_InitFont(&info, font->buffer, 0);

    int imgSizeMult = 2,
        lineHeight = (fontSize + 4) * imgSizeMult;
    float scale = stbtt_ScaleForPixelHeight(&info, fontSize * imgSizeMult);
    int x = 0, ascent, descent, lineGap, lineOffset = 0;
    stbtt_GetFontVMetrics(&info, &ascent, &descent, &lineGap);
    ascent = roundf(ascent * scale);
    descent = roundf(descent * scale);

    for (int l = 0; l < lines.size(); l++)
    {
        x = 0;
        text = lines[l];
        for (int i = 0; i < text.size(); i++)
        {
            int ax;
            stbtt_GetCodepointHMetrics(&info, text[i], &ax, (int *)0);
            x += roundf(ax * scale);
            int kern = stbtt_GetCodepointKernAdvance(&info, text[i], text[i + 1]);
            x += roundf(kern * scale);
            if (x >= (maxWidth * imgSizeMult) && isMultiline)
            {
                size_t split = text.substr(0, i + 1).find_last_of(' ');
                if (split == std::string::npos)
                    split = i + 1;

                if (split < text.size())
                {
                    lines[l] = text.substr(0, split);
                    lines.insert(lines.begin() + l + 1, text.substr(split + 1));
                }
                break;
            }
        }
    }

    *textHeight = lines.size() * lineHeight;
    *textWidth = 0;
    charWidth->resize(isMultiline ? 0 : lines[0].size());

    int prevX = 0;
    for (int l = 0; l < lines.size(); l++)
    {
        x = 0;
        text = lines[l];
        for (int i = 0; i < text.size(); i++)
        {
            int ax;
            stbtt_GetCodepointHMetrics(&info, text[i], &ax, (int *)0);
            x += roundf(ax * scale);
            int kern = stbtt_GetCodepointKernAdvance(&info, text[i], text[i + 1]);
            x += roundf(kern * scale);
            if (charWidth->size() > 0)
            {
                charWidth->at(i) = x - prevX;
                prevX = x;
            }
        }

        if (x > *textWidth)
            *textWidth = x;
    }

    std::vector<unsigned char> bitmap(*textWidth * *textHeight),
        rawTextData(bitmap.size() * 4);

    for (int l = 0; l < lines.size(); l++)
    {
        x = 0;
        text = lines[l];
        for (int i = 0; i < text.size(); i++)
        {
            int ax, lsb;
            stbtt_GetCodepointHMetrics(&info, text[i], &ax, &lsb);
            int c_x1, c_y1, c_x2, c_y2;
            stbtt_GetCodepointBitmapBox(&info, text[i], scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);
            int y = ascent + c_y1 + lineOffset, byteOffset = x + roundf(lsb * scale) + (y * *textWidth);
            stbtt_MakeCodepointBitmap(&info, bitmap.data() + byteOffset, c_x2 - c_x1, c_y2 - c_y1, *textWidth, scale, scale, text[i]);
            x += roundf(ax * scale);
            int kern = stbtt_GetCodepointKernAdvance(&info, text[i], text[i + 1]);
            x += roundf(kern * scale);
        }
        lineOffset += lineHeight;
    }
    for (unsigned long i = 0, j = 0; i < bitmap.size(); i++, j += 4)
    {
        rawTextData[j] = 255 * color.r;
        rawTextData[j + 1] = 255 * color.g;
        rawTextData[j + 2] = 255 * color.b;
        rawTextData[j + 3] = bitmap[i] > 127 ? 255 * color.a : 0;
    }

    textBytes->resize((*textWidth / imgSizeMult) * (*textHeight / imgSizeMult) * 4);

    stbir_resize_uint8(rawTextData.data(), *textWidth, *textHeight, 0, textBytes->data(), *textWidth / imgSizeMult, *textHeight / imgSizeMult, 0, 4);
    *textWidth /= imgSizeMult;
    *textHeight /= imgSizeMult;

    for (int i = 0; i < charWidth->size(); i++)
        charWidth->at(i) /= imgSizeMult;

    *completed = true;
}