#pragma once
#ifndef __E_BARBARIAN_H__
#define __E_BARBARIAN_H__

#include "entity.h"

/*
	@brief Spawn a rogue entity
	@return NULL on error, or a rogue entity otherwise
*/

Entity* barbarian_new();


void barbarian_pursue(Entity* self, Entity* target);

#endif // !__FIGHTER_H__