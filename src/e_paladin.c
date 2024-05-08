#include "simple_logger.h"
#include "world.h"
#include "e_paladin.h"
#include "camera.h"

void paladin_think(Entity* self);
void paladin_update(Entity* self);
void paladin_free(Entity* self);
void paladin_pursue(Entity* self, Entity* target);
void paladin_draw(Entity* self);

extern int fog;

Entity* paladin_new() {
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
		"images/mobwalk.png",
		120,
		80,
		10,
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
	SJson* json = NULL;
	SJson* wjson = NULL;
	json = sj_load("config/enemies.json");
	if (!json) {
		slog("failed to load enemies json");
		return NULL;
	}

	wjson = sj_object_get_value(json, "paladin");
	if (!wjson) {
		slog("Missing paladin object in enemies json");
		sj_free(json);
		return NULL;
	}

	float health;
	if (!sj_object_get_value_as_float(wjson, "health", &health)) {
		slog("paladin's health not initialized correctly.");
	}
	else {
		printf("paladin's health: %.2f\n", health);
	}

	self->health = health;

	wjson = sj_object_get_value(json, "paladin");
	if (!wjson) {
		slog("Missing paladin object in enemies json");
		sj_free(json);
		return NULL;
	}

	float speed;
	if (!sj_object_get_value_as_float(wjson, "speed", &speed)) {
		slog("paladin's health not initialized correctly.");
	}
	else {
		printf("paladin's speed: %.2f\n", speed);
	}
	self->speed = speed;

	self->think = paladin_think;
	self->update = paladin_update;
	self->free = paladin_free;
	self->hitbox = gfc_circle(self->position.x, self->position.y, 400);
	self->team = 1;
	
	self->pursue = paladin_pursue;
	self->draw = paladin_draw;
};

void paladin_think(Entity* self) {
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
void paladin_update(Entity* self) {
	if (!self) return;
	self->frame += 0.1;
	if (self->frame >= 10) self->frame = 0;
	vector2d_add(self->position, self->position, self->velocity);
	self->hitbox = gfc_circle(self->position.x + 64, self->position.y + 64, 50);
};
void paladin_free(Entity* self) {
	if (!self) return;
	entity_free(self);
};

void paladin_pursue(Entity* self, Entity* target) {
	Vector2D dir = { 0 };
	Sint32 mx = 0, my = 0;
	if (!target || !self) {
		slog("Missing Enities for paladin_pursue");
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





	if (self->position.x < loc.x) dir.x = self->speed;
	if (self->position.y < loc.y) dir.y = self->speed;
	if (self->position.x > loc.x) dir.x = -self->speed;
	if (self->position.y > loc.y) dir.y = -self->speed;
	vector2d_normalize(&dir);
	vector2d_scale(self->velocity, dir, self->speed);



	if (self->health < 3) {
		if (gfc_crandom() > .99) {
			self->health++;
		}
	}

};


void paladin_draw(Entity* self) {
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
		Color mouseColor = gfc_color8(50, 50, 50, 255);
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