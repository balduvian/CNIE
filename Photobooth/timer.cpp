#include "stdafx.h"
#include "timer.h"
#include <chrono>

photobooth::pb_long photobooth::getTime() {
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

photobooth::Timer::Timer(pb_long t) {
	going = false;
	time = t;
	start = std::chrono::high_resolution_clock::now();
}

void photobooth::Timer::adjust(pb_long t) {
	start = std::chrono::high_resolution_clock::now();
}

void photobooth::Timer::begin() {
	going = true;
}

void photobooth::Timer::stop() {
	going = false;
}

bool photobooth::Timer::update(float& along) {
	if (going) {
		auto end = std::chrono::high_resolution_clock::now();
		if (std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() >= time) {
			going = false;
			return true;
		}
		return false;/* to put here, or not to put here */
	}
	return false;
}