#include "simple_logger.h"
#include "world.h"
#include "e_boss.h"
#include "camera.h"

void boss_think(Entity* self);
void boss_update(Entity* self);
void boss_free(Entity* self);
void boss_pursue(Entity* self, Entity* target);
void boss_draw(Entity* self);
void boss_damage(Entity* self);


extern int fog;
extern int bossinvuln;
int boss;
Entity* boss_new() {
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
		"images/bosswalk.png",
		64,
		64,
		6,
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
   //Yoink Stats out of json
	SJson* json = NULL;
	SJson* wjson = NULL;
	json = sj_load("config/enemies.json");
	if (!json) {
		slog("failed to load enemies json");
		return NULL;
	}

	wjson = sj_object_get_value(json, "boss");
	if (!wjson) {
		slog("Missing boss object in enemies json");
		sj_free(json);
		return NULL;
	}

	float health;
	if (!sj_object_get_value_as_float(wjson, "health", &health)) {
		slog("boss's health not initialized correctly.");
	}
	else {
		printf("boss's health: %.2f\n", health);
	}

	self->health = health;

	wjson = sj_object_get_value(json, "boss");
	if (!wjson) {
		slog("Missing boss object in enemies json");
		sj_free(json);
		return NULL;
	}

	float speed;
	if (!sj_object_get_value_as_float(wjson, "speed", &speed)) {
		slog("boss's health not initialized correctly.");
	}
	else {
		printf("boss's speed: %.2f\n", speed);
	}
	self->speed = speed;

	self->think = boss_think;
	self->update = boss_update;
	self->free = boss_free;
	self->hitbox = gfc_circle(self->position.x, self->position.y, 400);
	self->team = 1;
	self->draw = boss_draw;
	self->pursue = boss_pursue;
	self->damage = boss_damage;
};

void boss_think(Entity* self) {
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
void boss_update(Entity* self) {
	if (!self) return;
	self->frame += 0.1;
	if (self->frame >= 6) self->frame = 0;
	vector2d_add(self->position, self->position, self->velocity);
	self->hitbox = gfc_circle(self->position.x + 64, self->position.y + 64, 50);
};
void boss_free(Entity* self) {
	if (!self) return;
	entity_free(self);
};

void boss_pursue(Entity* self, Entity* target) {
	Vector2D dir = { 0 };
	Sint32 mx = 0, my = 0;
	if (!target || !self) {
		slog("Missing Enities for boss_pursue");
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
		entity_damage(target);
		//Damage the player here

	}





	if (self->position.x < loc.x) dir.x = self->speed;
	if (self->position.y < loc.y) dir.y = self->speed;
	if (self->position.x > loc.x) dir.x = -self->speed;
	if (self->position.y > loc.y) dir.y = -self->speed;
	vector2d_normalize(&dir);
	vector2d_scale(self->velocity, dir, self->speed);

};


void boss_draw(Entity* self) {
	Vector2D camOffset, position, scale;;
	if (!self) {
		return;
	}
	camOffset = camera_get_offset();
	if (self->sprite) {
		//Create a draw offset
		//Vector2D drawOffset;
		//drawOffset.x = self->position.x-32;
		//drawOffset.y = self->position.y-32;
		if (bossinvuln == 1) {
			slog("Boss should have changed color");
			Color mouseColor = gfc_color8(150, 50, 200, 255);
			vector2d_add(position, self->position, camOffset);
			scale.y = 2;
			scale.x = 2;
			gf2d_sprite_render(
				self->sprite,
				position,
				&scale,
				NULL,
				NULL,
				NULL,
				&mouseColor,
				NULL,
				(Uint32)self->frame
			);
		}
		else if (bossinvuln ==0) {
			vector2d_add(position, self->position, camOffset);
			scale.y = 2;
			scale.x = 2;
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
		else {
			slog("No idea how you got here, error in boss draw.");
		}
		
	}
};

void boss_damage(Entity* self) {
	if (!self) {
		return;
	}
	if (bossinvuln == 1) {
		if (self->health > 0) {
			self->health = self->health - 1;
			if (self->health <= 0) {
				self->free(self);
				boss = 0;
			}
		}
	}
	else {
		slog("Boss is currently immune!");
	}
	

	return;
}





/*eol@eof*/