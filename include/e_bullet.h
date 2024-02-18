
#ifndef __E_BULLET_H__
#define __E_BULLET_H__

#include "entity.h"

/*
	@brief Spawns a bullet entity
	@params a location to where it was shot, and an 'owner'
	@return NULL on error, or a bullet Entity otherwise
*/

Entity* bullet_new(Entity* shooter, Vector2D location);




#endif // !__FIGHTER_H__