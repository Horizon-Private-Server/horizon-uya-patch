#include "interop.h"
#include "random.h"

int rand(int max)
{
    return (int)randSeed() % max;
}

int randRangeInt(int min, int max)
{
    return (int)randSeed() % ((max - min) + 1) + min;
}

float randRangeFloat(float min, float max)
{
    return min + (float)((int)randSeed()) * (max - min) * 3.0517578e-05;
}

float randRangeFloatNeg(float min, float max)
{
    int r = randSeed();
    float f = min + (float)r * (max - min) * 3.0517578e-05;
    if ((r & 1) != 0) f = -f;

    return f;
}

float randRot(void)
{
    return (float)((int)randSeed() + -0x4000) * MATH_PI * 6.1035156e-05;
}

void randVectorRange(VECTOR output, float min, float max)
{
    float radius = (float)randRangeFloat(min, max);
    float elevation = randRot();
    float angle = randRot();

    float elevation_cos = (float)FastCos(elevation);
    float angle_sin = (float)FastSin(angle);

    output[0] = elevation_cos * angle_sin * radius;
    elevation = (float)FastSin(elevation);
    elevation_cos = (float)FastSin(angle);
    output[1] = (int)(elevation * elevation_cos * radius);
    elevation = (float)FastCos(angle);
    output[2] = (int)(elevation * radius);
}

void randSphere(VECTOR output, float min, float max)
{
    float angle = randRot();
    float elevation = randRot();
    float radius = randRangeFloat(min,max);
    vector_sphere(output, radius, angle, elevation);
    return;
}
