#include "simple_logger.h"
#include "camera.h"
#include "world.h"
#include "entity.h"

typedef struct
{	
	Entity* entity_list;
	Uint32 entity_max;
} EntityManager;


static EntityManager _entity_manager = { 0 }; // Init a local global entity manager

void entity_system_close();

extern int spike;
extern int currScore;

void entity_system_init(Uint32 max) {
	if (_entity_manager.entity_list) {
		slog("Cannot have two instances of an entity manager, one is already active");
		return;
	}
	if (!max) {
		slog("cannot allocate 0 entities!");
		return;
	}
	_entity_manager.entity_list = gfc_allocate_array(sizeof(Entity), max);
	if (!_entity_manager.entity_list) {
		slog("Failed to allocate global entity list");
		return;
	}
	_entity_manager.entity_max = max;
	atexit(entity_system_close);

};

void entity_system_close() {
	entity_clear_all(NULL);
	if (_entity_manager.entity_list) {
		free(_entity_manager.entity_list);
	}
	memset(&_entity_manager, 0, sizeof(EntityManager));
};



void entity_clear_all(Entity* ignore) {
	int i;
	for (i = 0; i < _entity_manager.entity_max; i++) {
		if (&_entity_manager.entity_list[i] == ignore) {
			continue;
		}
		if (!_entity_manager.entity_list[i]._inuse) {
			continue; //skip this iteration of loop
		}
		entity_free(&_entity_manager.entity_list[i]);
	}
};

Entity* entity_new() {
	int i;
	for (i = 0; i < _entity_manager.entity_max; i++) {
		if (_entity_manager.entity_list[i]._inuse) {
			continue;
		}
		memset(&_entity_manager.entity_list[i], 0, sizeof(Entity));
		_entity_manager.entity_list[i]._inuse =1;

		//Set default Color
		//Set Default scale
		return &_entity_manager.entity_list[i];
	}
	slog("No more available entities");
	return NULL;
};

/*
	@brief clean up an entity, and free its spot for future use
	@param self the entity to free
*/

void entity_free(Entity* self) {
	if (!self) {
		return;
	}
	//slog("Freeing Entity");
	gf2d_sprite_free(self->sprite);
	//anything else allocated for the entity would be cleaned up here
	if (self->free) {
		self->free(self->data);
	}
	memset(self, 0, sizeof(Entity));
};

void entity_think(Entity* self) {
	if (!self) {
		return;
	}
	if (self->think) {
		self->think(self);
	}
}

void entity_pursue(Entity* self, Entity * target) {
	if (!self) {
		return;
	}
	if (self->pursue) {
		self->pursue(self,target);
	}
}

void entity_system_think() {
	int i;
	for (i = 0; i < _entity_manager.entity_max; i++) {
		if (!_entity_manager.entity_list[i]._inuse) {
			continue;
		}
		entity_think(&_entity_manager.entity_list[i]);
	}
};	

void entity_update(Entity* self) {
	if (!self) {
		return;
	}	
	//All Entities need to remain within bounds, check that here
	//If entity has an owner, it is a bullet and can fly off map
	if (!self->owner) {
		if (self->position.x < 32) {
		self->position.x = 32;
		} else if(self->position.x > 1120) {  
			self->position.x = 1120;
		}
		if (self->position.y < 32) {
			self->position.y = 32;
		}
		else if (self->position.y > 864) {
			self->position.y = 864;
		}

		//
		if (spike) {
			Vector2D tilePos = position_to_tile(self->position);
			//All spikes on left
			if (((tilePos.x == 3) && (tilePos.y == 3)) || ((tilePos.x == 2) && (tilePos.y == 3)) || ((tilePos.x == 2) && (tilePos.y == 2)) || ((tilePos.x == 3) && (tilePos.y == 2))) {
				self->free(self);
				//self->damage(self);
			}
			//All spikes on right
			if (((tilePos.x == 14) && (tilePos.y == 3)) || ((tilePos.x == 15) && (tilePos.y == 3)) || ((tilePos.x == 14) && (tilePos.y == 2)) || ((tilePos.x == 15) && (tilePos.y == 2))) {
				self->free(self);
				//self->damage(self);
			}
		}
		


	}


	if (self->update) {
		self->update(self);
	}
}

void entity_damage(Entity* self) {
	if (!self) {
		return;
	}
	if (self->damage) {
		self->damage(self);
		return;
	}

	if (self->health > 0) {
		self->health = self->health - 1;
		if (self->health <= 0) {
			self->free(self);
			currScore++;
		}
	}

	return;

}

void entity_system_update() {
	int i;
	for (i = 0; i < _entity_manager.entity_max; i++) {
		if (!_entity_manager.entity_list[i]._inuse) {
			continue;
		}
		entity_update(&_entity_manager.entity_list[i]);
	}
};


void entity_draw(Entity* self) {
	Vector2D camOffset,position;
	if (!self) {
		return;
	}
	camOffset = camera_get_offset();
	if (self->draw) {
		self->draw(self);
		return;
	} 
	if (self->sprite) {
		//Create a draw offset
		//Vector2D drawOffset;
		//drawOffset.x = self->position.x-32;
		//drawOffset.y = self->position.y-32;
		vector2d_add(position,self->position,camOffset);
		gf2d_sprite_render(
			self->sprite,
			position,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			(Uint32)self->frame
			);
	}
};


void entity_system_draw() {
	int i;
	for (i = 0; i < _entity_manager.entity_max; i++) {
		if (!_entity_manager.entity_list[i]._inuse) {
			continue;
		}
		entity_draw(&_entity_manager.entity_list[i]);
	}
};

// Function to check collision between two entities
int check_collision(Entity* entity1, Entity* entity2) {
	// Check if entities are on different teams
	if (entity1 == NULL || entity2 == NULL) {
		slog("One or two entities not found");
		return 0;
	}
	if (entity1->team != entity2->team) {
		// Assuming hitbox is a member of Entity representing its collision shape
		return gfc_circle_overlap(entity1->hitbox, entity2->hitbox);
	}
	return 0;
}









/*eol@eof*/