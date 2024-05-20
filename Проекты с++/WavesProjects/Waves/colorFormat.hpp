#pragma once
#include <vector>

#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))

typedef struct rgb {
	unsigned char r, g, b;

	rgb(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0) : r(r), g(g), b(b) {}
} RGB;

typedef struct hsl {
	int h;
	float s, l;

	hsl(int h = 0, float s = 0, float l = 0) : h(h), s(s), l(l) {}
} HSL;

float hueToRgb(float v1, float v2, float vH) {
	if (vH < 0)
		vH += 1;

	if (vH > 1)
		vH -= 1;

	if ((6 * vH) < 1)
		return (v1 + (v2 - v1) * 6 * vH);

	if ((2 * vH) < 1)
		return v2;

	if ((3 * vH) < 2)
		return (v1 + (v2 - v1) * ((2.0f / 3) - vH) * 6);

	return v1;
}

RGB hslToRgb(int h, float s, float l) {
	unsigned char r = 0;
	unsigned char g = 0;
	unsigned char b = 0;

	if (s == 0)
	{
		r = g = b = (unsigned char)(l * 255);
	}
	else
	{
		float v1, v2;
		float hue = (float)h / 360;

		v2 = (l < 0.5) ? (l * (1 + s)) : ((l + s) - (l * s));
		v1 = 2 * l - v2;

		r = (unsigned char)(255 * hueToRgb(v1, v2, hue + (1.0f / 3)));
		g = (unsigned char)(255 * hueToRgb(v1, v2, hue));
		b = (unsigned char)(255 * hueToRgb(v1, v2, hue - (1.0f / 3)));
	}

	return RGB(r, g, b);
}

RGB hslToRgb(HSL hsl_color)
{
	return hslToRgb(hsl_color.h, hsl_color.s, hsl_color.l);
}

HSL rgbToHsl(unsigned char _r, unsigned char _g, unsigned char _b)
{
	float h_f;
	int h;
	float s;
	float l;

	float r = static_cast<float>(_r);
	float g = static_cast<float>(_g);
	float b = static_cast<float>(_b);


	r /= 255.f;
	g /= 255.f;
	b /= 255.f;

	float max = MAX(MAX(r, g), b);
	float min = MIN(MIN(r, g), b);

	h_f = s = l = (max + min) / 2;

	if (max == min) {
		h_f = s = 0; // achromatic
	}
	else {
		float d = max - min;
		s = (l > 0.5) ? d / (2 - max - min) : d / (max + min);

		if (max == r) {
			h_f = (g - b) / d + (g < b ? 6 : 0);
		}
		else if (max == g) {
			h_f = (b - r) / d + 2;
		}
		else if (max == b) {
			h_f = (r - g) / d + 4;
		}

		h_f /= 6;
	}
	h = h_f * 360;

	return HSL(h, s, l);
}

HSL rgbToHsl(RGB rgb_color)
{
	return rgbToHsl(rgb_color.r, rgb_color.g, rgb_color.b);
}