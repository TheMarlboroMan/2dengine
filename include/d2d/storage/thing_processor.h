#pragma once

#include "definitions.h"
#include "attribute.h"
#include <map>
#include <string>

namespace d2d { namespace storage {

/**
 * a thing processor is basically an interface: it is something that must be 
 * implemented by the final application and that will receive an interpretation
 * of a "thing" in a map. Whatever to do with that thing is up to the 
 * final implementation.
 */

class thing_processor {

	public:

/**
 * sets up the processor before it begins to be fed things.
 */
	virtual void                setup()=0;

/**
 * processes the given thing.
 */
	virtual void                load(d2d::storage::position, int, const std::map<std::string, d2d::storage::attribute>&)=0;
};

}}
