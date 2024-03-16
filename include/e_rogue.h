#pragma once
#ifndef __E_ROGUE_H__
#define __E_ROGUE_H__

#include "entity.h"

/*
	@brief Spawn a rogue entity
	@return NULL on error, or a rogue entity otherwise
*/

Entity* rogue_new();


void rogue_pursue(Entity* self, Entity* target);

#endif // !__FIGHTER_H__