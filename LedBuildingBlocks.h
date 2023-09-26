#pragma once
#include "TickLEDs.h"
class LightPoint : public TickLEDs::LEDEntity {
public:
    //consts
    virtual float velocity(float elapsed, int location) = 0; //LEDs per second
    virtual float intensity_distance(float dist) = 0; //Yes, signed
    virtual Colors::RGBu8 color(float elapsed, int location) = 0;
    virtual Colors::RGBu8 intensity(Colors::RGBu8, float multiplier) = 0;
protected:
    int left_max_radius; //These keep it optimized for smaller points to not have to evaluate all the leds
    int right_max_radius;
    int led_count;
    
    //active
    float elapsed;
    float location;

    //definitions
    virtual std::vector<TickLEDs::LEDColor> get();
    virtual void tick_function(float dt);
/*public:
    LightPoint(std::function<float(float elapsed, int location)> velocity, std::function<float(float dist)> intensity_distance, std::function<Colors::RGBu8(float elapsed, int location)> color, std::function<Colors::RGBu8(Colors::RGBu8, float multiplier)> intensity, int left_max_radius, int right_max_radius, int location, int led_count);
    LightPoint() = default;
    LightPoint(const LightPoint& other) = default;
    LightPoint(LightPoint&& other) = default;*/
};

class MovingSource : public LightPoint {
private: 
    float const_velocity;
    Colors::RGBu8 const_color;
    float ISQLaw_coeff;
public:
    virtual float velocity(float elapsed, int location); //LEDs per second
    virtual float intensity_distance(float dist); //Yes, signed
    virtual Colors::RGBu8 color(float elapsed, int location);
    virtual Colors::RGBu8 intensity(Colors::RGBu8, float multiplier);
    MovingSource(float velocity, Colors::RGBu8 color, float isqlaw_coeff, int left_max, int right_max, int leds);
};