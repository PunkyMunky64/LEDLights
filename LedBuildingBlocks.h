#pragma once
#include "TickLEDs.h"
#include "ShaderLEDs.h"
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
    int iter_i = 0;
    
    //active
    float elapsed;
    float location;

    //definitions
    virtual TickLEDs::LEDColor get_next() final override;
    virtual void reset_iter() final override;
    virtual void tick_function(float dt) override;
    LightPoint() = default;
    LightPoint(const LightPoint& other) = default;
    LightPoint(LightPoint&& other) = default;
};

class MovingSource : public LightPoint {
private: 
    float const_velocity;
    float ISQLaw_coeff;
    Colors::RGBu8 const_color;
public:
    virtual float velocity(float elapsed, int location); //LEDs per second
    virtual float intensity_distance(float dist); //Yes, signed
    virtual Colors::RGBu8 color(float elapsed, int location);
    virtual Colors::RGBu8 intensity(Colors::RGBu8, float multiplier);
    MovingSource(float velocity, Colors::RGBu8 color, float isqlaw_coeff, int left_max, int right_max, int leds);
};

class ConstantLight : public ShaderLEDs {
private:
    Colors::RGBu8 color;
public:
    explicit ConstantLight(Colors::RGBu8 color, u8* stream, int led_count) : ShaderLEDs(stream, led_count) {
        this->color = color;
    }
    virtual Colors::RGBu8 poll(float time_ms, float i) override;
};