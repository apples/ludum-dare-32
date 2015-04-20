#ifndef _PHYSICSENGINE_H
#define _pHYSICSENGINE_H

#include"components.hpp"

struct CollisionData {
	std::vector<EntID> hits;
};

void physics_step(Database& db, double timeStep);

#endif
			
