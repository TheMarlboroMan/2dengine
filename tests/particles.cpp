#include <iostream>
#include <d2d/components/particle.h>

class rising_particle_module:
	public d2d::components::particle_module_interface {

	virtual void    add(
		d2d::components::particle& _particle, 
		d2d::collision::point _point,
		d2d::components::particle_index 
	) {

		_particle.pos=_point;
		_particle.vector={0., 1.};
		_particle.max_lifetime=0.5;
	}

	virtual void    tic(
		d2d::components::particle& _particle, 
		ldtools::tdelta _delta,
		d2d::components::particle_index 
	) {

		_particle.pos.y+=_particle.vector.y * _delta;
	}

	virtual void    expire(
		d2d::components::particle&, 
		d2d::components::particle_index 
	) {

	}
};

class static_particle_module:
	public d2d::components::particle_module_interface {

	virtual void    add(
		d2d::components::particle& _particle, 
		d2d::collision::point _point,
		d2d::components::particle_index 
	) {

		_particle.pos=_point;
		_particle.vector={0., 0.};
		_particle.max_lifetime=0.3;
	}

	virtual void    tic(
		d2d::components::particle&, 
		ldtools::tdelta,
		d2d::components::particle_index 
	) {

	}

	virtual void    expire(
		d2d::components::particle&, 
		d2d::components::particle_index 
	) {

	}
};

int main(int, char **) {

	rising_particle_module rp;
	static_particle_module sp;
	d2d::components::particle_manager parman{10};
	parman.register_module(rp);
	parman.register_module(sp);

	int input=0;
	bool running=true;
	while(running) {

		std::cout<<"1 - tell\n"
"2 - tic and tell\n"
"3 - add rising\n"
"4 - add static\n"
"0 - exit\n\n>>";

		std::cin>>input;

		switch(input) {
			case 0: running=false; break;
			case 1:
				for(std::size_t i=0; i<parman.size(); i++) {

					const auto& p=parman.get(i);
					std::cout<<i<<" type: "<<p.type<<" >> "<<p.pos<<", "<<p.vector<<", life: "<<p.lifetime<<std::endl;
				}
			break;
			case 2: 
				parman.tic(0.1); 
				for(std::size_t i=0; i<parman.size(); i++) {

					const auto& p=parman.get(i);
					std::cout<<i<<" type: "<<p.type<<" >> "<<p.pos<<", "<<p.vector<<", life: "<<p.lifetime<<std::endl;
				}
			break;
			case 3:
				parman.add(0, {0., 0.});
			break;
			case 4:
				parman.add(1, {0., 0.});
			break;
		}
	}

	return 0;
}

