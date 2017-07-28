#ifndef SETTINGS_H
#define SETTINGS_H

struct Settings
{
public:
    const int steps;
    const float lambda;
    const float epsilon;
public:
    Settings(int steps, float lambda, float epsilon)
        :steps(steps), lambda(lambda), epsilon(epsilon)
    {
    }
};

#endif // SETTINGS_H
