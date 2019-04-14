#include "stdafx.h"
#include <chrono>

namespace photobooth {

	typedef long long pb_long;
	typedef std::chrono::steady_clock::time_point pb_point;

	pb_long getTime();

	class Timer {
	public:
		Timer(pb_long time);

		void adjust(pb_long time);
		void begin();
		void stop();
		void reset();
		bool update(float&);
		float getAlong();
	private:
		bool going;
		pb_point start;
		pb_long time;
	};
}