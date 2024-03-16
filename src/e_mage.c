#include "simple_logger.h"
#include "world.h"
#include "e_bullet.h"
#include "e_mage.h"
#include "camera.h"

void mage_think(Entity* self);
void mage_update(Entity* self);
void mage_free(Entity* self);
void mage_pursue(Entity* self, Entity* target);
void mage_draw(Entity* self);


extern int fog;

extern Entity* player;

Entity* mage_new() {
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
	//Spawn at one of 3 gates, if Gates closed, spawn at the middle gate
	if (!fog) {
		int randomIndex = rand() % 3; // Generate a random number between 0 and 2

		switch (randomIndex) {
		case 0:
			self->position = tile_to_position(vector2d(2, 0));
			break;
		case 1:
			self->position = tile_to_position(vector2d(9, 0));
			break;
		case 2:
			self->position = tile_to_position(vector2d(15, 0));
			break;
		default:
			// Handle unexpected cases if any
			break;
		}
	}
	else {
		self->position = tile_to_position(vector2d(9, 0));
	}
	// Entity's Position

	self->think = mage_think;
	self->update = mage_update;
	self->free = mage_free;
	self->hitbox = gfc_circle(self->position.x, self->position.y, 400);
	self->team = 1;
	self->health = 1;
	self->pursue = mage_pursue;
	self->draw = mage_draw;
};

void mage_think(Entity* self) {
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
void mage_update(Entity* self) {
	if (!self) return;
	self->frame += 0.1;
	if (self->frame >= 16) self->frame = 0;
	vector2d_add(self->position, self->position, self->velocity);
	self->hitbox = gfc_circle(self->position.x + 64, self->position.y + 64, 50);
};
void mage_free(Entity* self) {
	if (!self) return;
	entity_free(self);
};

void mage_pursue(Entity* self, Entity* target) {
	Vector2D dir = { 0 };
	Sint32 mx = 0, my = 0;
	if (!target || !self) {
		slog("Missing Enities for fighter_pursue");
		return;
	}
	Vector2D loc = target->position;
	/*


	float dx = self->position.x - loc.x;
	float dy = self->position.y - loc.y;
	float distanceSquared = (dx * dx) + (dy * dy);

	// Compare squared distance with the squared threshold (10 * 10 = 100 in this case)
	if (distanceSquared <= 100) {
		entity_free(self);
	}
	*/

	//Test hitboxes using gfc_circles
	if (gfc_circle_overlap(self->hitbox, target->hitbox)) {
		entity_free(self);
		entity_damage(target);
		//Damage the player here

	}



	if(gfc_crandom() > 0.995){
		bullet_new(self, player->position);

	}



	if (self->position.x < loc.x) dir.x = 1;
	if (self->position.y < loc.y) dir.y = 1;
	if (self->position.x > loc.x) dir.x = -1;
	if (self->position.y > loc.y) dir.y = -1;
	vector2d_normalize(&dir);
	vector2d_scale(self->velocity, dir, 1);


};





void mage_draw(Entity* self) {
	Vector2D camOffset, position;
	if (!self) {
		return;
	}
	camOffset = camera_get_offset();
	if (self->sprite) {
		//Create a draw offset
		//Vector2D drawOffset;
		//drawOffset.x = self->position.x-32;
		//drawOffset.y = self->position.y-32;
		Color mouseColor = gfc_color8(0, 255, 0, 255);
		vector2d_add(position, self->position, camOffset);
		gf2d_sprite_render(
			self->sprite,
			position,
			NULL,
			NULL,
			NULL,
			NULL,
			&mouseColor,
			NULL,
			(Uint32)self->frame
		);
	}
};



/*eol@eof*/