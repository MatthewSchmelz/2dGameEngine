#pragma once
#ifndef __E_ARCHMAGE_H__
#define __E_ARCHMAGE_H__

#include "entity.h"

/*
	@brief Spawn a rogue entity
	@return NULL on error, or a rogue entity otherwise
*/

Entity* archmage_new();


void archmage_pursue(Entity* self, Entity* target);

#endif // !__FIGHTER_H__