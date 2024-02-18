#include "simple_json.h"
#include "simple_logger.h"
#include "gf2d_graphics.h"
#include "world.h"

/*
	@Brief allocates a new empty world
	@returns NULL on error or a blank world
*/

void world_tile_layer_build(World* world) {
	int i, j;
	Uint32 index;
	Vector2D position;
	Uint32 frame;
	if (!world) return;
	if (!world->tileSet)return;
	if (world->tileLayer) {
		gf2d_sprite_free(world->tileLayer);
	}
	world->tileLayer = gf2d_sprite_new();

	world->tileLayer->surface = gf2d_graphics_create_surface(world->tileWidth * world->tileSet->frame_w , world->tileHeight * world->tileSet->frame_h);

	world->tileLayer->frame_h = world->tileHeight * world->tileSet->frame_h;
	world->tileLayer->frame_w = world->tileWidth * world->tileSet->frame_w;

	if (!world->tileLayer->surface) {
		slog("Failed to create tilelayer surface");
		return;
	}

	
	for (j = 0;j < world->tileHeight; j++) {
		for (i = 0; i < world->tileWidth; i++) {
			index = i + (j * world->tileWidth);
			if (world->tileMap[index] == 0)continue;
			position.x = i * world->tileSet->frame_w;
			position.y = j * world->tileSet->frame_h;
			frame = world->tileMap[index] - 1;

			gf2d_sprite_draw_to_surface(
				world->tileSet,
				position,
				NULL,
				NULL,
				frame,
				world->tileLayer->surface
			);
		}
	}
	world->tileLayer = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), world->tileLayer->surface);
	if (!world->tileLayer) {
		slog("failed to convert world tile layer to texture");
		return;
	}
}


World* world_new(Uint32 Width, Uint32 Height) {
	World* world;

	world = gfc_allocate_array(sizeof(world),1);
	if (!world) {
		slog("failed to allocate new world");
		return;
	}
	world->tileMap = gfc_allocate_array(sizeof(Uint8), Height*Width);
	world->tileHeight = Height;
	world->tileWidth = Width;
	return world;
	
}

/*
	@brief frees a previously allocated world
	@param world the world to free
*/

//Frees the world and its memory
void world_free(World* world) {
	if (!world) {
		slog("No world given to free");
		return;
	}
	gf2d_sprite_free(world->background); //Background Image for World
	free(world->tileMap); //How many tiles tall map is
	gf2d_sprite_free(world->tileSet);
	gf2d_sprite_free(world->tileLayer);
	memset(world, 0, sizeof(World));
}


void world_draw(World* world) {

	if (!world) {
		return;
	}
	gf2d_sprite_draw_image(world->background, vector2d(0, 0));
	gf2d_sprite_draw_image(world->tileLayer, vector2d(0, 0));
	
}