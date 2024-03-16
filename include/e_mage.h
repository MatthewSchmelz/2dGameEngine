#pragma once
#ifndef __E_MAGE_H__
#define __E_MAGE_H__

#include "entity.h"

/*
	@brief Spawn a rogue entity
	@return NULL on error, or a rogue entity otherwise
*/

Entity* mage_new();


void mage_pursue(Entity* self, Entity* target);

#endif // !__FIGHTER_H__