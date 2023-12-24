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
	inline std::unique_ptr<ShaderLEDs> rainbow001(u8* data, int leds, int ledspersecond, std::shared_ptr<Colors::ColorPath360> path) {
		return (std::unique_ptr<ShaderLEDs>) std::make_unique<MovingRainbow>((u8*)data, leds, ledspersecond, path);
	}
	inline std::unique_ptr<ShaderLEDs> rainbow002(u8* data, int leds, float alpha, float beta, float gamma, std::shared_ptr<Colors::ColorPath360> path) {
		return (std::unique_ptr<ShaderLEDs>) std::make_unique<EvolvingWheelRainbow>((u8*)data, leds, alpha, beta, gamma, path);
	}
}

namespace TickLEDs_Controller_Bases {
	inline std::unique_ptr<TickLEDs> missles001(u8* data, int leds, int mincms, int maxcms, int isq, int red, std::shared_ptr<Colors::ColorPath360> path, int mindeg, int maxdeg, int probabilitydivider) {
		std::function<float(float)> velocity = [mincms, maxcms](float){
			return randrange(mincms, maxcms, 10) / 100;
		};
		std::function<float(float)> isqLaw = [isq](float) {
			return isq;
		};
		std::function<float(float)> redshift = [red](float) {
			return red;
		};
		std::function<Colors::RGBu8(float)> missleColor = [path, mindeg, maxdeg](float) {
			return path->to_rgb(randrange(mindeg, maxdeg, 10));
		};
		std::function<float(float, float)> missleProbability = [probabilitydivider](float dt, float) {
			return dt / probabilitydivider;
		};
		return (std::unique_ptr<TickLEDs>)std::make_unique<MissleStarter>(data, leds, missleProbability, missleColor, isqLaw, redshift, velocity);
	}
	inline std::unique_ptr<TickLEDs> waves001(u8* data, int leds, int min_vx_cms, int max_vx_cms, int max_vh_cms, int min_vh_cms, std::shared_ptr<Colors::ColorPath360> path, int mindeg, int maxdeg, int split_threshold, int probabilitydivider, float sigma) {
		std::function<float(float)> velocity = [min_vx_cms, max_vx_cms](float) {
			return randrange(min_vx_cms, max_vx_cms, 10) / 100;
		};
		std::function<float(float)> hvelocity = [min_vh_cms, max_vh_cms](float) {
			return randrange(min_vh_cms, max_vh_cms, 10) / 100;
		};
		std::function<float(float)> x = [leds](float) {
			return randrange(0, leds, 1);
		};
		std::function<Colors::RGBu8(float)> waveColor = [path, mindeg, maxdeg](float) {
			return path->to_rgb(randrange(mindeg, maxdeg, 10));
		};
		std::function<float(float, float)> waveProbability = [probabilitydivider](float dt, float) {
			return dt / probabilitydivider;
		};
		return (std::unique_ptr<TickLEDs>)std::make_unique<RandomBellWaves>(data, leds, waveProbability, waveColor, x, velocity, hvelocity, sigma, 0.01, split_threshold);
	}
}

namespace ShaderLEDs_Controllers {
	std::unique_ptr<ShaderLEDs> rainbow001_001(u8* data, int leds);
	std::unique_ptr<ShaderLEDs> rainbow001_002(u8* data, int leds);
	std::unique_ptr<ShaderLEDs> rainbow002_001(u8* data, int leds);
	std::unique_ptr<ShaderLEDs> rainbow002_002(u8* data, int leds);
	std::unique_ptr<ShaderLEDs> rainbow002_003(u8* data, int leds);
}

namespace TickLEDs_Controllers {
	std::unique_ptr<TickLEDs> missles001_001(u8* data, int leds);
    std::unique_ptr<TickLEDs> waves001_001(u8 *data, int leds);
}