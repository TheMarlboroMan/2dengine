#include <iostream>
#include <string>
#include <d2d/components/persistence.h>
#include <cassert>

int main(int, char **) {

	enum states {
		undiscovered=0,
		discovered=1,
		retrieved=2
	};

	d2d::components::persistence<std::string, int, int> persistence;

	std::cout<<"checking for empty groups"<<std::endl;
	assert(false==persistence.has("treasures"));
	assert(false==persistence.has("artifacts"));
	assert(0==persistence.size());

	std::cout<<"adding groups"<<std::endl;
	persistence.add("treasures");

	std::cout<<"checking the same group cannot be added twice"<<std::endl;
	try {

		persistence.add("artifacts");
		persistence.add("treasures");
		std::cout<<"should have failed, exiting"<<std::endl;
		return 1;
	}
	catch(d2d::components::exception& e) { }

	std::cout<<"checking groups exist now..."<<std::endl;
	assert(true==persistence.has("artifacts"));
	assert(true==persistence.has("treasures"));
	assert(2==persistence.size());
	assert(0==persistence.size("artifacts"));
	assert(0==persistence.size("treasures"));

	std::cout<<"checking it is illegal to ask for non-existent group size"<<std::endl;
	try {

		persistence.size("nothing");
		std::cout<<"should have failed, exiting"<<std::endl;
		return 1;
	}
	catch(d2d::components::exception& e) {}

	std::cout<<"checking for non-existing nodes..."<<std::endl;
	assert(false==persistence.has("artifacts", 1));
	assert(false==persistence.has("artifacts", 2));
	assert(false==persistence.has("treasures", 1));
	assert(false==persistence.has("treasures", 2));

	std::cout<<"adding new nodes..."<<std::endl;
	persistence.add("treasures", 1, undiscovered);
	persistence.add("treasures", 2, undiscovered);
	persistence.add("artifacts", 1, undiscovered);

	std::cout<<"checking the nodes were inserted..."<<std::endl;
	assert(2==persistence.size("treasures"));
	assert(1==persistence.size("artifacts"));
	assert(true==persistence.has("treasures", 1));
	assert(true==persistence.has("treasures", 2));
	assert(true==persistence.has("artifacts", 1));

	std::cout<<"check the same node cannot be added twice"<<std::endl;
	try {
		persistence.add("treasures", 1, undiscovered);
		std::cout<<"should have failed, exiting"<<std::endl;
		return 1;
	}
	catch(d2d::components::exception& e) {}

	std::cout<<"check nothing can be added to non-existent group"<<std::endl;
	try {
		persistence.add("nothing", 1, undiscovered);
		std::cout<<"should have failed, exiting"<<std::endl;
		return 1;
	}
	catch(d2d::components::exception& e) {}

	std::cout<<"changing the state of a node..."<<std::endl;
	persistence.set("artifacts", 1, discovered);

	std::cout<<"checking the state was changed through 'is'"<<std::endl;
	assert(true==persistence.is("artifacts", 1, discovered));
	assert(false==persistence.is("artifacts", 1, undiscovered));
	assert(false==persistence.is("artifacts", 1, retrieved));

	std::cout<<"checking the state was changed through 'get'"<<std::endl;
	assert(discovered==persistence.get("artifacts", 1));
	assert(undiscovered!=persistence.get("artifacts", 1));
	assert(retrieved!=persistence.get("artifacts", 1));

	std::cout<<"checking that a non-existing node cannot be changed (no group)"<<std::endl;
	try {
		persistence.set("nothing", 4, undiscovered);
		std::cout<<"should have failed, exiting"<<std::endl;
		return 1;
	}
	catch(d2d::components::exception& e) {}

	std::cout<<"checking that a non-existing node cannot be changed"<<std::endl;
	try {
		persistence.set("treasure", 4, undiscovered);
		std::cout<<"should have failed, exiting"<<std::endl;
		return 1;
	}
	catch(d2d::components::exception& e) {}

	std::cout<<"checking that a non-existing node cannot be checked with 'is' (no group)"<<std::endl;
	try {
		persistence.is("nothing", 1, undiscovered);
		std::cout<<"should have failed, exiting"<<std::endl;
		return 1;
	}
	catch(d2d::components::exception& e) {}

	std::cout<<"checking that a non-existing node cannot be checked with 'is'"<<std::endl;
	try {
		persistence.is("treasure", 1, undiscovered);
		std::cout<<"should have failed, exiting"<<std::endl;
		return 1;
	}
	catch(d2d::components::exception& e) {}

	std::cout<<"checking that a non-existing node cannot be checked with 'get' (no group)"<<std::endl;
	try {
		persistence.get("nothing", 1);
		std::cout<<"should have failed, exiting"<<std::endl;
		return 1;
	}
	catch(d2d::components::exception& e) {}

	std::cout<<"checking that a non-existing node cannot be checked with 'get'"<<std::endl;
	try {
		persistence.get("treasure", 11);
		std::cout<<"should have failed, exiting"<<std::endl;
		return 1;
	}
	catch(d2d::components::exception& e) {}

	std::cout<<"erasing a group"<<std::endl;
	persistence.erase("artifacts");

	std::cout<<"checking the group was erased"<<std::endl;
	assert(1==persistence.size());
	assert(false==persistence.has("artifacts"));

	std::cout<<"checking that a non-existing group cannot be erased"<<std::endl;
	try {
		persistence.erase("nothing");
		std::cout<<"should have failed, exiting"<<std::endl;
		return 1;
	}
	catch(d2d::components::exception& e) {}

	std::cout<<"clearing a group"<<std::endl;
	persistence.clear("treasures");

	std::cout<<"checking the group was cleared"<<std::endl;
	assert(1==persistence.size());
	assert(0==persistence.size("treasures"));

	std::cout<<"erasing a node"<<std::endl;
	persistence.add("treasures", 1, undiscovered);
	assert(1==persistence.size("treasures"));
	persistence.erase("treasures", 1);

	std::cout<<"checking that the node was erased"<<std::endl;
	assert(0==persistence.size("treasures"));

	std::cout<<"all checks are ok"<<std::endl;
	return 0;
}

