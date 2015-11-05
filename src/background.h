#include "color.h"

class background
{
public:
    Color color;
};

class solidColorBackground : public background
{
public:
    solidColorBackground(Color backgroundColor)
    {
        color = backgroundColor;
    }
};
