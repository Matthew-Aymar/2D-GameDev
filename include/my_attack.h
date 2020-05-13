#ifndef __MY_ATTACK_H__
#define __MY_ATTACK_H__

#include "gf2d_sprite.h"
#include "my_collider.h"

typedef struct Attack_S
{
	Uint8 index;		/*Check which attack this is in a sequence*/

	Sprite *sprite;		/*The sprite to render every frame*/
	float frame;		/*The current frame of the animation*/
	float start;		/*The starting frame of the attack animation*/
	float end;			/*The ending frame of the attack animation*/
	float active_s;		/*The starting frame for collision checks*/
	float active_e;		/*The ending frame for collision checks*/

	Vector2D direction; /*Direction from the attacker to this attack*/
	Vector2D knockback;	/*Direction of knockback for this attack*/
	Vector2D position;	/*Position of the attack in 2d space*/
	Vector3D rotation;	/*Rotation of the attack sprite*/
	float force;		/*Magnetude of knockback*/

	CirCol col;			/*Collider of the attack*/

	Uint8 reset;		/*Toggle on click to reset the last attack of the enemy*/

	Uint8 projectile;	/*Check if this attack is a projectile*/
	Vector2D launch;	/*Direction of the projectile*/
	float speed;		/*Amount to move each step*/

	float stun;			/*Hitstun duration to put on enemies*/
	int damage;			/*Damage the attack will do to enemies*/
}Attack;

#endif