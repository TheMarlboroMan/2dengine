#include "d2d/collision/aabb_solver.h"
#include "d2d/collision/tools.h"

#include <cassert>
#include <sstream>
#include <iostream>

using namespace d2d::collision;

void aabb_response::solve(spatiable& _subject) {

	if(edges & top) {

		collision::snap_to_top_of(_subject, *obstacle);
		return;
	}

	if(edges & bottom) {

		collision::snap_to_bottom_of(_subject, *obstacle);
		return;
	}

	if(edges & left) {

		collision::snap_to_left_of(_subject, *obstacle);
		return;
	}

	if(edges & right) {

		collision::snap_to_right_of(_subject, *obstacle);
		return;
	}
}

void aabb_solver::horizontal(
	collision::spatiable& _box,
	const std::vector<collision::spatiable const *>& _boxes
) {

	assert(_boxes.size());

	const auto& obstacle=*_boxes[0];

	//collision with the right edge of obstacle... This assumes that speeds per
	//tic are never so fast so as to allow one box to move through and object
	//almost completely.
	double right=_box.get_x() + _box.get_w();
	if(
		right > obstacle.get_x()
		&& right < obstacle.get_x() + obstacle.get_w()
	) {

		collision::snap_to_left_of(_box, obstacle);
	}
	else {

		collision::snap_to_right_of(_box, obstacle);
	}
}

void aabb_solver::vertical(
	collision::spatiable& _box,
	const std::vector<collision::spatiable const*>& _boxes
) {

	assert(_boxes.size());

	const auto& obstacle=*_boxes[0];

	//collision with the bottom part of obstacle...
	auto top=_box.get_y() + _box.get_h();
	if(
		top > obstacle.get_y()
		&& top < obstacle.get_y()+obstacle.get_h()
	) {

		collision::snap_to_bottom_of(_box, obstacle);
	}
	else {

		collision::snap_to_top_of(_box, obstacle);
	}

	return;
}

aabb_response aabb_solver::horizontal_complex(
	const collision::spatiable& _subject,
	const std::vector<collision::spatiable const *>& _obstacles
) {

	assert(_obstacles.size());

	const auto& subject_box=use_subject_previous
		? _subject.get_previous_box()
		: _subject.get_box();

	aabb_response res;
	for(const auto obstacle : _obstacles) {

		const auto& obstacle_box=use_obstacle_previous
			? (*obstacle).get_previous_box()
			: (*obstacle).get_box();

		int current_edge=0;

		if(collision::is_left_of(subject_box, obstacle_box)) {
			current_edge=aabb_response::left;
		}
		else if(collision::is_right_of(subject_box, obstacle_box)) {
			current_edge=aabb_response::right;
		}
		else if(collision::collides_with(subject_box, obstacle_box)) {
			current_edge=aabb_response::right | aabb_response::left;
		}
		else {
			std::stringstream ss;
			ss<<"error in horizontal pass, obstacle was nor left, nor right, nor colliding (previous positions used). subject current pos: "
				<<_subject.get_box()
				<<", subject position: "<<_subject.get_box()
				<<", subject prev position: "<<_subject.get_previous_box()
				<<", obstacle position: "<<(*obstacle).get_box()
				<<", obstacle prev position: "<<(*obstacle).get_previous_box();

			throw aabb_response_exception{ss.str()};
		}

		res.edges|=current_edge;

		//use current position to calculate penetration.
		double magnitude=current_edge==aabb_response::left
			? _subject.get_right() - obstacle->get_x()
			: obstacle->get_right() - _subject.get_x();

		if(magnitude > res.magnitude) {

			res.magnitude=magnitude;
			res.obstacle=obstacle;
		}
	}

	return res;
}

aabb_response aabb_solver::vertical_complex(
	const collision::spatiable& _subject,
	const std::vector<collision::spatiable const *>& _obstacles
) {

	assert(_obstacles.size());

	const auto& subject_box=use_subject_previous
		? _subject.get_previous_box()
		: _subject.get_box();

	aabb_response res;
	for(const auto obstacle : _obstacles) {

		const auto& obstacle_box=use_obstacle_previous
			? (*obstacle).get_previous_box()
			: (*obstacle).get_box();

		int current_edge=0;

		if(
			collision::is_below(subject_box, obstacle_box)
		) {

			current_edge=aabb_response::bottom;
		}
		else if(
			collision::is_above(subject_box, obstacle_box)
		) {

			current_edge=aabb_response::top;
		}
		else if(
			collision::collides_with(subject_box, obstacle_box)
		) {

			current_edge=aabb_response::top | aabb_response::bottom;
		}
		else {
			std::stringstream ss;

			ss<<"error in vertical pass, obstacle was nor above, nor below, nor colliding (previous positions used). subject current pos: "
				<<_subject.get_box()
				<<", subject position: "<<_subject.get_box()
				<<", subject prev position: "<<_subject.get_previous_box()
				<<", obstacle position: "<<(*obstacle).get_box()
				<<", obstacle prev position: "<<(*obstacle).get_previous_box();

			throw aabb_response_exception{ss.str()};
		}

		res.edges|=current_edge;

		//use current position to calculate penetration.
		double magnitude=current_edge==aabb_response::bottom
			? _subject.get_top() - obstacle->get_y()
			: obstacle->get_top() - _subject.get_y();

		if(magnitude > res.magnitude) {

			res.magnitude=magnitude;
			res.obstacle=obstacle;
		}
	}

	return res;
}
