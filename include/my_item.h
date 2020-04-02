#ifndef __MY_ITEM_H__
#define __MY_ITEM_H__

#include "gf2d_sprite.h"
#include "my_collider.h"

typedef struct Item_S
{
	Uint8 id;			//What item is this
	Uint8 count;		//How many the player has
	Vector2D pos;		//Where is the item in space
	Vector2D trackpos;	//Where should it be heading
	Vector2D trackdir;	//Which direction it should be heading
	Sprite *icon;		//What should be displayed on the UI
	Sprite *out;		//What should be displayed currently in space
	float active_s;		//What frame should we start checking for collisions
	float active_e;		//What frame should we stop checking for collisions
	float frame;		//Current frame of the animation
	Uint8 moving;		//Whether the item should be moving or not
	void(*use)(void *player);	
	void(*update)(void *player);
	CirCol col;			//Collider to be used for battle effects
	RectCol col2;		//Only for use if it fits better
	float force;		//How much knockback should be done to the target
	Uint8 collided;		//Check if this has already collided with the target
}Item;

/*
	@brief fill up a player's item slot with the required data
	@param a pointer to the players item slot
	@param the id representing the item to give
*/
void item_new(Item *i, Uint8 id);

/*
	@brief increase the count of players item from id by amount given
*/
void item_add(Uint8 id, Uint8 amount);

/*
	@brief initializes and updates the items every frame
	@param i - the item to update
	@param p - a pointer to the player
*/
void item_update_potion(Item *i, void *p);

void item_use_potion(void *p);

void item_update_bomb(Item *i, void *p);

void item_use_bomb(void *p);

void item_update_flash(Item *i, void *p);

void item_use_flash(void *p);

void item_update_chain(Item *i, void *p);

void item_use_chain(void *p);

void item_update_looper(Item *i, void *p);

void item_use_looper(void *p);

/*
	@brief scroll the items based on player movement
*/
void item_scroll(Item *i, Vector2D movement);

#endif