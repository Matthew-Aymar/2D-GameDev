#ifndef __MY_ENTITY_H__
#define __MY_ENTITY_H__

#include "gf2d_sprite.h"
#include "my_collider.h"
#include "my_scene.h"
#include "my_enemy.h"
#include "my_attack.h"
#include "my_follower.h"

typedef struct Entity_S
{
	Uint8		_inuse;		/*check if this entity in memory is active or not*/
	Sprite		*sprite;	/*a pointer to the sprite that is used by this entity*/
	float		frame;		/*current frame for the sprite*/
	float		fpl;		/*max frames per line*/

	Vector2D	position;	/*where the entity is in 2D space*/
	void(*think)(struct Entity_S *self);	/*called when an entity draws*/
	
	RectCol		col;		/*Collider of the entity*/
	Scene		*scene;		/*What scene this entity belongs to*/

	Vector2D	forcedir;	/*Direction force should be applied*/
	float		forcemag;	/*Amount of force to apply*/
	float		accel;		/*Force change every update*/

	Uint8		isEnm;		/*Check if this entity should be treated as a enemy*/
	Enemy		enm;		/*Contains battle data and colliders*/
}Entity;

/*
	@brief get a pointer to a new entity
	@return NULL on out of memory or error, a pointer to a blank entity otherwise
*/
Entity *entity_new();
/*
	@brief initialize the entity resource manager
	@param maxEnts upper bound of maximum concurrent entities to be supported
	@note must be called before creating a new entity
*/
void entity_manager_init(Uint32 maxEnts);

/*
	@brief free a previously allocated entity
	@param self a pointer to the entity to free
*/
void entity_free(Entity *self);

void entity_manager_close();

/*
	@brief update every active entity
*/
void entity_update_all();

/*
	@brief draw every active entity
*/
void entity_draw_all();

/*
	@move the entity relative to the player position to create a scrolling effect
	@param movement - the direction the player moved this update
	@param e - the entity to move
*/
void entity_scroll(Vector2D movement, Entity *e);

/*
	@check all enemy entities to see if the player's attack collided
	@param atk - the attack structure to check
*/
int entity_check_hits(Attack *atk);

/*
	@brief check all entity of type enemy to check for collision
*/
int entity_check_col(CirCol *col, Item *item);

/*
	@brief get the closest entity to param point
*/
Vector2D entity_get_closest(Vector2D point);

#endif