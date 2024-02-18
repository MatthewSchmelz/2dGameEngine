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

#endif