#pragma once
#include "TickLEDs.h"
class LightPoint : public TickLEDs::LEDEntity {
private:
    //consts
    std::function<float(float elapsed, int location)> velocity; //LEDs per second
    std::function<float(float dist)> intensity_distance; //Yes, signed
    std::function<Colors::RGBu8(float elapsed, int location)> color;
    std::function <Colors::RGBu8(Colors::RGBu8, float multiplier)> intensity;
    int left_max_radius; //These keep it optimized for smaller points to not have to evaluate all the leds
    int right_max_radius;
    int led_count;
    //active
    float elapsed;
    float location;

    //definitions
    virtual std::vector<TickLEDs::LEDColor> get();
    virtual void tick_function(float dt);
public:
    LightPoint(std::function<float(float elapsed, int location)> velocity, std::function<float(float dist)> intensity_distance, std::function<Colors::RGBu8(float elapsed, int location)> color, std::function<Colors::RGBu8(Colors::RGBu8, float multiplier)> intensity, int left_max_radius, int right_max_radius, int location, int led_count);
    LightPoint() = default;
    LightPoint(const LightPoint& other) = default;
    LightPoint(LightPoint&& other) = default;
};

