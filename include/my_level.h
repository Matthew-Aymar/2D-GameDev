#ifndef __MY_LEVEL_H__
#define __MY_LEVEL_H__

#include "simple_json.h"
#include "gf2d_sprite.h"
#include "my_collider.h"

typedef struct Room_S
{
	int _inuse;		/*Check whether this room was built already*/
	SJson *layout;	/*Format of the room, to be loaded from a file*/
	char *name;		/*Identifier for the room*/
	int height;		/*Number of tiles on the y-axis*/
	int width;		/*Number of tiles on the x-axis*/
	Vector2D pos;	/*Origin position of the room, anchored to the top-left*/
	RectCol *col;	/*Collection of all colliders in the room*/
}Room;

typedef struct Level_S
{
	SJson *format;	/*Format of the level, collection of 'rooms' to be loaded from a file*/
	char *name;		/*Based on the name of the related scene*/
	Sprite *tiles;	/*Sprites for all tiles tied to the level*/
	int pallete;	/*Size of the tile pallete*/
	Room *rooms;	/*Collection of rooms in the level*/
	int roommax;	/*Maximum amount of rooms in a level at one time*/
}Level;

/*
	@brief creates a new room using a json file
	@param name of the file to find
*/
Room level_room_new(Room *r, int index);

/*
	@brief constructs a new room with collisions
	@param the room to build
*/
void level_room_build(Room *r);

/*
	@brief builds the format for the level based on a file
	@param string to locate the file and to be used as an identifier
*/
void level_build_all(char *n);

/*
	@brief remove components on the currently active level to prepare for a new build
*/
void level_deconstruct();

/*
	@brief draw all elements of the level to the display
	@note to be called every draw frame
*/
void level_draw_all();

/*
	@brief prep the current level for use in memory
*/
void level_init(int maxrooms, int maxtiles);

#endif