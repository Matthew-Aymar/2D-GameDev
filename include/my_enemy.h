#ifndef __MY_ENEMY_H__
#define __MY_ENEMY_H__

#include "my_collider.h"

typedef struct Enemy_S
{
	Uint8 lastatk;		/*Last attack hit by the player used to avoid getting hit twice by the same hitbox*/
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
	@param the index of the attack colliding with
*/
void enemy_on_hit(Enemy *e, Uint8 atk);

#endif