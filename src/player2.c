#include "simple_logger.h"
#include "camera.h"
#include "player2.h"


void player2_think(Entity* self);
void player2_update(Entity* self);
void player2_free(Entity* self);
void player2_damage(Entity* self);


extern int invuln, stompOn, menu;

Entity* player2_new() {
	Entity* self;
	self = entity_new();
	if (!self) {
		slog("failed to spawn a player2");
		return NULL;
	}
	/*self->sprite = gf2d_sprite_load_all(
		"images/ed210.png",
		128,
		128,
		15,
		0
	); //Entity's sprite
	*/
	self->sprite = gf2d_sprite_load_all(
		"images/mobwalk.png",
		120,
		80,
		10,
		0
	); //Entity's sprite
	self->frame = 0;
	self->position = vector2d(0, 0); // Entity's Position

	self->think = player2_think;
	self->update = player2_update;
	self->free = player2_free;
	self->damage = player2_damage;
	self->hitbox = gfc_circle(self->position.x + 64, self->position.y + 64, 10);
	self->team = 0;
	self->health = 2;
};

void player2_think(Entity* self) {
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
	vector2d_scale(self->velocity, dir, 3);
};
void player2_update(Entity* self) {
	if (!self) return;
	self->frame += 0.1;
	if (self->frame >= 10) self->frame = 0;
	vector2d_add(self->position, self->position, self->velocity);
	//Update the hitbox
	if (stompOn) {
		self->hitbox = gfc_circle(self->position.x + 64, self->position.y + 64, 50);
	}
	else {
		self->hitbox = gfc_circle(self->position.x + 64, self->position.y + 64, 10);
	}

	camera_center_on(self->position);


};
void player2_free(Entity* self) {
	if (!self) return;
};

void player2_damage(Entity* self) {
	slog("player2 damaged");
	if (invuln == 1) {
		return;
		slog("player2 took damage but was shielded");
	}

	if (self->health > 0) {
		self->health = self->health - 1;
	}
	else {
		slog("player2 Took Lethal Hit.");
		menu = 0;
	}
}







/*eol@eof*/