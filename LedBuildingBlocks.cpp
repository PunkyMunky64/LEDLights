#include "LedBuildingBlocks.h"
#include <functional>
#include <iostream>
#include <algorithm>

std::vector<TickLEDs::LEDColor> LightPoint::get() {
    auto ret = std::vector<TickLEDs::LEDColor>();
    for (int i = location - left_max_radius; i <= location + right_max_radius; i++) {
        int j = i;
        if (j < 0) {
            j += led_count; //Only guaranteed to protect from one interval around
        }
        if (j >= led_count) {
            j -= led_count;
        }
        float dist = (float)i - location; //i not j to avoid rollover distance
        auto c = intensity(color(elapsed, j), intensity_distance(dist));
        ret.push_back(TickLEDs::LEDColor(j, c));
    }
    return ret;
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

/*LightPoint::LightPoint(std::function<float(float elapsed, int location)> velocity, std::function<float(float dist)> intensity_distance, std::function<Colors::RGBu8(float elapsed, int location)> color, std::function <Colors::RGBu8(Colors::RGBu8, float multiplier)> intensity, int left_max_radius, int right_max_radius, int initial_location, int led_count) {
    this->velocity = velocity;
    this->intensity_distance = intensity_distance;
    this->color = color;
    this->intensity = intensity;
    this->left_max_radius = left_max_radius;
    this->right_max_radius = right_max_radius;
    this->location = initial_location;
    this->led_count = led_count;
    this->elapsed = 0;
}*/

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
