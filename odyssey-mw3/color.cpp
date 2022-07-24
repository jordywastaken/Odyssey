#include "stdafx.h"

color color::set(int index, float col)
{
	color m_col = *this;
	*&m_col[index] = col;

	return m_col;
}

color color::set(int index, int col)
{
	color m_col = *this;
	*&m_col[index] = col / 255.f;

	return m_col;
}template<typename T> static inline void swap(T& a, T& b)
{
    T tmp = a; a = b; b = tmp;
}

// Dear ImGui
// Convert rgb floats ([0-1],[0-1],[0-1]) to hsv floats ([0-1],[0-1],[0-1]), from Foley & van Dam p592
// Optimized http://lolengine.net/blog/2013/01/13/fast-rgb-to-hsv
void rgb_to_hsv(float r, float g, float b, float& out_h, float& out_s, float& out_v)
{
    float K = 0.f;
    if(g < b)
    {
        swap(g, b);
        K = -1.f;
    }
    if(r < g)
    {
        swap(r, g);
        K = -2.f / 6.f - K;
    }

    const float chroma = r - (g < b ? g : b);
    out_h = fabsf(K + (g - b) / (6.f * chroma + 1e-20f));
    out_s = chroma / (r + 1e-20f);
    out_v = r;
}

// Convert hsv floats ([0-1],[0-1],[0-1]) to rgb floats ([0-1],[0-1],[0-1]), from Foley & van Dam p593
// also http://en.wikipedia.org/wiki/HSL_and_HSV
void hsv_to_rgb(float h, float s, float v, float& out_r, float& out_g, float& out_b)
{
    if(s == 0.0f)
    {
        // gray
        out_r = out_g = out_b = v;
        return;
    }

    h = fmodf(h, 1.0f) / (60.0f / 360.0f);
    int   i = (int)h;
    float f = h - (float)i;
    float p = v * (1.0f - s);
    float q = v * (1.0f - s * f);
    float t = v * (1.0f - s * (1.0f - f));

    switch(i)
    {
        case 0: out_r = v; out_g = t; out_b = p; break;
        case 1: out_r = q; out_g = v; out_b = p; break;
        case 2: out_r = p; out_g = v; out_b = t; break;
        case 3: out_r = p; out_g = q; out_b = v; break;
        case 4: out_r = t; out_g = p; out_b = v; break;
        case 5: default: out_r = v; out_g = p; out_b = q; break;
    }
}
