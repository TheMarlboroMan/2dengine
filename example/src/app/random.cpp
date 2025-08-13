#include "app/random.h"

using namespace app;

random::random()
	:numgen{0, 255}
{}

int random::get(
	int _min,
	int _max
) {

	if(_min==_max) {

		return _max;
	}
	int res=numgen() % (_max-_min+1);
	return res+_min;

}

