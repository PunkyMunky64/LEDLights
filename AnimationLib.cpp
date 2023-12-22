#include "AnimationLib.h"
#include <memory>
#include "LedBuildingBlocks.h"
#include "TickLEDs.h"




std::unique_ptr<ShaderLEDs> ShaderLEDs_Controllers::rainbow001_001(u8* data, int leds) {
    return ShaderLEDs_Controller_Bases::rainbow001(data, leds, 100, std::make_shared<Colors::HSVColorPath>(128, 255));
}

std::unique_ptr<ShaderLEDs> ShaderLEDs_Controllers::rainbow001_002(u8* data, int leds) {
    return ShaderLEDs_Controller_Bases::rainbow001(data, leds, 100, std::make_shared<ArbitraryCosinePath>());
}

std::unique_ptr<ShaderLEDs> ShaderLEDs_Controllers::rainbow002_001(u8* data, int leds) {
    return ShaderLEDs_Controller_Bases::rainbow002(data, leds, 0.2, 0.2, 0.5, std::make_shared<Colors::HSVColorPath>(128, 255));
}

std::unique_ptr<ShaderLEDs> ShaderLEDs_Controllers::rainbow002_002(u8* data, int leds) {
    return ShaderLEDs_Controller_Bases::rainbow002(data, leds, 0.2, 0.5, 1.5, std::make_shared<Colors::HSVColorPath>(128, 255));
}

std::unique_ptr<ShaderLEDs> ShaderLEDs_Controllers::rainbow002_003(u8* data, int leds) {
    return ShaderLEDs_Controller_Bases::rainbow002(data, leds, 0.2, 0.5, 1.5, std::make_shared<ArbitraryCosinePath>());
}

std::unique_ptr<TickLEDs> TickLEDs_Controllers::missles001_001(u8* data, int leds) {
	return TickLEDs_Controller_Bases::missles001(data, leds, 3, 7, 10.0, 5.0, std::make_shared<Colors::HSVColorPath>(200, 255), 95 * 2, 138 * 2, 500);
}