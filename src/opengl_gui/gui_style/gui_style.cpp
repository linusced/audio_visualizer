#include "../../../header/opengl_gui/gui_style.hpp"

opengl_gui::Style::Style()
{
    sizeProperties = {
        {"width", SIZE()},
        {"min-width", SIZE()},
        {"max-width", SIZE()},
        {"height", SIZE()},
        {"min-height", SIZE()},
        {"max-height", SIZE()},
        {"top", SIZE()},
        {"bottom", SIZE()},
        {"left", SIZE()},
        {"right", SIZE()},
        {"border-width", SIZE()},
        {"border-radius", SIZE()},
        {"padding-top", SIZE()},
        {"padding-bottom", SIZE()},
        {"padding-left", SIZE()},
        {"padding-right", SIZE()},
        {"font-size", SIZE()},
        {"scale", SIZE()},
        {"translate-x", SIZE()},
        {"translate-y", SIZE()},
    };

    colorProperties = {
        {"color", COLOR()},
        {"background-color", COLOR()},
        {"border-color", COLOR()},
    };

    miscProperties = {
        {"overflow", MISC()},
        {"visibility", MISC()},
        {"transition", MISC()},
        {"text-align", MISC()},
    };
}

void opengl_gui::Style::add(const Style &s, double elapsedTime, bool *isTransitionActive)
{
    std::string transition;
    glm::vec4 transitionColorValue;
    float transitionSizeValue;

    if (isTransitionActive != nullptr && miscProperties.at("transition").isSet)
    {
        transition = miscProperties.at("transition").value;

        size_t numberBegin = transition.find_first_of("0123456789."), numberEnd = transition.find_first_not_of("0123456789.", numberBegin);
        if (numberBegin != std::string::npos)
        {
            if (numberEnd == std::string::npos)
                numberEnd = transition.size();

            double transitionDuration = std::stod(transition.substr(numberBegin, numberEnd - numberBegin));

            if (numberEnd < transition.size() && transition.substr(numberEnd) == "ms")
                transitionDuration /= 1000.0;

            transition = transition.substr(0, numberBegin);

            auto sizeIterator = sizeProperties.find(transition);
            if (sizeIterator != sizeProperties.end())
            {
                if (elapsedTime < transitionDuration)
                {
                    *isTransitionActive = true;
                    transitionSizeValue = glm::mix(sizeProperties.at(transition).value, s.sizeProperties.at(transition).value, elapsedTime / transitionDuration);
                }
                else
                    transitionSizeValue = s.sizeProperties.at(transition).value;
            }

            auto colorIterator = colorProperties.find(transition);
            if (colorIterator != colorProperties.end())
            {
                if (elapsedTime < transitionDuration)
                {
                    *isTransitionActive = true;
                    transitionColorValue = glm::mix(colorProperties.at(transition).value, s.colorProperties.at(transition).value, elapsedTime / transitionDuration);
                }
                else
                    transitionColorValue = s.colorProperties.at(transition).value;
            }
        }
    }

    for (auto &sizePair : this->sizeProperties)
        if (!sizePair.second.isImportant && s.sizeProperties.at(sizePair.first).isSet)
        {
            if (sizePair.first == transition)
                sizePair.second.value = transitionSizeValue;
            else
                sizePair.second = s.sizeProperties.at(sizePair.first);
        }

    for (auto &colorPair : this->colorProperties)
        if (!colorPair.second.isImportant && s.colorProperties.at(colorPair.first).isSet)
        {
            if (colorPair.first == transition)
                colorPair.second.value = transitionColorValue;
            else
                colorPair.second = s.colorProperties.at(colorPair.first);
        }

    for (auto &miscPair : this->miscProperties)
        if (!miscPair.second.isImportant && s.miscProperties.at(miscPair.first).isSet)
            miscPair.second = s.miscProperties.at(miscPair.first);
}