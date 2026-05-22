#include "d2d/collision/collision_tracker.h"
#include "d2d/collision/tools.h"
#include <stdexcept>

using namespace d2d::collision;

bool collision_tracker::is_attached(
	const spatiable& _target
) const {

	return nullptr!=get_host(_target);
}

const spatiable * collision_tracker::get_host(
	const spatiable& _target
) const {

	//The algorithms here can be much better but...
	for(const auto& watch : watched) {

		if(!watch.attached.size()) {

			continue;
		}

		auto it=std::find_if(
			std::begin(watch.attached),
			std::end(watch.attached),
			[&_target](const spatiable * _t) -> bool {return _t==&_target;}
		);

		if(it!=std::end(watch.attached)) {

			return watch.body;
		}
	}

	return nullptr;
}

collision_tracker& collision_tracker::restart() {

	watched.clear();
	targets.clear();
	return *this;
}

collision_tracker& collision_tracker::reset() {

	corrections.clear();
	passive_movements.clear();
	for(auto& item : watched) {

		item.attached.clear();
	}
	return *this;
}

collision_tracker& collision_tracker::tic() {

	corrections.clear();
	passive_movements.clear();

	for(auto& watched_entity : watched) {

		//No movement? no problem.
		if(!watched_entity.body->has_moved()) {

			continue;
		}

		//Overwrite the previous vector of the moving entity (fear not, part
		//of the watched node and not the spatiable itself!).
		auto vector=watched_entity.previous_vector;
		watched_entity.previous_vector=watched_entity.body->get_motion_vector();

		//If there are attached entities and we are moving we add passive
		//movements for all the attached entities here.
		if(watched_entity.attached.size() 
			&& !(vector.x==0. && vector.y==0.)
		) {

			for(auto& attached : watched_entity.attached) {

				passive_movements.push_back({watched_entity.body, attached, vector});
			}
		}

		//Now, let's check all targets for collisions...
		const auto& current=watched_entity.body->get_box();
		const auto& previous=watched_entity.body->get_previous_box();
		for(auto& target : targets) {

			//Maybe we have to run some application-specific checks... This is
			//the point to do so.
			if(watched_entity.can_push_policy) {

				if(!watched_entity.can_push_policy(*watched_entity.body, *target)) {

					continue;
				}
			}

			if(!collides_with(current, *target)) {

				continue;
			}

			//If attached, just ignore it!... 
			if(std::any_of(
				std::begin(watched_entity.attached),
				std::end(watched_entity.attached),
				[&target](const spatiable * _att) -> bool {
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

			if(is_left_of(*target, previous)) {

				corrections.push_back({watched_entity.body, target, box_edge::left});
			}
			else if(is_right_of(*target, previous)) {

				corrections.push_back({watched_entity.body, target, box_edge::right});
			}
			else if(is_above(*target, previous)) {

				corrections.push_back({watched_entity.body, target, box_edge::top});
			}
			else if(is_below(*target, previous)) {

				corrections.push_back({watched_entity.body, target, box_edge::bottom});
			}
		}
	}

	return *this;
}

collision_tracker& collision_tracker::watch(
	const spatiable& _spatiable,
	can_push_policy_fn _push_policy
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

	watched.push_back({&_spatiable, {}, _spatiable.get_motion_vector(), _push_policy});
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

collision_tracker& collision_tracker::correct_snaps() {

	for(const auto& c : corrections) {

		correct_snap(c);
	}

	return *this;
}

int collision_tracker::correct_snaps(
	const spatiable& _target
) {

	int edges=0;
	for(const auto& c : corrections) {

		if(c.target==&_target) {

			edges|=correct_snap(c);
		}
	}

	return edges;
}

int collision_tracker::correct_snap(
	const collision_tracker_correction& _node
) {

	switch(_node.edge) {

		case box_edge::left:
			snap_to_left_of(*_node.target, *_node.watched); 
			return aabb_edges::left;
		case box_edge::right:
			snap_to_right_of(*_node.target, *_node.watched); 
			return aabb_edges::right;
		case box_edge::top:
			snap_to_top_of(*_node.target, *_node.watched); 
			return aabb_edges::top;
		case box_edge::bottom:
			snap_to_bottom_of(*_node.target, *_node.watched); 
			return aabb_edges::bottom;
	}

	return 0;
}

d2d::motion::motion_vector collision_tracker::attached_vector_for(
	const spatiable& _attached
) const {

	d2d::motion::motion_vector result{};

	for(const auto& c : passive_movements) {

		if(&_attached!=c.target) {

			continue;
		}

		result+=c.vector;
	}

	return result;
}
