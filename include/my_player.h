#ifndef __MY_PLAYER_H__
#define __MY_PLAYER_H__

#include "gf2d_sprite.h"
#include "my_entity.h"

typedef struct
{
	Entity	*player_ent;/*entity assoiciated with the player*/
	Uint8	battle;		/*check if the player is currently in a battle scene*/
	Uint8	dir;		/*check the direction the player is facing for interactions*/
	Uint8	moving;		/*check if the player is in motion or idle*/
	float	speed;		/*how fast the player should move in 2D space*/

	/*Overworld Sprites*/
	Sprite  *over_forward;
	Sprite  *over_left;
	Sprite  *over_backward;
	Sprite  *over_right;
	Sprite  *over_idle;
}Player;

/*
	@brief create a new player and entity
	@return NULL on out of memory or error, a pointer to a player otherwise
	@note should only be 1 player active at a time
*/
Player *player_new();

/*
	@brief check players movement input
	@param current frame's key events
	@note should be run every iteration
*/
void player_check_movement(Uint8 W, Uint8 A, Uint8 S, Uint8 D);

/*
	@brief move the player in the overworld & update thier sprite based on direction
	@param integer representing the direction wanted
		0 - back
		1 - top right
		2 - right
		3 - bottom right
		4 - forwards
		5 - bottom left
		6 - left
		7 - top left
	@note only moves on 4 directions
*/
void player_movement_overworld();

/*
	@brief move the player in battle & update thier sprite based on direction
	@param integer representing the direction wanted
		0 - back
		1 - top right
		2 - right
		3 - bottom right
		4 - forwards
		5 - bottom left
		6 - left
		7 - top left
	@note allows for 8 directional movement
*/
//void player_movement_battle();

//void player_interact();

//void player_attack();

void player_free(Player *self);

#endif