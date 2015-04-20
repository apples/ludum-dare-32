#ifndef _PHYSICSENGINE_H
#define _PHYSICSENGINE_H

#include"components.hpp"
#include "entcom.hpp"

struct CollisionData {
	enum class HitDir {
		UNKNOWN,
		UP,
		DOWN,
		LEFT,
		RIGHT
	};
	struct Hit {
		EntID eid;
		HitDir dir = HitDir::UNKNOWN;
		Hit() = default;
		Hit(EntID eid, HitDir dir) : eid(eid), dir(dir) {}
	};
	std::vector<Hit> hits;
};

void physics_step(DB& db, double timeStep);

#endif
			
