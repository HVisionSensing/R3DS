#ifndef SETTINGS_H
#define SETTINGS_H

struct Settings
{
public:
    const int stepsCount;
    const float stepLen;
    const float stepLenAxis;
    const float stop;
public:
    Settings(int stepsCount, float stepLen, float stepLenAxis, float stop)
        :stepsCount(stepsCount), stepLen(stepLen), stepLenAxis(stepLenAxis), stop(stop)
    {
    }
};

#endif // SETTINGS_H
