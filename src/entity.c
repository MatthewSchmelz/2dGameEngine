#include "simple_logger.h"
#include "entity.h"

typedef struct
{	
	Entity* entity_list;
	Uint32 entity_max;
} EntityManager;


static EntityManager _entity_manager = { 0 }; // Init a local global entity manager

void entity_system_close();


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
	if (self->update) {
		self->update(self);
	}
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
	if (!self) {
		return;
	}
	if (self->draw) {
		self->draw(self);
		return;
	} 
	if (self->sprite) {
		//Create a draw offset
		Vector2D drawOffset;
		drawOffset.x = self->position.x-64;
		drawOffset.y = self->position.y-64;

		gf2d_sprite_render(
			self->sprite,
			drawOffset,
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