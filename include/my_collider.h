#ifndef __MY_COLLIDER_H__
#define __MY_COLLIDER_H__

#include <SDL.h>
#include "gfc_types.h"
#include "gfc_vector.h"
#include "my_scene.h"

typedef struct RectCol_S
{
	Vector2D origin;	/*Starting position of the box*/
	Vector2D size;		/*Size, x : width, y : height*/
	Uint8 solid;		/*Whether it should stop movement or not*/
}RectCol;

typedef struct CirCol_S
{
	Vector2D origin;	/*Starting position of the circle*/
	float radius;		/*Size of the circle*/
	Uint8 solid;		/*Whether it should stop movement or not*/
}CirCol;

/*
	@brief Creates and returns a new rect collider with the parameters provided
	@param floats for position and size, and a boolean for solid
*/
RectCol col_new_rect(float posx, float posy, float sizex, float sizey, Uint8 solid);

/*
	@brief Creates and returns a new circle collider with the parameters provided
	@param floats for position and radius, and a boolean for solid
*/
CirCol col_new_circle(float posx, float posy, float radius, Uint8 solid);

/*
	@brief circle-on-circle collision
	@param pointers to the two colliders to check
	@return 1 on collision, 0 otherwise
*/
int col_circle_circle(CirCol *col1, CirCol *col2);

/*
	@brief circle-on-rect collision
	@param pointers to the two colliders to check
	@return 1 on collision, 0 otherwise
*/
int col_circle_rect(CirCol *col1, RectCol *col2);

/*
	@brief rect-on-rect collision
	@param pointers to the two colliders to check
	@return 1 on collision, 0 otherwise
*/
int col_rect_rect(RectCol *col1, RectCol *col2);

int col_battle_bounds(Scene *s, Vector2D point);

/*
	@brief clears the collider and its space in memory
	@param the rectangle collider to free
*/
void col_free_rect(RectCol *col);

/*
	@brief clears the collider and its space in memory
	@param the circle collider to free
*/
void col_free_cir(CirCol *col);

#endif