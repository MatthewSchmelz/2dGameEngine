#include "simple_logger.h"
#include "e_fighter.h"


void fighter_think(Entity* self);
void fighter_update(Entity* self);
void fighter_free(Entity* self);
void fighter_pursue(Entity* self, Entity* target);


Entity* fighter_new() {
	Entity* self;
	self = entity_new();
	if (!self) {
		slog("failed to spawn a player");
		return NULL;
	}
	self->sprite = gf2d_sprite_load_all(
		"images/ed210.png",
		128,
		128,
		15,
		0
	); //Entity's sprite
	self->frame = 0;
	self->position = vector2d(0, 0); // Entity's Position

	self->think = fighter_think;
	self->update = fighter_update;
	self->free = fighter_free;
};

void fighter_think(Entity* self) {
	/*
	Vector2D dir = { 0 };
	Sint32 mx = 0, my = 0;
	if (!self) return;
	SDL_GetMouseState(&mx, &my);
	if (self->position.x < mx) dir.x = 1;
	if (self->position.y < my) dir.y = 1;
	if (self->position.x > mx) dir.x = -1;
	if (self->position.y > my) dir.y = -1;
	vector2d_normalize(&dir);
	vector2d_scale(self->velocity, dir, 3);
	*/

};
void fighter_update(Entity* self) {
	if (!self) return;
	self->frame += 0.1;
	if (self->frame >= 16) self->frame = 0;
	vector2d_add(self->position, self->position, self->velocity);
};
void fighter_free(Entity* self) {
	if (!self) return;
	entity_free(self);
};

void fighter_pursue(Entity* self, Entity* target) {
	Vector2D dir = { 0 };
	Sint32 mx = 0, my = 0;
	if (!target || !self) {
		slog("Missing Enities for fighter_pursue");
		return;
	} 
	Vector2D loc = target->position;
	
	float dx = self->position.x - loc.x;
	float dy = self->position.y - loc.y;
	float distanceSquared = (dx * dx) + (dy * dy);

	// Compare squared distance with the squared threshold (10 * 10 = 100 in this case)
	if (distanceSquared <= 100) {
		entity_free(self);
	}

	

	
	if (self->position.x < loc.x) dir.x = 1;
	if (self->position.y < loc.y) dir.y = 1;
	if (self->position.x > loc.x) dir.x = -1;
	if (self->position.y > loc.y) dir.y = -1;
	vector2d_normalize(&dir);
	vector2d_scale(self->velocity, dir, 1);

};









/*eol@eof*/