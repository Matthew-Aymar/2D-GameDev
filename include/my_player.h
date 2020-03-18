#ifndef __MY_PLAYER_H__
#define __MY_PLAYER_H__

#include "gf2d_sprite.h"
#include "my_entity.h"
#include "my_collider.h"
#include "my_room.h"
#include "my_attack.h"
#include "my_follower.h"

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

	CirCol battle_col;	/*Collider to be used in battle scenes*/
	Sprite *attack;		/*Basic attack of the player*/
	Uint8 atknum;		/*Which attack in the sequence*/
	Uint8 attacking;	/*check if the player is in a attack animation*/
	float attackcd;		/*Cooldown duration of the attack string*/

	Attack atk[3];		/*Collection of player attacks*/

	Follower *active;	/*Current follower on the field*/
	Vector2D trackdir;	/*opposite direction for the follower to trail behind*/
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
	@note only moves on 4 directions
*/
void player_movement_overworld();

/*
	@brief move the player in battle & update thier sprite based on direction
	@note allows for 8 directional movement
*/
void player_movement_battle();

//void player_interact();

/*
	@brief starts or updates the attack state and frame
*/
void player_attack();

/*
	@brief check the actions of the player, EX: attacks and interactions
	@param click - the keyboard state of the mouse
	@param space - the keyboard state of the space bar
	@param num - if a key 1-4 is being pressed to swap followers
*/
void player_check_actions(Uint8 click, Uint8 space, float mx, float my, Uint8 num);

/*
	@breif free the player and related data from memory
*/
void player_free(Player *self);

/*
	@breif returns the player_ent's collider for overworld collisions
*/
RectCol *player_get_rect();

/*
	@brief returns the player's battle collider for combat collisions
*/
CirCol *player_get_circle();

/*
	@brief get players last movement
	@return x and y position
*/
Vector2D player_get_last();

/*
	@brief set the player's battle state TEMP
*/
void player_set_battle();

/*
	@brief draws the player's active follower
*/
void player_draw_follower();

/*
	@brief swaps the active follower in the scene
	@param slot - which follower slot to switch to
*/
void player_swap_follower(Uint8 slot);
#endif