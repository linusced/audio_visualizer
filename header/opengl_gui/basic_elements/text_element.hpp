#ifndef OPENGL_GUI_TEXT_ELEMENT
#define OPENGL_GUI_TEXT_ELEMENT

#include "../gui_element.hpp"

namespace opengl_gui
{
    struct Font
    {
        unsigned char *buffer = nullptr;

        Font(std::string filePath);
    };

    class TextElement : public Element
    {
    public:
        TextElement(Font *font, std::string text, std::string className, bool isMultiline = true);
        ~TextElement();

        void resizeMesh(const Style &style, const Layout &layout, const glm::ivec2 &windowSize) override;
        void update() override;

        std::string getText();
        void setText(std::string text);

        bool isMultiline;

    protected:
        static void thread_createData(glm::vec4 color, int fontSize, int maxWidth, glm::ivec2 windowSize, std::string text, bool isMultiline, Font *font, std::vector<unsigned char> *textBytes, int *textWidth, int *textHeight, std::vector<int> *charWidth, bool *completed);

        void updateCursorPos(int cursorPos);

        Font *font = nullptr;
        std::string text;
        Texture *textTexture = nullptr;

        std::vector<unsigned char> textBytes, textureBytes;
        std::vector<int> charWidth;
        int cursorPos = -1;

        int textWidth, textHeight;
        const GLenum textFormat = GL_RGBA;

        std::thread *thread = nullptr;
        bool createdTextData = false;

        int prevFontSize = 0;
        glm::vec4 prevColor = glm::vec4(0);
    };
}

#endif