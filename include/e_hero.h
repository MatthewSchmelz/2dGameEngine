#pragma once
#ifndef __E_hero_H__
#define __E_hero_H__

#include "entity.h"

/*
	@brief Spawn a hero entity
	@return NULL on error, or a hero entity otherwise
*/

Entity* hero_new();


void hero_pursue(Entity* self, Entity* target);

#endif // !__FIGHTER_H__