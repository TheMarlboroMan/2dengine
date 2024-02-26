#pragma once

#include "definitions.h"
#include "attribute.h"
#include <map>
#include <string>

namespace d2d { namespace storage {

/**
 * interface for a property processor. Maps may have properties that will 
 * depend on the final application so the library cannot provide for much
 * specifity in this regard.
 */

class property_processor {

	public:

/**
 * sets up the processor before it begins to be fed properties. 
 */
	virtual void                setup()=0;

/**
 * processes the attributes present in the map, mapped as "property-name"
 * to its value wrapped in an attribute object.
 */
	virtual void                load(
		const std::map<std::string, d2d::storage::attribute>&
	)=0;
};

}}
