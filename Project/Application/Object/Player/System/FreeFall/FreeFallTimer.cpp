#include "FreeFallTimer.h"

void FreeFallTimer::Update()
{
	if (!timer_.IsActive()) {
		return;
	}

	timer_.Update();

}
