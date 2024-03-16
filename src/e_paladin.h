#pragma once
#ifndef __E_PALADIN_H__
#define __E_PALADIN_H__

#include "entity.h"

/*
	@brief Spawn a fighter entity
	@return NULL on error, or a fighter entity otherwise
*/

Entity* paladin_new();


void paladin_pursue(Entity* self, Entity* target);

#endif // !__FIGHTER_H__