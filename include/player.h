#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "entity.h"

/*
	@brief Spawn a player
	@return NULL on error, or a player entity otherwise
*/

Entity* player_new();

#endif // !__PLAYER_H__