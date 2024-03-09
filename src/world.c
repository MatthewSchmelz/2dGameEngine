#include "simple_json.h"
#include "simple_logger.h"
#include "gf2d_graphics.h"
#include "camera.h"
#include "world.h"


void world_tile_layer_build(World* world) {
	int i, j;
	int index;
	Vector2D position;
	Uint32 frame;
	if (!world)return;

	//if (!world->tileSet) return;

	if (world ->tileLayer) {
		gf2d_sprite_free(world->tileLayer);
	}
	world->tileLayer = gf2d_sprite_new();

	world->tileLayer->surface = gf2d_graphics_create_surface(world->tileWidth * world->tileSet->frame_h, world->tileHeight * world->tileSet->frame_h);

	world->tileLayer->frame_w = world->tileWidth * world->tileSet->frame_w;
	world->tileLayer->frame_h = world->tileHeight * world->tileSet->frame_h;

	
	if (!world->tileLayer->surface) {
		slog("failed to create tilelayer surface");
		return;
	}

	for (j = 0;j < world->tileHeight; j++) {
		for (i = 0; i < world->tileWidth; i++) {
			index = i + (j * world->tileWidth);
			// Check if index is within the bounds of tileMap
			if (index >= world->tileWidth * world->tileHeight) {
				// Handle the out-of-bounds access or skip this iteration
				// Depending on your requirements
				continue; // or break; or handle the error
			}
			if (!world->tileMap[index]) {
				continue;
			}
			if (world->tileMap[index] == 0) {
				continue;
			}

			position.x = i * world->tileSet->frame_w;
			position.y = j * world->tileSet->frame_h;
			frame = world->tileMap[index] - 1;
			gf2d_sprite_draw_to_surface(world->tileSet,position,NULL,NULL,frame,world->tileLayer->surface);
		}
	}
	world->tileLayer->texture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(),world->tileLayer->surface);
	if (!world->tileLayer->texture) {
		slog("failed to convert world tile layer to texture");
		return;
	}

}



World* world_test_new() {
	int i;
	int width = 75, height = 45;
	World* world;
	world = world_new(width, height);

	if (!world) return NULL;
	world->background = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
	world->tileSet = gf2d_sprite_load_all("images/backgrounds/dungeonTiles.png", 16, 16, 1, 1);
	for (i = 0; i < width; i++) {
		world->tileMap[i] = 1;
		world->tileMap[i + ((height - 1) * width)] = 1;
	}
	for (i = 0; i < height; i++) {
		world->tileMap[i*width] = 1;
		world->tileMap[i*width + (width-1)] = 1;
	}
	world->tileLayer = gf2d_sprite_new();

	world_tile_layer_build(world);

	return world;
}

World* world_load(const char* filename) {
	SJson* json = NULL;
	SJson* wjson = NULL;
	World* world = NULL;
	SJson* vertical, * horizontal;
	SJson* item;
	int w = 0, h = 0;
	int i, j;
	int tile;
	const char* tileSet;
	const char* background;
	int frame_w, frame_h;
	int frames_per_line;
	if (!filename) {
		slog("No filename provided for World_load");
		return NULL;
	}


	json = sj_load(filename);
	if (!json) {
		slog("failed to load world file %s", filename);
		return NULL;
	}
	wjson = sj_object_get_value(json, "world");
	if (!wjson) {
		slog("%s missing 'world' object", filename);
		sj_free(json);
		return NULL;
	}

	vertical = sj_object_get_value(wjson, "tileMap");
	if (!vertical) {
		slog("%s missing tileMap", filename);
		sj_free(json);
		return NULL;
	}
	h = sj_array_get_count(vertical);
	horizontal = sj_array_get_nth(vertical,0);
	w = sj_array_get_count(horizontal);

	world = world_new(w, h);
	if (!world) {
		slog("failed to create space for a new world for file %s", filename);
		sj_free(json);
		return NULL;
	}
	for (j = 0; j < h;j++) {
		horizontal = sj_array_get_nth(vertical, j);
		if (!horizontal)continue;
		for (i = 0; i < w;i++) {
			item = sj_array_get_nth(horizontal, i);
			if (!item)continue;
			tile = 0;
			sj_get_integer_value(item, &tile);
			world->tileMap[i + (j* w)] = tile;
		}
	}
	background = sj_object_get_value_as_string(wjson, "background");
	world->background = gf2d_sprite_load_image(background);

	tileSet= sj_object_get_value_as_string(wjson, "tileSet");
	sj_object_get_value_as_int(wjson, "frame_w", &frame_w);
	sj_object_get_value_as_int(wjson, "frame_h", &frame_h);
	sj_object_get_value_as_int(wjson, "frames_per_line", &frames_per_line);
	world->tileSet = gf2d_sprite_load_all(tileSet, frame_w, frame_h, frames_per_line, 1);

	world->tileLayer = gf2d_sprite_new();

	world_tile_layer_build(world);

	//sj_free(json);
	return world;
}

/* Working test world for sanity reasons
World* world_test_new() {
	int i;
	int width = 75, height = 45;
	World* world;
	world = world_new(width, height);

	if (!world) return NULL;
	world->background = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
	world->tileSet = gf2d_sprite_load_all("images/backgrounds/dungeonTiles.png", 16, 16, 1, 1);
	for (i = 0; i < width; i++) {
		world->tileMap[i] = 1;
		world->tileMap[i + ((height - 1) * width)] = 1;
	}
	for (i = 0; i < height; i++) {
		world->tileMap[i*width] = 1;
		world->tileMap[i*width + (width-1)] = 1;
	}
	return world;
}
*/





World* world_new(Uint32 Width, Uint32 Height) {
	World* world;

	if ((!Width) || (!Height)) {
		slog("Cannot make a world with no width and height");
		return NULL;
	}

	world = gfc_allocate_array(sizeof(world),1);
	if (!world) {
		slog("failed to allocate new world");
		return NULL;
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
	gf2d_sprite_free(world->tileSet);
	gf2d_sprite_free(world->tileLayer);
	free(world->tileMap); //How many tiles tall map is
	memset(world, 0, sizeof(World));
}


void world_draw(World* world) {
	Vector2D offset;
	if (!world) {
		return;
	}
	offset = camera_get_offset();
	gf2d_sprite_draw_image(world->background, offset);
	if (!world->tileSet)return;
	gf2d_sprite_draw_image(world->tileLayer, offset);



}

void world_setup_camera(World* world) {
	if (!world)return;
	if ((!world->tileLayer) || (!world->tileLayer->surface)) {
		slog("No tilelayer set for world");
		return;
	}
	camera_set_bounds(gfc_rect(0,0,world->tileLayer->surface->w, world->tileLayer->surface->h));
	camera_apply_bounds();
	camera_enable_binding(1);
	
}	















/*
		for (j = 0;j < world->tileHeight; j++) {
		for (i = 0; i < world->tileWidth; i++) {
			index = i + (j * world->tileWidth);
			if (world->tileMap[index] == 0)continue;
			position.x = i * world->tileSet->frame_w;
			position.y = j * world->tileSet->frame_h;
			frame = world->tileMap[index] - 1;

			gf2d_sprite_draw(
				world->tileSet,
				position,
				NULL,
				NULL,
				NULL,
				NULL,
				NULL,
				frame);
		}
	}
	*/
