#include <iostream>
#include <vector>
#include <d2d/collision/aabb_static_checker.h>

using bbox=d2d::collision::box;
using d2d::collision::spatiable;
using d2d::collision::point;
using d2d::collision::box_edge;
using d2d::collision::t_position;
using d2d::collision::t_size;

class test_box : public spatiable {

	public:

							test_box(t_position _x, t_position _y, t_size _w, t_size _h):
		bounding_box{{_x, _y}, _w, _h}
	{}

	bbox                     bounding_box;
	virtual bbox&            get_box() {return bounding_box;}
	virtual const bbox&      get_box() const {return bounding_box;}
	virtual const bbox&      get_previous_box() const {return bounding_box;}
	virtual void            commit_box() {};
	virtual void            rollback_box() {};
	virtual point&          get_origin() {return bounding_box.origin;}
	virtual const point&    get_origin() const {return bounding_box.origin;}
	virtual t_position      get_x() const {return bounding_box.origin.x;}
	virtual t_position      get_y() const {return bounding_box.origin.y;}
	virtual t_size          get_w() const {return bounding_box.w;}
	virtual t_size          get_h() const {return bounding_box.h;}
	virtual bool            is_passable_edge(box_edge) const {return false;}
	virtual d2d::motion::motion_vector get_motion_vector() const {return {0., 0.};}
	virtual void            set_motion_vector(d2d::motion::motion_vector) {}
};

int main(int, char **) {

	using namespace d2d::collision;

	test_box subject{244.,985., 12, 24};
	aabb_static_checker sc{subject};
	std::vector<test_box> boxes={
		{256.,976., 16,16},
		{256.,992., 16,16},
		{256.,1008., 16,16}
	};

	bool running=true;
	int option; 
	t_size w, h;
	t_position x, y;

	while(running) {

		std::cout<<"subject at: "<<subject.get_box();
		if(0!=boxes.size()) {

			std::cout<<"\n";
			for(const auto& b: boxes) {

				std::cout<<b.get_box()<<"\n";
			}
		}

		std::cout<<"0 - exit\n1 - run\n2 - change subject\n3 - add box\n4 - clear boxes\n>>";
		std::cin>>option;

		switch(option) {

			case 0: running=false; break;
			case 1: 
				sc.reset();
				sc.detect_all(boxes);

				if(!sc.has_collision()) {

					std::cout<<"all clear\n";
					break;
				}

				std::cout<<"there is a collision\n";
				for(auto& s : sc.get_results()) {

					std::cout<<">> "<<s->get_box()<<"\n";
				}
			break;
			case 2: {
				std::cout<<"reposition subject: enter x y w h >>";
				std::cin>>x>>y>>w>>h;
				//TODO: Control errors;
				auto& sbox=subject.get_box();
				sbox.origin={x,y};
				sbox.w=w;
				sbox.h=h;
			}
			break;
			case 3:
				std::cout<<"new box: enter x y w h >>";
				std::cin>>x>>y>>w>>h;
				//TODO: Control errors;
				boxes.push_back({x, y, w, h});
			break;
			case 4:
				boxes.clear();
			break;
		}
	}

	return 0;
}
