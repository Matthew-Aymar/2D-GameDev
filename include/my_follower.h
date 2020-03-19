#ifndef __MY_FOLLOWER_H__
#define __MY_FOLLOWER_H__

#include "gf2d_sprite.h"
#include "my_collider.h"
#include "my_scene.h"
#include "my_attack.h"

typedef struct Follower_S
{
	Uint8 id;		/*Identifier for the follower type*/
	Uint8 mode;		/*AI mode of the follower in combat: 0 - passive, 1 - defense, 2 - offense*/

	Sprite *sprite;	/*Sprite to use for the follower*/
	Vector2D pos;	/*position of the follower in 2d space*/

	Uint8 moving;
	Vector2D forcedir;
	float forcemag;
	float accel;

	int health;	    /*current hp of the follower*/
	int speed;	    /*movement speed of the follower*/
	int attack;		/*Amount of damage done by follower attacks*/
	int magic;		/*Amount of damage done by follower skills*/
	int defense;	/*Followers damage resistance*/

	CirCol col;		/*Collider to use in battle*/

	Scene *scene;	/*Scene the follower should be active in*/

	Uint8 bflip;	/*direction of the bounce*/
	float bounce;	/*Bounce added to the track pos for idle effect*/

	float cooldown;		/*Cooldown for basic attacks*/
	Attack melee;		/*basic melee attack when in offense mode*/
	Attack ranged;		/*basic ranged attacks when in defense mode*/
	Uint8 shot_out;		/*Whether the follower has fired its attack already*/
	Vector2D target;	/*The position of the nearest enemy*/
}Follower;

/*
	@brief initializes a follower to values based on the creature
	@param f -> pointer to a follower
	@param id -> id of the creature to set
*/
void follower_new(Follower *f, Uint8 id);

/*
	@brief update the follower movement based on position and draw its sprite
	@param player - a pointer to the player data
	@param f - the follower to update
*/
void follower_update(void *player, Follower *f);

/*
	@brief draws the currently active follower
*/
void follower_draw(Follower *f); 

/*
	@brief sees if the follower should do a basic attack and then preforms one if able
*/
void follower_check_attack(Follower *f);

/*
	@brief update the followers attacks every frame
*/
void follower_update_attack(Follower *f);

/*
	@breif scrolls the followers based on player movement
*/
void follower_scroll(Follower *f, Vector2D movement);

/*
	@brief fires a laser after a short delay from the followers position through the mouse position
*/
void follower_laser(Follower *f, Vector2D mousepos);

#endif