#pragma once

#include "primitives.h"

struct Box
{
    primitives::space_t xmin {0};
    primitives::space_t xmax {0};
    primitives::space_t ymin {0};
    primitives::space_t ymax {0};

    bool touches(const Box& other) const
    {
        bool too_high   {ymin > other.ymax};
        bool too_low    {ymax < other.ymin};
        bool left       {xmax < other.xmin};
        bool right      {xmin > other.xmax};
        bool outside {too_high or too_low or left or right};
        return not outside;
    }
};

