#include "../../../header/opengl_gui/gui_element.hpp"

bool opengl_gui::Element::inputs(glm::ivec2 relativeCursorPosition)
{
    if (window->isLmbDown())
    {
        if (!lmbDown)
        {
            lmbDown = requireInputs = true;
            lmbDownStart = relativeCursorPosition;
            lmbDownScrollMeshOffsetY = scrollMeshOffsetY;

            if (maxScrollY > 0 && lmbDownStart.x >= prevContainerLayout.width - scrollMeshWidth && (lmbDownStart.y < lmbDownScrollMeshOffsetY || lmbDownStart.y > lmbDownScrollMeshOffsetY + scrollMeshHeight))
            {
                scrollY = (relativeCursorPosition.y - scrollMeshHeight / 2) / (float)prevContainerLayout.height * (maxScrollY + prevContainerLayout.height);
                lmbDownScrollMeshOffsetY = -1;
                styleChange = true;
            }
        }
        else if (maxScrollY > 0 && lmbDownStart.x >= prevContainerLayout.width - scrollMeshWidth)
        {
            if (lmbDownScrollMeshOffsetY == -1)
                lmbDownScrollMeshOffsetY = scrollMeshOffsetY;

            int newScrollY = ((relativeCursorPosition.y - lmbDownStart.y) + lmbDownScrollMeshOffsetY) / (float)prevContainerLayout.height * (maxScrollY + prevContainerLayout.height);

            if (newScrollY != scrollY)
                scrollY = newScrollY;

            styleChange = true;

            window->setCursor(GLFW_HAND_CURSOR);
            return true;
        }
    }
    else if (lmbDown)
    {
        lmbDown = requireInputs = false;
        styleChange = maxScrollY > 0;
    }

    if (!lmbDown && maxScrollY > 0)
    {
        double mouseScrollY = window->getScrollY();
        if (mouseScrollY != 0.0)
        {
            scrollY -= mouseScrollY * 33;
            styleChange = true;
            return true;
        }
    }

    if (maxScrollY > 0 && relativeCursorPosition.x >= prevContainerLayout.width - scrollMeshWidth)
        window->setCursor(GLFW_HAND_CURSOR);

    return false;
}