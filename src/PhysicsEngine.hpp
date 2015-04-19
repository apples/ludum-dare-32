#ifndef _PHYSICSENGINE_H
#define _pHYSICSENGINE_H

#include"components.hpp"

class Physics
{
	//Variables
	double ts;	//timestep
	Database db&;

	/*
	*	function to update the bounding boxes
	*/
	void updateBB(BoundingBox& bb, Velociy& vel)
	{
		bb.rect.left += vel.x*ts;
		bb.rect.top += vel.y*ts;
	}

	void intersect_box(BoundingBox& b1, BoundingBox& b2)
	{
		FloatRect rect1 = b1.rect;
		FloarRect rect2 = b2.rect;

		float right1, bot1, right2, bot2, nleft, ntop, nwidth, nheight;
		right1 = rect1.left + rect2.width;
		bot1 = rect1.top + rect1.height;
		right2 = rect2.left + rect2.width;
		bot2 = rect2.top + rect2.height;

		if(rect1.left < rect2.left) //left bound of r1 is left of r2 left bound
		{
			nleft = rect2.left;
			if(right1 < right2) //r1 right bound is inside r2
				nwidth = right1 - rect2.left;
			else 				//r2 right bound is outside r2
				nwidth = rect2.width;
		}
		else					//left bound of r1 is right of r2 left bound
		{
			nleft = rect1.left;
			if(right1 < right2)	//right bound of r1 is inside of r2
				nwidth = rect1.width;
			else 				//right bound of r1 is outside r2
				nwidth = right2 - rect1.left;
		}

		if(rect1.top > rect2.top) //top bound of r1 is above of r2 top
		{
			ntop = rect2.top;
			if(bot1 > bot2) 	//r1 bottom is inside r2
				nheight = bot2-bot1;
			else 				//r1 bottom is outside r2
				nheight = rect2.height;
		}
		else					//top bound of r1 is below of r2 top
		{
			ntop = rect1.top;
			if(bot1 < right2)	//bottom bound of r1 is inside of r2
				nheight = rect1.height;
			else 				//bottom bound of r1 is outside r2
				nheight = bot2 - rect1.top;
		}
	}


	public:
		Physics(Database data&) {
			db = data;	
		};

		//update function
		void step(double timeStep, Database db&)
		{
			ts = timeStep;
			auto bb_vels = db.query<BoundingBox, Velocity>();
			auto bbs = db.query<BoundingBox>();

			for(auto ent_v : bb_vels) //search entities with velocities
			{
				EntID v_eid = get<0>(ent);
				BoundingBox& v_bb = get<1>(ent).data();
				Velocity& v_vel = get<2>(ent).data();

				updateBB(bb, vel); 
				
				for(auto ent_bb : bbs) 	//search entities with bounding boxes
				{
					EntID bb_eid = get<0>(ent_bb);
					BoundingBox& bb = bbs.get<1>().data();
					if(ent_v == ent_bb)
						continue;

					if(v_bb.rect.intersection(bb.rect)) //maybe don't need the intersection rectangle
					{
						if(bb_eid.get<Solid>())
						{
							intersect_box(v_bb, bb);

						}
						else


				}
					//will need to tell entites that they have collided	
					//make vector of collided entites that each entity has collided with
			}
		}

}



#endif
			
