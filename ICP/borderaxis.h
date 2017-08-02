#ifndef BORDERAXIS_H
#define BORDERAXIS_H

struct BorderAxis
{
public:
    float max;
    float min;
    float len;

    BorderAxis(float maxInConstruct, float minInConstruct)
        :max(maxInConstruct), min(minInConstruct), len(maxInConstruct-minInConstruct)
    {
    }
};

#endif // BORDERAXIS_H
