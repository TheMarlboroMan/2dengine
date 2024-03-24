#pragma once

#include <d2d/collision/spatiable.h>
#include <d2d/components/timeouts.h>
#include <iostream>

namespace app {

class breaking_platform:
	public d2d::collision::spatiable {

	public:

	/**
    * the three integers represent breaking, gone and returning times in 
    * ms
    **/
	                                        breaking_platform(d2d::collision::point, int, int, int);
	void                                    tic(float);
	void                                    start_breaking();
	bool                                    is_ok() const {return state==states::ok;}
	bool                                    is_breaking() const {return state==states::breaking;}
	bool                                    is_gone() const {return state==states::gone;}
	bool                                    is_returning() const {return state==states::returning;}
	bool                                    is_solid() const {return !is_gone();}
	int                                     get_breaking_ms() const {return ms_breaking;}
	int                                     get_returning_ms() const {return ms_returning;}
	float                                   get_timer() const {return timeout.get();}

//Begin implementation of spatiable

	virtual d2d::collision::box&            get_box() {return collision_box;}
	virtual const d2d::collision::box&      get_box() const {return collision_box;}
	virtual const d2d::collision::box&      get_previous_box() const {return collision_box;}
	virtual d2d::collision::point&          get_origin() {return collision_box.origin;}
	virtual const d2d::collision::point&    get_origin() const {return collision_box.origin;}
	virtual double                          get_x() const {return collision_box.origin.x;}
	virtual double                          get_y() const {return collision_box.origin.y;}
	virtual int                             get_w() const {return collision_box.w;}
	virtual int                             get_h() const {return collision_box.h;}
	virtual bool                            is_passable_edge(d2d::collision::box_edge) const {return false;}
	virtual d2d::collision::color           get_debug_outline_color() const {return ldv::rgba8(255,0,64,255);}
	virtual d2d::collision::color           get_debug_fill_color() const {return ldv::rgba8(128, 0, 64, 255);}

//End implementation of spatiable

	private:

	enum class states {
		ok,
		breaking,
		gone,
		returning
	}                                   state{states::ok};

	int                                 ms_breaking,
	                                    ms_gone,
	                                    ms_returning;

	d2d::components::timeout            timeout;
	d2d::collision::box                 collision_box;

	static const int                    w{16},
	                                    h{8};
};

std::ostream& operator<<(std::ostream&, const breaking_platform&);
}
