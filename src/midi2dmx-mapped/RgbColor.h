#include <stdint.h>

struct RgbColor
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

RgbColor hsl2rgb(uint8_t hue, uint8_t saturation, uint8_t lightness)
{
    RgbColor rgb;

    if (saturation == 0)
    {
        rgb.r = lightness;
        rgb.g = lightness;
        rgb.b = lightness;

        return rgb;
    }

    uint16_t h = hue;
    uint16_t s = saturation;
    uint16_t v = lightness;
    uint8_t  a = h / 43;
    uint16_t m = (h - (a * 43)) * 6;
    uint8_t  p = (v * (255 - s)) >> 8;
    uint8_t  q = (v * (255 - ((s * m) >> 8))) >> 8;
    uint8_t  t = (v * (255 - ((s * (255 - m)) >> 8))) >> 8;

    switch (a)
    {
        case 0:
            rgb.r = v;
            rgb.g = t;
            rgb.b = p;
            break;
        case 1:
            rgb.r = q;
            rgb.g = v;
            rgb.b = p;
            break;
        case 2:
            rgb.r = p;
            rgb.g = v;
            rgb.b = t;
            break;
        case 3:
            rgb.r = p;
            rgb.g = q;
            rgb.b = v;
            break;
        case 4:
            rgb.r = t;
            rgb.g = p;
            rgb.b = v;
            break;
        default:
            rgb.r = v;
            rgb.g = p;
            rgb.b = q;
            break;
    }

    return rgb;
}
