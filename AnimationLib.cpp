#include "AnimationLib.h"
#include <memory>
#include "LedBuildingBlocks.h"
#include "TickLEDs.h"

std::shared_ptr<TickLEDs> TickLEDs_Controllers::missles001_001(u8 data[], int leds) {
    std::function<float(float)> velocity = [](float){
        return randrange(3, 7, 10) / 10;
    };
    std::function<float(float)> isqLaw = [](float) {
        return 10.0;
    };
    std::function<float(float)> redshift = [](float) {
        return 5.0;
    };
    std::function<Colors::RGBu8(float)> missleColor = [](float) {
        return randomHsvColor(95, 138, 200, 255);
    };
    std::function<float(float, float)> missleProbability = [](float dt, float) {
        return dt / 500;
    };
return (std::shared_ptr<TickLEDs>)std::make_shared<MissleStarter>((u8*)&data, leds, missleProbability, missleColor, isqLaw, redshift, velocity);
}

std::shared_ptr<ShaderLEDs> ShaderLEDs_Controllers::rainbow001_001(u8 data[], int leds) {
    return (std::shared_ptr<ShaderLEDs>) std::make_shared<MovingRainbow>((u8*)&data, leds, 100);
}