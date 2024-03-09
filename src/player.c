#include "simple_logger.h"
#include "camera.h"
#include "player.h"


void player_think(Entity *self);
void player_update(Entity* self);
void player_free(Entity* self);


Entity *player_new() {
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
	self->frame =0;
	self->position = vector2d(0,0); // Entity's Position

	self ->think = player_think;
	self->update = player_update;
	self->free = player_free;
	self->hitbox = gfc_circle(self->position.x+128, self->position.y+128, 10);
	self->team = 0;
	self->health = 2;
};

void player_think(Entity* self) {
	const Uint8* keys;
	Vector2D dir = { 0 };
	Sint32 mx = 0, my = 0;
	keys = SDL_GetKeyboardState(NULL);
	if (!self) return;
	if (keys[SDL_SCANCODE_W])
	{
		dir.y = -1;
	}
	if (keys[SDL_SCANCODE_A])
	{
		dir.x = -1;
	}
	if (keys[SDL_SCANCODE_S])
	{
		dir.y = 1;
	}
	if (keys[SDL_SCANCODE_D])
	{
		dir.x = 1;
	}

	vector2d_normalize(&dir);
	vector2d_scale(self->velocity,dir,3);
};
void player_update(Entity* self) {
	if (!self) return;
	self->frame += 0.1;
	if (self->frame >= 16) self->frame = 0;
	vector2d_add(self->position, self->position, self->velocity);
	//Update the hitbox
	self->hitbox = gfc_circle(self->position.x, self->position.y, 10);
	camera_center_on(self->position);


};
void player_free(Entity* self) {
	if (!self) return;
};









/*eol@eof*/