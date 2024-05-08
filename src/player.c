#include "simple_logger.h"
#include "camera.h"
#include "player.h"


void player_think(Entity *self);
void player_update(Entity* self);
void player_free(Entity* self);
void player_damage(Entity* self);
void player_draw(Entity* self);

extern int invuln, stompOn, menu;

Entity *player_new() {
	Entity* self;
	self = entity_new();
	if (!self) {
		slog("failed to spawn a player");
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
		"images/wizardwalk.png",
		32,
		32,
		8,
		0
	); //Entity's sprite
	self->frame =0;
	self->position = vector2d(0,0); // Entity's Position

	self ->think = player_think;
	self->update = player_update;
	self->free = player_free;
	self->damage = player_damage;
	self->hitbox = gfc_circle(self->position.x+64, self->position.y+64, 10);
	self->team = 0;
	self->health = 2;
	self->draw = player_draw;
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
	if (self->frame >= 8) self->frame = 0;
	vector2d_add(self->position, self->position, self->velocity);
	//Update the hitbox
	if (stompOn) {
		self->hitbox = gfc_circle(self->position.x+64, self->position.y+64, 400);
	}
	else {
		self->hitbox = gfc_circle(self->position.x+64, self->position.y+64, 10);
	}
	
	camera_center_on(self->position);


};
void player_free(Entity* self) {
	if (!self) return;
};

void player_damage(Entity* self) {
	slog("player damaged");
	if (invuln == 1) {
		return;
		slog("Player took damage but was shielded");
	}

	if (self->health > 0) {
		self->health = self->health - 1;
	}
	else {
		slog("Player Took Lethal Hit.");
		menu = 0;
	}
}

void player_draw(Entity* self) {
	Vector2D camOffset, position, scale;
	if (!self) {
		return;
	}
	camOffset = camera_get_offset();
	if (self->sprite) {
		//Create a draw offset
		//Vector2D drawOffset;
		//drawOffset.x = self->position.x-32;
		//drawOffset.y = self->position.y-32;
		scale.y = 1.5;
		scale.x = 1.5;
		vector2d_add(position, self->position, camOffset);
		gf2d_sprite_render(
			self->sprite,
			position,
			&scale,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			(Uint32)self->frame
		);
	}
};





/*eol@eof*/