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
	int index;			/*position in the room manager*/
	//Interactable i[5];		/*Interactable object in this room*/
}Room;

/*
	@brief creates a new room in memory, initializes its layout, and creates its tiles
	@param l - SJson array
	@param max - number of values in the SJson array
	@return a pointer to a new room
*/
void *room_new(Room *rm, int l[240], float xpos, float ypos, int index);

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

/*
	@brief initializes all the active rooms and the room manager
*/
void room_manager_init();

/*
	@brief redraws the rooms tiles and checks for collision on the room the player is currently in
	@note index position 4 should be the current room for the player
*/
void room_manager_update();

/*
	@brief scrolls all the active rooms according to player movement
	@param movement - vector of the players movement
*/
void room_manager_scroll(Vector2D movement);

/*
	@free all the rooms associated with the room manager
*/
void room_manager_close();

/*
	@brief checks if the player has entered a different room to allow for collisions
*/
void room_manager_swap(float xpos, float ypos);

int room_check_col(RectCol *col);

#endif