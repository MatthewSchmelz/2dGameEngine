#pragma once
#ifndef __E_COMPANION_H__
#define __E_COMPANION_H__

#include "entity.h"

/*
	@brief Spawn a fighter entity
	@return NULL on error, or a fighter entity otherwise
*/

Entity* companion_new();


void companion_pursue(Entity* self, Entity* target);

#endif // !__FIGHTER_H__