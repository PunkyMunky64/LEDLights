#include "AnimationLib.h"
#include <memory>
#include "LedBuildingBlocks.h"
#include "TickLEDs.h"


std::unique_ptr<TickLEDs> TickLEDs_Controller_Bases::missles001(u8* data, int leds, int mincms, int maxcms, int isq, int red, std::shared_ptr<Colors::ColorPath360> path, int mindeg, int maxdeg, int probabilitydivider) {
    std::function<float(float)> velocity = [mincms, maxcms](float){
        return randrange(mincms, maxcms, 10) / 10;
    };
    std::function<float(float)> isqLaw = [isq](float) {
        return 10.0;
    };
    std::function<float(float)> redshift = [red](float) {
        return 5.0;
    };
    std::function<Colors::RGBu8(float)> missleColor = [path, mindeg, maxdeg](float) {
        return path->to_rgb(randrange(mindeg, maxdeg, 10));
    };
    std::function<float(float, float)> missleProbability = [](float dt, float) {
        return dt / 500;
    };
return (std::unique_ptr<TickLEDs>)std::make_unique<MissleStarter>((u8*)data, leds, missleProbability, missleColor, isqLaw, redshift, velocity);
}


//--------------------------------------------------------------------------------------------------------------------------------//


std::unique_ptr<TickLEDs> TickLEDs_Controllers::missles001_001(u8* data, int leds) {
    return TickLEDs_Controller_Bases::missles001(data, leds, 3, 7, 10.0, 5.0, std::make_shared<Colors::HSVColorPath>(200, 255), 95*2, 138*2, 500);
}


//--------------------------------------------------------------------------------------------------------------------------------//


std::unique_ptr<ShaderLEDs> ShaderLEDs_Controller_Bases::rainbow001(u8* data, int leds, int ledspersecond, std::shared_ptr<Colors::ColorPath360> path) {
    return (std::unique_ptr<ShaderLEDs>) std::make_unique<MovingRainbow>((u8*)data, leds, ledspersecond, path);
}

std::unique_ptr<ShaderLEDs> ShaderLEDs_Controller_Bases::rainbow002(u8 *data, int leds, float alpha, float beta, float gamma, std::shared_ptr<Colors::ColorPath360> path) {
    return (std::unique_ptr<ShaderLEDs>) std::make_unique<EvolvingWheelRainbow>((u8*)data, leds, alpha, beta, gamma, path);
}

//--------------------------------------------------------------------------------------------------------------------------------//


std::unique_ptr<ShaderLEDs> ShaderLEDs_Controllers::rainbow001_001(u8* data, int leds) {
    return ShaderLEDs_Controller_Bases::rainbow001(data, leds, 100, std::make_shared<Colors::HSVColorPath>(128, 255));
}

std::unique_ptr<ShaderLEDs> ShaderLEDs_Controllers::rainbow001_002(u8* data, int leds) {
    return ShaderLEDs_Controller_Bases::rainbow001(data, leds, 100, std::make_shared<ArbitraryCosinePath>());
}

std::unique_ptr<ShaderLEDs> ShaderLEDs_Controllers::rainbow002_001(u8* data, int leds) {
    return ShaderLEDs_Controller_Bases::rainbow002(data, leds, 0.2, 0.2, 0.5, std::make_shared<Colors::HSVColorPath>(128, 255));
}