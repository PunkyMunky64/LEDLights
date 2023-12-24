#pragma once
#include "TickLEDs.h"
#include "ShaderLEDs.h"
#include "blending.h"
#include "lib.h"
//#include <stdlib.h>

//Functions
//Consider moving this to a seperate module maybe?

int randrange(int a, int b);
float randrange(int a, int b, int precision_multiplier);
Colors::RGBu8 randomHsvColor(u8 minh, u8 maxh, u8 s, u8 v);


//--------------------------------------------------------------------------------------------------------------------------------//


class LightPoint : public TickLEDs::LEDEntity { //Abstract class
public:
    //consts
    virtual float velocity() = 0; //LEDs per second, virtual override can access protected members in this class
    virtual float intensity_distance(float dist) = 0; //Yes, signed
    virtual bool kill_condition() = 0;
    virtual Colors::RGBu8 color() = 0;
    virtual Colors::RGBu8 intensity(Colors::RGBu8, float multiplier) = 0;

protected: //Could construct these down but better for extension classes to handle it
    int left_max_radius; //These keep it optimized for smaller points to not have to evaluate all the leds
    int right_max_radius;
    int led_count;
    int iter_i = 0;
    
    //active
protected:
    float location_raw; //Not looped
    float location; //Need to stay in protected scope because these values dont have other ways of being acessed: tick_function override is now consumed. 
    float elapsed;

    //definitions
    virtual TickLEDs::LEDColor get_next() final override;
    virtual void reset_iter() final override;
    virtual void tick_function(float dt) override;
protected:
    LightPoint() = default;
public:
    LightPoint(const LightPoint& other) = default;
    LightPoint(LightPoint&& other) = default;
};


//--------------------------------------------------------------------------------------------------------------------------------//


class MovingSource : public LightPoint { 
private: 
protected:
    float const_velocity;
    float ISQLaw_coeff;
    float location_start;
    float location_end;
    float grace_distance;
    Colors::RGBu8 const_color;
public:
    virtual float velocity() override; //LEDs per second
    virtual float intensity_distance(float dist) override; //Yes, signed
    virtual bool kill_condition() override;
    virtual Colors::RGBu8 color() override;
    virtual Colors::RGBu8 intensity(Colors::RGBu8, float multiplier) override;
    MovingSource(float velocity, Colors::RGBu8 color, float isqlaw_coeff, float location_start, float location_end, float grace_distance, int left_max, int right_max, int leds);
    MovingSource(const MovingSource& other) = default;
    MovingSource(MovingSource&& other) = default;
};


//--------------------------------------------------------------------------------------------------------------------------------//


class MissleMovingSource : public MovingSource {
    float redshift_ratio = 1.0; //represents how much stronger ISQ law is on direction that the missle (should be, negative) moving away from; imprecise terminology
    //TODO: make redshift based on velocity and velocity a functional?
public:
    virtual float intensity_distance(float dist) override;
    explicit MissleMovingSource(float velocity, Colors::RGBu8 color, float isqlaw_coeff, int redshift_ratio, float grace_distance, int left_max, int right_max, int leds) : MovingSource(velocity, color, isqlaw_coeff, 0, leds, grace_distance, left_max, right_max, leds) {
        this->redshift_ratio = redshift_ratio;
    }
};


//--------------------------------------------------------------------------------------------------------------------------------//


class RayStarter : public TickLEDs {
    //TODO
    //Implement as a MissleStarter that keeps trails behind instead of dissapearing
};


//--------------------------------------------------------------------------------------------------------------------------------//


class ConstantLight : public ShaderLEDs {
private:
    Colors::RGBu8 color;
public:
    explicit ConstantLight(Colors::RGBu8 color, u8* stream, int led_count) : ShaderLEDs(stream, led_count) {
        this->color = color;
    }
    ConstantLight() = default;
    Colors::RGBu8 poll(float time_ms, int i) override;
};


//--------------------------------------------------------------------------------------------------------------------------------//


class MissleStarter : public TickLEDs {
public:
    /*
    Probability models can be derived from a poisson distribution.However,
    unlike a poisson distribution, the missle locations are rounded up to the locations at which the ticks
    were called. This also means that on outlier long ticks, only one missle can be fired, despite the 
    distribution calling for more. It also means that if there is a probability too high for the ticks,
    the granular approximation of the poisson distribution may visually cluster with the ticks, making
    a repeating pattern.
    */
    std::function<float(float dt, float elapsed)> missleProbability; 
    std::function<Colors::RGBu8(float elapsed)> missleColor;
    std::function<float(float elapsed)> missleISQCoeff;
    std::function<float(float elapsed)> missleRedshiftRatio;
    std::function<float(float elapsed)> missleVelocity;
    void entity_handler(float dt) override;
    explicit MissleStarter(u8* stream, int led_count, std::function<float(float dt, float elapsed)> missleProbability, std::function<Colors::RGBu8(float elapsed)> missleColor, std::function<float(float elapsed)> missleISQCoeff, std::function<float(float elapsed)> missleRedshiftRatio, std::function<float(float elapsed)> missleVelocity) : TickLEDs(stream, led_count, Blending::add) {
        this->missleProbability = missleProbability;
        this->missleISQCoeff = missleISQCoeff;
        this->missleColor = missleColor;
        this->missleRedshiftRatio = missleRedshiftRatio;
        this->missleVelocity = missleVelocity;
    }
};


//--------------------------------------------------------------------------------------------------------------------------------//


class ArbitraryCosinePath : public Colors::ColorPath360 {
public:
    virtual Colors::RGBu8 to_rgb(float theta) override {
        u8 colr = 255 * (0.5 + 0.5 * cos(rad(theta) + 0));
        u8 colg = 255 * (0.5 + 0.5 * cos(rad(theta) + 2 + 0.5));
        u8 colb = 255 * (0.5 + 0.5 * cos(rad(theta) + 4));
        
        return Colors::RGBu8(colr, colg, colb);
    }
};


//--------------------------------------------------------------------------------------------------------------------------------//


class MovingRainbow : public ShaderLEDs {
public:
    int leds_per_second;
    std::shared_ptr<Colors::ColorPath360> path;
    Colors::RGBu8 poll(float time_ms, int i) override {
        return path->to_rgb((int)(i * 358 / led_count + time_ms * leds_per_second / 1000) % 360);
    }
    explicit MovingRainbow(u8* stream, int led_count, int leds_per_second, std::shared_ptr<Colors::ColorPath360> path) : ShaderLEDs(stream, led_count) {
        this->leds_per_second = leds_per_second;
        this->path = path;
    }
};


//--------------------------------------------------------------------------------------------------------------------------------//


class EvolvingWheelRainbow : public ShaderLEDs {
public:
    float alpha; //dhsv dt
    float beta;  //dhsv dtheta
    float gamma; //dthetaoffset dt
    std::shared_ptr<Colors::ColorPath360> path;
    Colors::RGBu8 poll(float time_ms, int i) override {
        float time = time_ms / 1000.0;
        float timealpha_angle = mod2pi(time * alpha); //-pi to pi
        float timegamma_angle = mod2pi(time * gamma); //-pi to pi
        float theta = (float)i * 2.0 * PI / (float)led_count - PI;

        float thetaoffset = timegamma_angle;
        float reltheta = mod2pi(theta - thetaoffset);
        float flippedreltheta = reltheta > PI ? modnpipi(-reltheta) : reltheta;
        float sidetheta = flippedreltheta * beta;
        return path->to_rgb(degree(sidetheta + timealpha_angle));
    }
    explicit EvolvingWheelRainbow(u8* stream, int led_count, float alpha, float beta, float gamma, std::shared_ptr<Colors::ColorPath360> path) : ShaderLEDs(stream, led_count) {
        this->alpha = alpha;
        this->beta = beta;
        this->gamma = gamma;
        this->path = path;
    }
};


//--------------------------------------------------------------------------------------------------------------------------------//


class BellWave : public LightPoint {
public:
    float velocity() override;
    float intensity_distance(float dist) override;
    bool kill_condition() override;
    Colors::RGBu8 color() override;
    virtual Colors::RGBu8 intensity(Colors::RGBu8, float multiplier) override;

    float hvelocity();
    void set_velocity(float v);
    void set_hvelocity(float vh);
    void alter_intensity_coeff(float factor);
    float height(void);

    BellWave(float vi, Colors::RGBu8 color, float intensity_coeff, float sigma, float x, float hi, float vh, int left_max, int right_max, int leds, float kill_threshold, float split_threshold);

protected:
    virtual void tick_function(float dt) override;
    
private:
    float kill_threshold;
    float v;
    Colors::RGBu8 c;
    float intensity_coeff;
    float sigma;
    float h;
    float vh;
};


class RandomBellWaves : public TickLEDs {
public:
    std::function<float(float dt, float elapsed)> waveProbability;
    std::function<Colors::RGBu8(float elapsed)> waveColor;
    std::function<float(float elapsed)> waveVelocity;
    std::function<float(float elapsed)> waveHeightVelocity;
    std::function<float(float elapsed)> waveX;
    std::function<float(float elapsed)> waveSplitVelocity;
    float max_intensity_threshold;
    float min_intensity_threshold;
    float sigma;
    //std::function<float(float elapsed)> waveSplitThreshold; //TODO? each wave chooses an initial max threshold?
    //std::function<float(float elapsed)> waveSigma //TODO? each wave can have a different wwidth multiplier
    //std::function<float(float elapsed)> waveIntensityCoeff //TODO? each wave can have a different intensity coeff
private:
    std::vector<std::weak_ptr<BellWave>> wave_entities;
    void entity_handler(float dt) override;
    void add_bellwave(BellWave* wave);

public:
    explicit RandomBellWaves(u8* stream, int led_count, std::function<float(float dt, float elapsed)> waveProbability, std::function<Colors::RGBu8(float elapsed)> waveColor, std::function<float(float elapsed)> waveX, std::function<float(float elapsed)> waveVelocity, std::function<float(float elapsed)> waveHeightVelocity, float sigma, float min_intensity_threshold, float max_intensity_threshold) : TickLEDs(stream, led_count, Blending::add) {
        this->waveProbability = waveProbability;
        this->waveColor = waveColor;
        this->waveVelocity = waveVelocity;
        this->waveHeightVelocity = waveHeightVelocity;
        this->waveX = waveX;
        this->max_intensity_threshold = max_intensity_threshold;
        this->min_intensity_threshold = min_intensity_threshold;
        this->sigma = sigma;
    }
};