#pragma once
#include <memory>
#include "LedBuildingBlocks.h"
#include "TickLEDs.h"

//A library containing functions that return animations or handle them.
//One function within the "skeleton code" for this project should successfully get lights going.

//Ideas:
//Evolving warm and cool Light with spontaneous other whites spin across
//Random floating gradients
//Color-ranged missles
//Rainbow missles
//Quick rotating hsv
//Overlaying stepping hsv
//Stepping center "beat" hsv
//Random Dissipating wave points

//TODO: Figure out how to make the animations interactive with other data sources and change parameters

namespace ShaderLEDs_Controller_Bases {
	std::unique_ptr<ShaderLEDs> rainbow001(u8* data, int leds, int ledspersecond, std::shared_ptr<Colors::ColorPath360> path);
	std::unique_ptr<ShaderLEDs> rainbow002(u8* data, int leds, float alpha, float beta, float gamma, std::shared_ptr<Colors::ColorPath360> path);
}

namespace TickLEDs_Controller_Bases {
	std::unique_ptr<TickLEDs> missles001(u8* data, int leds, int mincms, int maxcms, int isq, int red, std::shared_ptr<Colors::ColorPath360> path, int mindeg, int maxdeg, int probabilitydivider);
}

namespace ShaderLEDs_Controllers {
	std::unique_ptr<ShaderLEDs> rainbow001_001(u8* data, int leds);
	std::unique_ptr<ShaderLEDs> rainbow001_002(u8* data, int leds);
	std::unique_ptr<ShaderLEDs> rainbow002_001(u8* data, int leds);
}

namespace TickLEDs_Controllers {
	std::unique_ptr<TickLEDs> missles001_001(u8* data, int leds);
}