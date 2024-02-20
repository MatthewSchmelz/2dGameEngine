#include "simple_logger.h"
#include "e_bullet.h"


void bullet_think(Entity* self);
void bullet_update(Entity* self);
void bullet_free(Entity* self);
void bullet_draw(Entity* self);



Entity* bullet_new(Entity* shooter, Vector2D location) {
	Entity* self;
	Vector2D slop;
	Vector2D adjustposition;
	self = entity_new();
	if (!self) {
		slog("failed to spawn a player");
		return NULL;
	}
	self->sprite = gf2d_sprite_load_all(
		"images/pointer.png",
		128,
		128,
		15,
		0
	); //Entity's sprite
	self->frame = 0;
	self->position = vector2d(0, 0); // Entity's Position

	self->think = bullet_think;
	self->update = bullet_update;
	self->free = bullet_free;
	self->draw = bullet_draw;
	self->owner = shooter;
	//Move the bullet to the center of the player
	self->position = shooter->position;

	//We need to get the slope that the bullt is moving at, this can be stored as a Vector2D, rise/run
	slop.x = (location.x - self->position.x);
	slop.y = (location.y - self->position.y);
	self->slope = slop;

	//self->hitbox = gfc_circle(self->position.x+64, self->position.y+64, 300);
	self->hitbox = gfc_circle(self->position.x, self->position.y, 300);
	self->team = shooter->team;
	slog("Bullet fired");

	return self;
};


//move the bullet towards the location and check if it hit anything
void bullet_think(Entity* self) {
	if (!self) return;
	vector2d_normalize(&self->slope);
	vector2d_scale(self->velocity, self->slope, 4);

	//Check if we hit anything
	

};
void bullet_update(Entity* self) {
	if (!self) return;
	self->frame += 0.1;
	if (self->frame >= 16) self->frame = 0;
	vector2d_add(self->position, self->position, self->velocity);
	self->hitbox = gfc_circle(self->position.x, self->position.y, 10);
	//Check to see if bullets are out of the play area


};
void bullet_free(Entity* self) {
	if (!self) return;
	entity_free(self);
};

void bullet_draw(Entity* self) {

	gf2d_sprite_render(
		self->sprite,
		self->position,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		(Uint32)self->frame
	);
}






/*eol@eof*/