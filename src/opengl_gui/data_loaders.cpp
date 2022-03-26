#include "../../header/opengl_gui/data_loaders.hpp"

std::string opengl_gui::textFileData(std::string filePath)
{
    std::string data;
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open())
        throw std::invalid_argument("opengl_gui::textFileData Failed to load file \"" + filePath + '\"');

    file.seekg(0, std::ios::end);
    data.resize(file.tellg());
    file.seekg(0, std::ios::beg);
    file.read(&data[0], data.size());
    file.close();

    return data;
}

opengl_gui::TextureData::TextureData(std::string filePath)
{
    int nColorChannels;
    bytes = stbi_load(filePath.c_str(), &width, &height, &nColorChannels, 0);
    if (width < 1 || height < 1 || nColorChannels < 1)
        throw std::invalid_argument("opengl_gui::TextureData Failed to load image \"" + filePath + '\"');

    format = GL_RGBA;
    if (nColorChannels == 1)
        format = GL_RED;
    else if (nColorChannels == 2)
        format = GL_RG;
    else if (nColorChannels == 3)
        format = GL_RGB;
}
opengl_gui::TextureData::~TextureData()
{
    delete bytes;
}