#ifndef __MY_PLAYER_H__
#define __MY_PLAYER_H__

#include "gf2d_sprite.h"
#include "my_entity.h"
#include "my_collider.h"
#include "my_room.h"
#include "my_attack.h"
#include "my_follower.h"
#include "my_item.h"

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
	Uint8 can_atk;

	Attack atk[3];		/*Collection of player attacks*/

	float follower_cd;	/*Cooldown for the player to swap*/
	Follower *team[4];	/*Collection of pointers to the player's followers*/
	Vector2D trackdir;	/*opposite direction for the follower to trail behind*/
	Uint8 selected;		/*which slot on the UI is selected, -1 in none*/
	Uint8 picked;		/*Which slot was clicked on*/
	Vector4D window;	/*the position of the team window to check for edits*/

	Uint8 show_status;  /*Whether to display the status screen on the UI*/

	Item items[5];		/*Collection of items the player has*/
	Uint8 item_1;		/*Id of which item is in slot #1 (Q)*/
	Uint8 item_2;		/*Id of which item is in slot #2 (E)*/
	Uint8 item_out;		/*If the item is active*/
	float item_swapcd;	/*to allow for item switching when pressing tab*/

	Vector2D mouse;		/*Mouse positions for tracking*/

	Interactable *interact;
	Uint8 can_interact;

	Uint8 grounded;		/*User cannot act if not grounded*/
	
	int health;
	int health_max;

	int enemy_health;
	int enemy_health_max;

	Uint8 rooms_passed;
	Uint8 next_boss;
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
void player_check_actions(Uint8 left_click, Uint8 right_click, Uint8 space, float mx, float my, Uint8 num, Uint8 tab, Uint8 q, Uint8 e);

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
	@brief returns the player's follower collider for combat collisions
*/
CirCol *player_get_follower_circle();

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
	@param slot1 - which follower slot to switch
	@param slot2 - which follower slot to switch to
*/
void player_swap_follower(Uint8 slot1, Uint8 slot2);

/*
	@brief toggles the player to be able to interact with a particular object
	@param b the state of the boolean
	@param i the object to interact with
*/
void player_can_interact(Uint8 b, Interactable *i);

/*
	@brief frees and generates new followers for the players
*/
void player_shuffle_followers();

/*
	@brief sets item_out to zero
*/
void player_item_out();

int player_follower_col(CirCol *other);

void player_heal_current();

void player_clear_interact();

Vector2D player_get_interact_pos();

void player_follower_new(Uint8 id);

void player_restock();

Vector2D player_position();

/*
	@brief set to let the player keep track of enemy hp
*/
void player_set_enemy_health(int current, int max);

/*
	@brief deal damage to the player 
*/
void player_damage(int amount);

/*
	@brief deal damage to the player's current follower
*/
void player_follower_damage(int amount);

void player_room_passed();

void player_reset_passed();

int player_get_passed();

void player_set_boss();

int player_get_boss();
#endif