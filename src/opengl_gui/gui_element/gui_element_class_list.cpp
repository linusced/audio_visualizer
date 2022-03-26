#include "../../../header/opengl_gui/gui_element.hpp"

std::vector<std::string>::iterator opengl_gui::ClassList::begin()
{
    return strArray.begin();
}
std::vector<std::string>::iterator opengl_gui::ClassList::end()
{
    return strArray.end();
}
size_t opengl_gui::ClassList::size()
{
    return strArray.size();
}
std::string &opengl_gui::ClassList::operator[](size_t index)
{
    return strArray[index];
}
bool opengl_gui::ClassList::contains(std::string str)
{
    return std::find(begin(), end(), str) != end();
}

void opengl_gui::ClassList::remove(std::string str)
{
    auto it = std::find(begin(), end(), str);
    if (it != end())
        strArray.erase(it);
}
void opengl_gui::ClassList::add(std::string str)
{
    auto it = std::find(begin(), end(), str);
    if (it == end())
        strArray.push_back(str);
}
void opengl_gui::ClassList::clear()
{
    strArray.clear();
}

void opengl_gui::ClassList::operator=(std::string className)
{
    clear();

    size_t currentPos = 0, whitespacePos;
    do
    {
        whitespacePos = className.find(' ', currentPos);
        if (whitespacePos == std::string::npos)
            whitespacePos = className.size();

        add(className.substr(currentPos, whitespacePos - currentPos));

        currentPos = whitespacePos + 1;

    } while (currentPos < className.size());
}