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

	Uint8 state;		/*Whether the enemy will be aggressive or defensive*/
	Vector2D target;	/*Position of the entity to attack*/
	Vector2D track[4];	/*Points around the target to follow*/
	Uint8 tracking;		/*Index of which position the enemy is following*/
	float speed;		/*How fast the enemy should move*/
	float lastr;

	Attack ranged;
	Uint8 shot_out;
	float cooldown;

	int health;
	int health_max;

	Sprite *sprite;
	Uint8 id;

	Uint8 boss;			/*1 is djinn boss, 2 is leviathan boss*/
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

void enemy_check_attack(void *ent);

void enemy_update_attack(void *ent);

int enemy_hp_current();

#endif