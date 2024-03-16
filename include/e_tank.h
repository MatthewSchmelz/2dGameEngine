#pragma once
#ifndef __E_TANK_H__
#define __E_TANk_H__

#include "entity.h"

/*
	@brief Spawn a rogue entity
	@return NULL on error, or a rogue entity otherwise
*/

Entity* tank_new();


void tank_pursue(Entity* self, Entity* target);

#endif // !__FIGHTER_H__