#ifndef __MY_ENEMY_H__
#define __MY_ENEMY_H__

#include "my_collider.h"
#include "my_attack.h"
#include "my_item.h"

typedef struct Enemy_S
{
	int lastatk;		/*Last attack hit by the player used to avoid getting hit twice by the same hitbox*/
	CirCol battle_col;	/*Collider to be used in a battle scene*/
	float hitstun;		/*Duration of hitstun*/
}Enemy;

/*
	@breif creates a new enemy and enemy entity
	@return pointer to a new enemy
*/
void enemy_new(Enemy *e);

/*
	@brief allocates knockback and damage dealt when they collide with a player's attack
	@param ent - the entity of the enemy
	@param atk - the data of the attack colliding with
*/
void enemy_on_hit(void *ent, Attack *atk);

void enemy_on_col(void *ent, Item *item);

#endif