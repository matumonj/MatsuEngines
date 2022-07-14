
#pragma once
#include<math.h>
#define PI 3.14
	namespace Easing {
		inline float EaseIn(int t) {
			return t * t;
		}
		inline float EaseOut(float t, float start, float end) {
			return (end - start) * (1 - cosf(t * PI / 2)) + start;
		}
		inline float EaseInOut(int& t);
		inline float EaseOutIn(int& t);
	}


