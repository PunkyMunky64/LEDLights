//Todo:
//Death function of leds
//More librarying
//Optimizations
//Shader LEDs

#include "LedBuildingBlocks.h"
#include <functional>
#include <iostream>
#include <algorithm>

TickLEDs::LEDColor LightPoint::get_next() {

    //for (int i = location - left_max_radius; i <= location + right_max_radius; i++) {
    int i = iter_i - left_max_radius + location;
    if (i > location + right_max_radius) return TickLEDs::LEDColor::Dead();
    int j = i;
    if (j < 0) {
        j += led_count; //Only guaranteed to protect from one interval around
    }
    if (j >= led_count) {
        j -= led_count;
    }
    float dist = (float)i - location; //i not j to avoid rollover distance
    auto c = intensity(color(elapsed, j), intensity_distance(dist));
    iter_i++;
    return TickLEDs::LEDColor(j, c);
}

void LightPoint::reset_iter() {
    iter_i = 0;
}

void LightPoint::tick_function(float dt) {
    location += dt * velocity(elapsed, location);
    elapsed += dt;
    if (location < 0) {
        location += led_count;
    }
    if (location >= led_count) {
        location -= led_count;
    }
}

float MovingSource::velocity(float elapsed, int location)
{
    return const_velocity;
}

float MovingSource::intensity_distance(float dist)
{
    return __min(1.0, ISQLaw_coeff / (dist * dist));
}

Colors::RGBu8 MovingSource::color(float elapsed, int location)
{
    return const_color;
}

Colors::RGBu8 MovingSource::intensity(Colors::RGBu8 color, float multiplier)
{
    return Colors::RGBu8(color.r * multiplier, color.g * multiplier, color.b * multiplier);
}

MovingSource::MovingSource(float velocity, Colors::RGBu8 color, float isqlaw_coeff, int left_max, int right_max, int leds) {
    this->const_velocity = velocity;
    this->const_color = color;
    this->ISQLaw_coeff = isqlaw_coeff;
    this->left_max_radius = left_max;
    this->right_max_radius = right_max;
    this->led_count = leds;
    this->location = 0;
}

Colors::RGBu8 ConstantLight::poll(float time_ms, float i) {
    return color;
}