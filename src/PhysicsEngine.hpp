#ifndef _PHYSICSENGINE_H
#define _pHYSICSENGINE_H

#include"components.hpp"
#include "entcom.hpp"

struct CollisionData {
	std::vector<EntID> hits;
};

void physics_step(DB& db, double timeStep);

#endif
			
