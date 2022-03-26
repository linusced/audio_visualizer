#ifndef OPENGL_GUI_STYLE
#define OPENGL_GUI_STYLE

#include "common.hpp"

namespace opengl_gui
{
    struct Layout
    {
        int x = 0, y = 0, width = 0, height = 0;
    };

    class Style
    {
    public:
        enum SIZE_UNIT
        {
            PIXELS,
            PERCENTAGE,
            VIEW_WIDTH,
            VIEW_HEIGHT
        };
        struct SIZE
        {
            SIZE_UNIT unit;
            float value;
            bool isSet = false, isImportant = false;
        };
        struct COLOR
        {
            glm::vec4 value;
            bool isSet = false, isImportant = false;
        };
        struct MISC
        {
            std::string value;
            bool isSet = false, isImportant = false;
        };

        Style();

        std::string name;

        std::map<std::string, SIZE> sizeProperties;
        std::map<std::string, COLOR> colorProperties;
        std::map<std::string, MISC> miscProperties;

        void add(const Style &s, double elapsedTime, bool *isTransitionActive);

        Layout getLayout(const Layout *parentLayout, const glm::ivec2 *windowSize) const;

        int getPixelSize(const SIZE &size, const glm::ivec2 *parentSize, const glm::ivec2 *windowSize, int axis) const;
    };
}

#endif