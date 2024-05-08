#ifndef __ENTITY_H__
#define __ENTITY_H__


#include <gfc_types.h>
#include "gf2d_sprite.h"
#include "gfc_shape.h"
/*
	@purpose make an entity on the screen
*/

typedef struct Entity_S Entity;

struct Entity_S {
	Uint8 _inuse;
	Sprite* sprite;
	float frame;
	Vector2D position;
	Vector2D velocity;
	void (*think)(Entity* self);
	void (*update)(Entity* self);
	void (*free)(Entity* self);
	void (*draw)(Entity* self);
	void (*damage)(Entity* self);
	void (*pursue)(Entity* self, Entity * target);
	void* data;
	Entity* owner; // Now Entity is a forward declaration
	Vector2D slope;
	Circle hitbox; //gives us a simple area for where the hitbox of the entity is and saves me math
	int team; //0 for player 1 for enemies.
	int health;
	float speed;
};

/*
typedef struct Entity_S {
	Uint8 _inuse; //Is the Entity in Use :: FLAG
	Sprite *sprite; //Entity's sprite
	float frame;
	Vector2D position; // Entity's Position
	Vector2D velocity; //How much an entity's position changes per update
	void (*think)(struct Entity_S* self); //Entity's think function to make decisons
	void (*update)(struct Entity_S *self); //Entity's think function to execute decisons
	void (*free)(struct Entity_S* self); //Free allocated data
	void *data; //For ad hoc addition data for the entity
	Entity* owner; //If the entity has an owner, for use in bullets
	Vector2D slope; //for bullets, the slope they are moving at
}Entity;

*/
/*
	@brief initalizes the entity management system and queues cleanup on exit
	@param 'max' the manximum numbver of entities that can exist at once
*/

void entity_system_init(Uint32 max);

/*
	@brief clean up active entityes
	@param 'ignore' do not clean up this entity
*/

void entity_clear_all(Entity *ignore);

/*
	@brief get a blank entity for use
	@return NULL on no more room or error, a blank entity otherwise
*/

Entity* entity_new();

/*
	@brief clean up an entity, and free its spot for future use
	@param self the entity to free
*/

void entity_free(Entity *self);


/*
	@brief run the think functions for all active entities
*/
void entity_system_think();

/*
	@brief run the update functions for all active entities
*/
void entity_system_update();

/*
	@brief run the draw functions for all active entities
*/
void entity_system_draw();

void entity_damage(Entity* self);

void entity_pursue(Entity* self, Entity* target);


#endif