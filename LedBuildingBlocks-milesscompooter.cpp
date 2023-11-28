//Todo:
//Optimizations

#define SQRT_CONSIDERABLE_PERCENTAGE 0.1

#include "LedBuildingBlocks.h"
#include <functional>
#include <iostream>
#include <algorithm>



float considerable_isqlaw_distance(float coeff) {
    return coeff / SQRT_CONSIDERABLE_PERCENTAGE;
}//Returns how far an ISQlaw will yield over CONSIDERABLE_PERCENTAGE% of the value at 1 unit away, for truncating

TickLEDs::LEDColor LightPoint::get_next() {

    //for (int i = location - left_max_radius; i <= location + right_max_radius; i++) {
    int i = iter_i - left_max_radius + location;
    if (i > location + right_max_radius) return TickLEDs::LEDColor::Dead();
    int j = i;
    if (j < 0) {
        j += led_count; //Only guaranteed to protect from one interval around, we shouldn't spread left_max and right_max farther
    }
    if (j >= led_count) {
        j -= led_count;
    }
    float dist = (float)i - location; //i not j to avoid rollover distance
    auto c = intensity(color(), intensity_distance(dist));
    iter_i++;
    return TickLEDs::LEDColor(j, c);
}

void LightPoint::reset_iter() {
    iter_i = 0;
}

void LightPoint::tick_function(float dt) {
    location_raw += dt * velocity(); //these shouldn't derail from each other, if they did its a problem
    location += dt * velocity();
    elapsed += dt;
    if (location < 0) {
        location += led_count;
    }
    if (location >= led_count) {
        location -= led_count;
    }
}

float MovingSource::velocity()
{
    return const_velocity;
}

float MovingSource::intensity_distance(float dist)
{
    if (location_raw + dist > location_end || location_raw + dist < location_start) return 0;
    return __min(1.0, ISQLaw_coeff / (dist * dist));
}

Colors::RGBu8 MovingSource::color()
{
    return const_color;
}

Colors::RGBu8 MovingSource::intensity(Colors::RGBu8 color, float multiplier)
{
    return Colors::RGBu8(color.r * multiplier, color.g * multiplier, color.b * multiplier);
}

bool MovingSource::kill_condition() {
    return this->location_raw > location_end + grace_distance; //doesnt actually use closure values here
}

MovingSource::MovingSource(float velocity, Colors::RGBu8 color, float isqlaw_coeff, float location_start, float location_end, float grace_distance, int left_max, int right_max, int leds) {
    this->const_velocity = velocity;
    this->const_color = color;
    this->ISQLaw_coeff = isqlaw_coeff;
    this->left_max_radius = left_max;
    this->right_max_radius = right_max;
    this->led_count = leds;
    this->location_start = location_start;
    this->location_end = location_end;
    this->grace_distance = grace_distance;
    this->location_raw = this->location = 0;
}

Colors::RGBu8 ConstantLight::poll(float time_ms, int i) {
    return color;
}

float MissleMovingSource::intensity_distance(float dist)
{
    return 0.0f;
}

void MissleStarter::entity_handler(float dt)
{
    if (missleProbability(dt, elapsed)) { //fix this line, test
        float isq = missleISQCoeff(elapsed);
        float redshift = missleRedshiftRatio(elapsed);
        float rightmax = considerable_isqlaw_distance(isq * redshift);
        float leftmax = considerable_isqlaw_distance(isq);
        add_entity(new MissleMovingSource(
            1.0, missleColor(elapsed), isq, missleRedshiftRatio(elapsed), get_led_count(), leftmax, rightmax, get_led_count())
        );
    }
    return;
}
