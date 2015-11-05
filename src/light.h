#include "vector.h"
#include "color.h"

class light
{
public:
    double lightIntensity;
    Color lightColor;
    Vector position;
};

class pointLight : public light
{
};
