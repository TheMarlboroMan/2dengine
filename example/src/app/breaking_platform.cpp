#include "app/breaking_platform.h"

using namespace app;

breaking_platform::breaking_platform(
	d2d::collision::point _pt,
	int _breaking_ms,
	int _gone_ms,
	int _return_ms
):
	ms_breaking{_breaking_ms},
	ms_gone{_gone_ms},
	ms_returning{_return_ms},
	timeout{ms_breaking / 1000.f},
	collision_box{_pt, w, h}
{}

void breaking_platform::start_breaking() {

	if(!is_ok()) {

		return;
	}

	state=states::breaking;
	timeout.reset().target( (float)ms_breaking / 1000.f);
}

void breaking_platform::tic(
	float _delta
) {

	if(is_ok()) {

		return;
	}

	timeout.tic(_delta);

	if(timeout.is_counting()) {

		return;
	}

	switch(state) {

		case states::ok:
			return;
		case states::breaking:
			state=states::gone;
			timeout.reset().target((float)ms_gone/1000.f);
			return;
		case states::gone:
			state=states::returning;
			timeout.reset().target((float)ms_returning/1000.f);
			return;
		case states::returning:
			state=states::ok;
			return;
	}
}

std::ostream& app::operator<<(
	std::ostream& _stream,
	const breaking_platform& _block
) {

	_stream<<"breaking_platform["<<_block.get_box()<<"]";
	return _stream;
}
