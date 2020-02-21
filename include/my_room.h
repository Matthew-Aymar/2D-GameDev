#ifndef __MY_ROOM_H__
#define __MY_ROOM_H__

#include "simple_json.h"
#include "my_tile.h"

typedef struct Room_S
{
	Tile tiles[240];	/*Collection of all tiles in the room*/
	int *layout;		/*array containing the layout of the room*/
	int maxtiles;		/*Maximum tiles in the room/array*/
	Vector2D origin;	/*Starting position of the room*/
}Room;

/*
	@brief creates a new room in memory, initializes its layout, and creates its tiles
	@param l - SJson array
	@param max - number of values in the SJson array
	@return a pointer to a new room
*/
Room *room_new(int l[240], float xpos, float ypos);

/*
	@brief updates the room, checks for collisions
	@param a pointer to the room to check
*/
void room_update(Room *rm);

/*
	@brief scroll the room relative to player movement
	@param rm - pointer to the current room the player is in.
	@param movement - player movement
*/
void room_scroll(Room *rm, Vector2D movement);

/*
	@brief frees the room in memory
	@param a pointer to the room to free
*/
void room_free(Room *rm);

#endif