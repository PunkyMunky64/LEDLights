//Todo:
//Optimizations

#define SQRT_CONSIDERABLE_PERCENTAGE 0.1
#define NOTHROW true
#include "LedBuildingBlocks.h"
#include <functional>
#include <iostream>
#include <algorithm>
#include <random>

int randrange(int a, int b) { //b is included
    return a + rand() % (b - a + 1);
}

float randrange(int a, int b, int precision_multiplier) {
    return a + rand() % ((b - a + 1) * precision_multiplier) / (float)precision_multiplier;
}

//--------------------------------------------------------------------------------------------------------------------------------//

Colors::RGBu8 randomHsvColor(u8 minh, u8 maxh, u8 s, u8 v) {
    u8 h = minh + rand() % (maxh - minh + 1);
    return Colors::HSVu8(h, s, v).to_rgb();
}

//--------------------------------------------------------------------------------------------------------------------------------//

float considerable_isqlaw_distance(float coeff) {
    return coeff / SQRT_CONSIDERABLE_PERCENTAGE;
}//Returns how far an ISQlaw will yield over CONSIDERABLE_PERCENTAGE% of the value at 1 unit away, for truncating



float considerable_isqlaw_distance(float coeff) {
    return coeff / SQRT_CONSIDERABLE_PERCENTAGE;
}//Returns how far an ISQlaw will yield over CONSIDERABLE_PERCENTAGE% of the value at 1 unit away, for truncating

TickLEDs::LEDColor LightPoint::get_next() {
    //a yield function which is equal to a loop like: for (int i = location - left_max_radius; i <= location + right_max_radius; i++) {

    int i = iter_i - left_max_radius + location;
    if (i > location + right_max_radius) return TickLEDs::LEDColor::Dead();

    //Normalize the range of i
    int i_ordinal = i;
    if (i_ordinal < 0) {
        i_ordinal += led_count; //Only guaranteed to protect from one interval around, we shouldn't spread left_max and right_max farther
    }
    if (i_ordinal >= led_count) {
        i_ordinal -= led_count;
    }

    float dist = (float)i - location; //i not j to avoid rollover distance
    auto c = intensity(color(), intensity_distance(dist));
    iter_i++;
    return TickLEDs::LEDColor(i_ordinal, c);
}

//--------------------------------------------------------------------------------------------------------------------------------//

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


//--------------------------------------------------------------------------------------------------------------------------------//


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
    if (left_max_radius >= led_count || right_max_radius >= led_count) {
        if (!NOTHROW) {
            throw "Left or right calculation cutoff radius on a LightPoint object cannot be greater than (or equal to) led count";
        } else {
            left_max_radius  = __min(led_count, left_max_radius -1);
            right_max_radius = __min(led_count, right_max_radius-1);
        }
    }
    //TODO find a better means of constructor delegation to handle this error
}


//--------------------------------------------------------------------------------------------------------------------------------//


Colors::RGBu8 ConstantLight::poll(float time_ms, int i) {
    return color;
}


//--------------------------------------------------------------------------------------------------------------------------------//


float MissleMovingSource::intensity_distance(float dist)
{
    if (location_raw + dist > location_end || location_raw + dist < location_start) return 0;
    int shift = dist < 0 ? redshift_ratio : 1.0;
    return __min(1.0, ISQLaw_coeff * shift  / (dist * dist));
}


//--------------------------------------------------------------------------------------------------------------------------------//


void MissleStarter::entity_handler(float dt)
{
    float rand01 = (float)(rand() % 10000) / 10000;
    if (rand01 < missleProbability(dt, elapsed)) {
        float isq = missleISQCoeff(elapsed);
        float redshift = missleRedshiftRatio(elapsed);
        float rightmax = considerable_isqlaw_distance(isq);
        float leftmax = considerable_isqlaw_distance(isq * redshift);
        add_entity(new MissleMovingSource(
            missleVelocity(elapsed), missleColor(elapsed), missleISQCoeff(elapsed), missleRedshiftRatio(elapsed), get_led_count(), leftmax, rightmax, get_led_count())
        );
    }
    return;
}


//--------------------------------------------------------------------------------------------------------------------------------//


float BellWave::velocity() {
    return v;
}

float BellWave::intensity_distance(float dist) {
    return h * intensity_coeff * (-dist * dist * h * h); //higher height, more concentrated h = 1/w
}

bool BellWave::kill_condition() {
    return height() < kill_threshold;
}

Colors::RGBu8 BellWave::color() {
    return Colors::RGBu8();
}

void BellWave::set_velocity(float v) {
    this->v = v;
}

void BellWave::alter_intensity_coeff(float factor) {
    this->intensity_coeff *= factor;
}

float BellWave::height(void) {
    return h * intensity_coeff;
}

Colors::RGBu8 BellWave::intensity(Colors::RGBu8, float multiplier) {
    return Colors::RGBu8(c.r * multiplier, c.g * multiplier, c.b * multiplier);
};

BellWave::BellWave(float vi, Colors::RGBu8 color, float intensity_coeff, float x, float hi, float vh, int left_max, int right_max, int leds, float kill_threshold, float split_threshold) {
    this->v = vi;
    this->c = color;
    this->intensity_coeff = intensity_coeff;
    this->x = x;
    this->h = hi;
    this->vh = vh;
    this->kill_threshold = kill_threshold;

    this->left_max_radius = left_max;
    this->right_max_radius = right_max;
    this->led_count = leds;
    if (left_max_radius >= led_count || right_max_radius >= led_count) {
        if (!NOTHROW) {
            throw "Left or right calculation cutoff radius on a LightPoint object cannot be greater than (or equal to) led count";
        } else {
            left_max_radius  = __min(led_count, left_max_radius -1);
            right_max_radius = __min(led_count, right_max_radius-1);
        }
    }    
}

void BellWave::tick_function(float dt) {
    LightPoint::tick_function(dt);
    h += vh;
}

void RandomBellWaves::entity_handler(float dt) {
    float rand01 = (float)(rand() % 10000) / 10000;
    if (rand01 < waveProbability(dt, elapsed)) {
        float rightmax = 100; //TODO Arbitrary right now
        float leftmax = 100;
        
        add_bellwave(
            new BellWave(
                waveVelocity(elapsed), waveColor(elapsed), 1.0, waveX(elapsed), waveHeight(elapsed), waveHeightVelocity(elapsed), leftmax, rightmax, get_led_count(), min_intensity_threshold, max_intensity_threshold
            )
        );        
    }
    for (int i = 0; i < wave_entities.size(); i++) {
        //TODO: pruning after many waves
        //Generated by copilot finish later
        if (auto wave = wave_entities[i].lock()) {
            if (wave->height() > max_intensity_threshold) {
                wave->alter_intensity_coeff(0.5);
                auto waveCopy = std::make_shared<BellWave>(*wave);
                float impulse = waveSplitVelocity(elapsed);
                wave->set_velocity(wave->velocity() + impulse);
                waveCopy->set_velocity(waveCopy->velocity() - impulse);
                add_bellwave(waveCopy.get());
            }
        }
    }
}

void RandomBellWaves::add_bellwave(BellWave *wave) {
    //Push back to the TickLEDs base class entity tracker, and then also save a copied weak pointer to the wave_entities tracker, which will be used to handle splitting the waves.
    //I suppose we sort of bypassed weak pointer conditions. //TODO: add weak pointer checks
    wave_entities.push_back(
        std::weak_ptr<BellWave>(
            std::dynamic_pointer_cast<BellWave>(
                add_entity(
                    wave
                ).lock()
            )
        )
    );
    //TODO: reimplement tickleds in randombellwaves? if class extension shenanigans dont work here
}


//--------------------------------------------------------------------------------------------------------------------------------//
