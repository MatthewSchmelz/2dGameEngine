#pragma once
#ifndef __E_FIGHTER_H__
#define __E_FIGHTER_H__

#include "entity.h"

/*
	@brief Spawn a fighter entity
	@return NULL on error, or a fighter entity otherwise
*/

Entity* fighter_new();


void fighter_pursue(Entity* self, Entity* target);

#endif // !__FIGHTER_H__