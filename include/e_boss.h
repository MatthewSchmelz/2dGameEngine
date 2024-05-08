#pragma once
#ifndef __E_BOSS_H__
#define __E_BOSS_H__

#include "entity.h"

/*
	@brief Spawn a boss entity
	@return NULL on error, or a boss entity otherwise
*/

Entity* boss_new();


void boss_pursue(Entity* self, Entity* target);

#endif // !__boss_H__