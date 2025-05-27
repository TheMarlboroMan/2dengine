#include "d2d/collision/collision_tracker.h"
#include "d2d/collision/tools.h"
#include <stdexcept>

using namespace d2d::collision;

collision_tracker& collision_tracker::reset() {

	watched.clear();
	targets.clear();
	return *this;
}

collision_tracker& collision_tracker::tic() {

	corrections.clear();

	for(auto& entity : watched) {

		//No movement? no problem.
		if(!entity.body->has_moved()) {

			continue;
		}

		const auto vector=entity.body->get_motion_vector();
		if(entity.attached.size() 
			&& !(vector.x==0. && vector.y==0.)
		) {

			for(auto& attached : entity.attached) {

				//TODO: Not enjoying the edge part...
std::cout<<"ADDED ATTACHMENT CORRECTION"<<std::endl;
				corrections.push_back({entity.body, attached, vector, box_edge::left});
			}
		}

		const auto& current=entity.body->get_box();
		const auto& previous=entity.body->get_previous_box();
		for(auto& target : targets) {

			if(!collides_with(current, *target)) {

				continue;
			}

std::cout<<"TARGET ATTACHMENTS: "<<entity.attached.size()<<std::endl;

			//If attached, just ignore it!... 
			if(std::any_of(
				std::begin(entity.attached),
				std::end(entity.attached),
				[&target](const spatiable * _att) -> bool {
std::cout<<"ONE AND THE SAME"<<std::endl;
					return target==_att;
				}
			)) {

				continue;
			}


			//TODO TODO TODO
			//all we know now is that the box moved but we don't know where
			//the collision originates from... So we will just use a SHIT
			//approximation and will change it because this will not work
			//reliably when movement happens in both axes at once.
			d2d::motion::motion_vector v{0., 0.};

			if(is_left_of(*target, previous)) {

				corrections.push_back({entity.body, target, v, box_edge::left});
std::cout<<"ADD TARGET CORRECTION"<<std::endl;
			}
			else if(is_right_of(*target, previous)) {

				corrections.push_back({entity.body, target, v, box_edge::right});
std::cout<<"ADD TARGET CORRECTION"<<std::endl;
			}
			else if(is_above(*target, previous)) {

				corrections.push_back({entity.body, target, v, box_edge::top});
std::cout<<"ADD TARGET CORRECTION"<<std::endl;
			}
			else if(is_below(*target, previous)) {

				corrections.push_back({entity.body, target, v, box_edge::bottom});
std::cout<<"ADD TARGET CORRECTION"<<std::endl;
			}
			else {

std::cout<<"NO TARGET CORRECTION??"<<std::endl;
			}
		}
	}

	return *this;
}

collision_tracker& collision_tracker::watch(
	const spatiable& _spatiable
) {

	//Will not allow for the same node to be added twice...
	if(std::any_of(
		std::begin(watched), 
		std::end(watched),
		[&_spatiable](const node& _node) -> bool {return _node.body == &_spatiable;}
	)) {

		return *this;
	}
	
	//Will not allow for elements already into the targets list.
	if(std::any_of(
		std::begin(targets), 
		std::end(targets), 
		[&_spatiable](const spatiable * _node) -> bool {return _node == &_spatiable;}
	)) {

		throw std::runtime_error("cannot watch an element that is already into the target list");
	}

	watched.push_back({&_spatiable, {}});
	return *this;
}

collision_tracker& collision_tracker::unwatch(
	const spatiable& _spatiable
) {

	auto it=std::find_if(
		std::begin(watched), 
		std::end(watched), 
		[&_spatiable](const node& _node) -> bool {return _node.body == &_spatiable;}
	);
	if(std::end(watched)==it) {

		return *this;
	}

	watched.erase(it);
	return *this;
}

collision_tracker& collision_tracker::attach(
	const spatiable& _watched,
	spatiable& _attached
) {

	//watched must be on the watched list.
	auto watched_it=std::find_if(
		std::begin(watched), 
		std::end(watched), 
		[&_watched](const node& _node) -> bool {return _node.body == &_watched;}
	);

	if(watched_it==std::end(watched)) {

		throw std::runtime_error("cannot attach to unwatched node");
	}

	//attached MUST NOT BE on the watched list xD
	if(!std::all_of(
		std::begin(watched), 
		std::end(watched), 
		[&_attached](const node& _node) -> bool {return _node.body != &_attached;}
	)) {

		throw std::runtime_error("cannot attach node that is already watched");
	}

	//No sense in attaching the same thing TWICE... check that too.
	auto& attached_list=(watched_it->attached);
	if(std::any_of(
		std::begin(attached_list),
		std::end(attached_list),
		[&_attached](const spatiable * _node) -> bool {return _node==&_attached;}
	)) {

		return *this;
	}

	attached_list.push_back(&_attached);
std::cout<<"ATTACHED!"<<std::endl;
	return *this;
}

collision_tracker& collision_tracker::detach(
	const spatiable& _watched,
	const spatiable& _attached
) {

	//watched must be on the watched list.
	auto watched_it=std::find_if(
		std::begin(watched), 
		std::end(watched), 
		[&_watched](const node& _node) -> bool {return _node.body == &_watched;}
	);

	if(watched_it==std::end(watched)) {

		throw std::runtime_error("cannot detach from unwatched node");
	}

	auto& attached_list=(watched_it->attached);
	auto pos=std::find_if(
		std::begin(attached_list),
		std::end(attached_list),
		[&_attached](const spatiable * _node) -> bool {return _node==&_attached;}
	);

	if(std::end(attached_list)==pos) {

		return *this;
	}

	attached_list.erase(pos);
	return *this;
}

collision_tracker& collision_tracker::detach_from_all(
	const spatiable& _attached
) {

	//TODO: Soooo very much meh...
	for(auto& watched_node : watched) {

		detach(*watched_node.body, _attached);
	}
	return *this;
}

collision_tracker& collision_tracker::target(
	spatiable& _target
) {

	//Not on the watch list.
	if(std::any_of(
		std::begin(watched),
		std::end(watched),
		[&_target](const node& _node) -> bool {return _node.body==&_target;}
	)) {

		throw std::runtime_error("cannot target a node on the watch list");
	}

	//Not repeated!!
	if(std::any_of(
		std::begin(targets),
		std::end(targets),
		[&_target](const spatiable * _node) -> bool {return _node==&_target;}
	)) {

		return *this;
	}

	targets.push_back(&_target);
	return *this;
}

collision_tracker& collision_tracker::untarget(
	const spatiable& _target
) {

	auto it=std::find_if(
		std::begin(targets),
		std::end(targets),
		[&_target](const spatiable * _node) -> bool {return _node==&_target;}
	);

	if(std::end(targets) == it) {

		return *this;
	}

	targets.erase(it);
	return *this;
}

void collision_tracker::push_correct(
	const collision_tracker_correction& _node
) {

	//Not a snap??
	if(!_node.is_snap()) {

std::cout<<"NO SNAP, MEH"<<std::endl;
		return;
	}

std::cout<<"WILL SNAP"<<std::endl;
	switch(_node.edge) {

		case box_edge::left:
			snap_to_left_of(*_node.target, *_node.watched); 
			return;
		case box_edge::right:
			snap_to_right_of(*_node.target, *_node.watched); 
			return;
		case box_edge::top:
			snap_to_top_of(*_node.target, *_node.watched); 
			return;
		case box_edge::bottom:
			snap_to_bottom_of(*_node.target, *_node.watched); 
			return;
	}
}
