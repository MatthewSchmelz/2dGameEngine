#pragma once
#ifndef __E_ninja_H__
#define __E_ninja_H__

#include "entity.h"

/*
	@brief Spawn a ninja entity
	@return NULL on error, or a ninja entity otherwise
*/

Entity* ninja_new();


void ninja_pursue(Entity* self, Entity* target);

#endif // !__FIGHTER_H__