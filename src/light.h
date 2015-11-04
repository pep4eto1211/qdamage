#include "vector.h"

class light
{
public:
    double lightIntensity;
    Vector position;
};

class pointLight : public light
{
};
