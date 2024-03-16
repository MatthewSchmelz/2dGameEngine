#ifndef __WORLD_H__
#define __WORLD_H__

#include "gf2d_sprite.h"

typedef struct {
	Sprite* background; //Background Image for World
	Sprite* tileLayer; //prerendered tile layer
	Sprite *tileSet;  //Sprite containing Tiles that make up the world
	Uint8* tileMap;
	Uint32 tileHeight; //How many tiles tall map is
	Uint32 tileWidth; //How many tiles Wide the map is

} World;


/*
	@brief load a world from a config file
	@param filename the name of the world file to load
	@return NULL on error or a useable world otherwise
*/
World* world_load(const char* filename);


World* world_test_new();
/*
	@Brief allocates a new empty world
	@returns NULL on error or a blank world
*/

World *world_new(Uint32 Width, Uint32 Height);

/*
	@brief frees a previously allocated world
	@param world the world to free
*/

//Frees the world and its memory
void world_free(World * world);


void world_draw(World * world);

/*
	@brief set the camera bounds to thr world size

*/
void world_setup_camera(World* world);

Vector2D tile_to_position(Vector2D tilemap);

Vector2D position_to_tile(Vector2D pos);

#endif