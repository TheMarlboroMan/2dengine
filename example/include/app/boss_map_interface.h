#pragma once

#include <d2d/collision/definitions.h>

namespace app {

/**
 *Defines an interface that the boss class uses to communicate with the world
 *outside, doing things such as spawning projectiles, skulls... maybe music?
 */
class boss_map_interface {

	public:

	//Must create a projectile targeted towards the player, from the given point.
	//Can have some extra angle.
	virtual void boss_create_targeted_projectile(d2d::collision::point, double, int=0)=0;

	//Must create a projectile on the given point with the angle expressed by
	//the second argument (velocity being the third).
	virtual void boss_create_directed_projectile(d2d::collision::point, int, double)=0;

	//Must spawn a skull in the location with the given id.
	virtual void boss_spawn_skull(int)=0;

	//Must return the center of the player.
	virtual d2d::collision::point boss_get_target() const=0;
};
}
