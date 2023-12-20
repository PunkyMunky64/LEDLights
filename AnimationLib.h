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

namespace ShaderLEDs_Controllers {
	std::shared_ptr<ShaderLEDs> rainbow001_001(u8 data[], int leds);
}

namespace TickLEDs_Controllers {
	std::shared_ptr<TickLEDs> missles001_001(u8 data[], int leds);
}