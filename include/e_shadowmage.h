#pragma once
#ifndef __E_shadowmage_H__
#define __E_shadowmage_H__

#include "entity.h"

/*
	@brief Spawn a shadowmage entity
	@return NULL on error, or a shadowmage entity otherwise
*/

Entity* shadowmage_new();


void shadowmage_pursue(Entity* self, Entity* target);

#endif // !__FIGHTER_H__