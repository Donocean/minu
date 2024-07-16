#include "minu_anim_easing.h"

#include <math.h>

#define PI  3.1415926f
#define _c1 1.70158
#define _c2 1.70158 * 1.525
#define _c3 1.70158 + 1
#define _c4 6.2831853 / 3
#define _c5 6.2831853 / 4.5
#define _n1 7.5625
#define _d1 2.75

int linear(const int t)
{
    return t;
}

int easeInQuad(const int t)
{
    float ft = t;
    ft = ft / maxT;
    ft = ft * ft;
    return (int)(ft * maxT);
}

int easeOutQuad(const int t)
{
    float ft = t;
    ft = ft / maxT;
    ft = 1 - (1 - ft) * (1 - ft);
    return (int)(ft * maxT);
}

int easeInOutQuad(const int t)
{
    float ft = t;
    ft = ft / maxT;
    if (t < maxT / 2)
        ft = 2 * ft * ft;
    else
        ft = 1 - pow(-2 * ft + 2, 3) / 2;
    return (int)(ft * maxT);
}

int easeInCubic(const int t)
{
    float ft = t;
    ft = ft / maxT;
    ft = ft * ft * ft;
    return (int)(ft * maxT);
}

int easeOutCubic(const int t)
{
    float ft = t;
    ft = ft / maxT;
    ft = 1 - pow(1 - ft, 3);
    return (int)(ft * maxT);
}

int easeInOutCubic(const int t)
{
    float ft = t;
    ft = ft / maxT;
    if (t < maxT / 2)
        ft = 4 * ft * ft * ft;
    else
        ft = 1 - pow(-2 * ft + 2, 3) / 2;
    return (int)(ft * maxT);
}

int easeInQuart(const int t)
{
    float ft = t;
    ft = ft / maxT;
    ft = ft * ft * ft * ft;
    return (int)(ft * maxT);
}

int easeOutQuart(const int t)
{
    float ft = t;
    ft = ft / maxT;
    ft = 1 - pow(1 - ft, 4);
    return (int)(ft * maxT);
}

int easeInOutQuart(const int t)
{
    float ft = t;
    ft = ft / maxT;
    if (t < maxT / 2)
        ft = 8 * ft * ft * ft * ft;
    else
        ft = 1 - pow(-2 * ft + 2, 4) / 2;
    return (int)(ft * maxT);
}

int easeInQuint(const int t)
{
    float ft = t;
    ft = ft / maxT;
    ft = ft * ft * ft * ft * ft;
    return (int)(ft * maxT);
}

int easeOutQuint(const int t)
{
    float ft = t;
    ft = ft / maxT;
    ft = 1 - pow(1 - ft, 5);
    return (int)(ft * maxT);
}

int easeInOutQuint(const int t)
{
    float ft = t;
    ft = ft / maxT;
    if (t < maxT / 2)
        ft = 16 * ft * ft * ft * ft * ft;
    else
        ft = 1 - pow(-2 * ft + 2, 5) / 2;
    return (int)(ft * maxT);
}

int easeInSine(const int t)
{
    float ft = t;
    ft = ft / maxT;
    ft = 1 - cos((ft * PI) / 2);
    return (int)(ft * maxT);
}

int easeOutSine(const int t)
{
    float ft = t;
    ft = ft / maxT;
    ft = sin((ft * PI) / 2);
    return (int)(ft * maxT);
}

int easeInOutSine(const int t)
{
    float ft = t;
    ft = ft / maxT;
    ft = -(cos(ft * PI) - 1) / 2;
    return (int)(ft * maxT);
}

int easeInExpo(const int t)
{
    float ft = t;
    ft = ft / maxT;
    if (t == 0)
        return 0;
    ft = pow(2, 10 * ft - 10);
    return (int)(ft * maxT);
}

int easeOutExpo(const int t)
{
    float ft = t;
    ft = ft / maxT;
    if (t == maxT)
        return maxT;
    ft = 1 - pow(2, -10 * ft);
    return (int)(ft * maxT);
}

int easeInOutExpo(const int t)
{
    float ft = t;
    ft = ft / maxT;
    if (t == 0)
        return 0;
    if (t == maxT)
        return maxT;
    if (t < maxT / 2)
        ft = pow(2, 20 * ft - 10) / 2;
    else
        ft = (2 - pow(2, -20 * ft + 10)) / 2;
    return (int)(ft * maxT);
}

int easeInCirc(const int t)
{
    float ft = t;
    ft = ft / maxT;
    ft = 1 - sqrt(1 - pow(ft, 2));
    return (int)(ft * maxT);
}

int easeOutCirc(const int t)
{
    float ft = t;
    ft = ft / maxT;
    ft = sqrt(1 - pow(ft - 1, 2));
    return (int)(ft * maxT);
}

int easeInOutCirc(const int t)
{
    float ft = t;
    ft = ft / maxT;
    if (t < maxT / 2)
        ft = (1 - sqrt(1 - pow(2 * ft, 2))) / 2;
    else
        ft = (sqrt(1 - pow(-2 * ft + 2, 2)) + 1) / 2;
    return (int)(ft * maxT);
}

int easeInBack(const int t)
{
    float ft = t;
    ft = ft / maxT;
    ft = _c3 * ft * ft * ft - _c1 * ft * ft;
    return (int)(ft * maxT);
}

int easeOutBack(const int t)
{
    float ft = t;
    ft = ft / maxT;
    ft = 1 + _c3 * pow(ft - 1, 3) + _c1 * pow(ft - 1, 2);
    return (int)(ft * maxT);
}

int easeInOutBack(const int t)
{
    float ft = t;
    ft = ft / maxT;
    if (t < maxT / 2)
        ft = (pow(2 * ft, 2) * ((_c2 + 1) * 2 * ft - _c2)) / 2;
    else
        ft = (pow(2 * ft - 2, 2) * ((_c2 + 1) * (ft * 2 - 2) + _c2) + 2) / 2;
    return (int)(ft * maxT);
}

int easeInElastic(const int t)
{
    float ft = t;
    ft = ft / maxT;
    if (t == 0)
        return 0;
    if (t == maxT)
        return maxT;
    ft = -(pow(2, 10 * ft - 10)) * sin((ft * 10 - 10.75) * _c4);
    return (int)(ft * maxT);
}

int easeOutElastic(const int t)
{
    float ft = t;
    ft = ft / maxT;
    if (t == 0)
        return 0;
    if (t == maxT)
        return maxT;
    ft = pow(2, -10 * ft) * sin((ft * 10 - 0.75) * _c4) + 1;
    return (int)(ft * maxT);
}

int easeInOutElastic(const int t)
{
    float ft = t;
    ft = ft / maxT;
    if (t == 0)
        return 0;
    if (t == maxT)
        return maxT;
    if (t < maxT / 2)
        ft = -(pow(2, 20 * ft - 10) * sin((20 * ft - 11.125) * _c5)) / 2;
    else
        ft = (pow(2, -20 * ft + 10) * sin((20 * ft - 11.125) * _c5)) / 2 + 1;
    return (int)(ft * maxT);
}

int easeInBounce(const int t)
{
    return maxT - easeOutBounce(maxT - t);
}

int easeOutBounce(const int t)
{
    float ft = t;
    ft = ft / maxT;
    if (ft < 1 / _d1)
        ft = _n1 * ft * ft;
    else if (ft < 2 / _d1)
    {
        ft = ft - 1.5 / _d1;
        ft = _n1 * ft * ft + 0.75;
    }
    else if (ft < 2.5 / _d1)
    {
        ft = ft - 2.25 / _d1;
        ft = _n1 * ft * ft + 0.9375;
    }
    else
    {
        ft = ft - 2.625 / _d1;
        ft = _n1 * ft * ft + 0.984375;
    }
    return (int)(ft * maxT);
}

int easeInOutBounce(const int t)
{
    if (t < maxT / 2)
        return (maxT - easeOutBounce(maxT - 2 * t)) / 2;
    return (maxT + easeOutBounce(2 * t - maxT)) / 2;
}
