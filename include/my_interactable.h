#ifndef __MY_INTERACTABLE_H__
#define __MY_INTERACTABLE_H__

#include "gf2d_sprite.h"
#include "my_entity.h"
#include "my_collider.h"

typedef struct Interactable_S
{
	Uint8 id;							//Which interactable this is
	void(*on_interact)(void *player);	//What to do when the player interacts
	Sprite *sprite;
	void *tile;
}Interactable;

void interactable_new(Uint8 id, Interactable *i, Vector2D position);

void interactable_free(Interactable *i);

/*
	@breif calculates the interactable to generate based on percentages
	@param num a random percentage 0-100
	@return a intager to be used as the id for the interactable
*/
int interactable_chance(int num);

//5 types
/*
	@brief determines the effects based on the id of the item
	@note should be put into the interactable's on_interact function
	@param a pointer to the active player
*/
void interactable_healer(void *player);

void interactable_shuffler(void *player);

void interactable_monster(void *player);

void interactable_box(void *player);

void interactable_rock(void *player);

void interactable_clear(Interactable *i);

void interactable_get_tile(Interactable *i, void *t);

Vector2D interactable_get_pos(Interactable *i);

#endif