#ifndef __MINU_ANIM_EASING_H_
#define __MINU_ANIM_EASING_H_

#ifdef __cplusplus
extern "C" {
#endif

// t: 0 ~ maxT
#define maxT 1000

typedef int (*easingPath)(const int t);

// NOTE: all functions t: 0 ~ maxT
int linear(const int t);

int easeInQuad(const int t);
int easeOutQuad(const int t);
int easeInOutQuad(const int t);

int easeInCubic(const int t);
int easeOutCubic(const int t);
int easeInOutCubic(const int t);

int easeInQuart(const int t);
int easeOutQuart(const int t);
int easeInOutQuart(const int t);

int easeInQuint(const int t);
int easeOutQuint(const int t);
int easeInOutQuint(const int t);

int easeInSine(const int t);
int easeOutSine(const int t);
int easeInOutSine(const int t);

int easeInExpo(const int t);
int easeOutExpo(const int t);
int easeInOutExpo(const int t);

int easeInCirc(const int t);
int easeOutCirc(const int t);
int easeInOutCirc(const int t);

int easeInBack(const int t);
int easeOutBack(const int t);
int easeInOutBack(const int t);

int easeInElastic(const int t);
int easeOutElastic(const int t);
int easeInOutElastic(const int t);

int easeInBounce(const int t);
int easeOutBounce(const int t);
int easeInOutBounce(const int t);

#ifdef __cplusplus
}
#endif

#endif /* __MINU_ANIM_EASING_H_ */
