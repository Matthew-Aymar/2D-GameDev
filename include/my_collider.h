#ifndef __MY_COLLIDER_H__
#define __MY_COLLIDER_H__

typedef struct RectCol_S
{
	Vector2D origin;	/*Starting position of the box*/
	Vector2D size;		/*Size, x : width, y : height*/
	int solid;			/*Whether it should stop movement or not*/
}RectCol;

typedef struct CirCol_S
{
	Vector2D origin;	/*Starting position of the circle*/
	float radius;		/*Size of the circle*/
	int solid;			/*Whether it should stop movement or not*/
}CirCol;

/*
	@brief Creates and returns a new rect collider with the parameters provided
	@param floats for position and size, and a boolean for solid
*/
RectCol col_new_rect(float posx, float posy, float sizex, float sizey, int solid);

/*
	@brief Creates and returns a new circle collider with the parameters provided
	@param floats for position and radius, and a boolean for solid
*/
CirCol col_new_circle(float posx, float posy, float radius, int solid);

/*
	@brief circle-on-circle collision
	@param pointers to the two colliders to check
	@return 1 on collision, 0 otherwise
*/
int col_circle_cirle(CirCol *col1, CirCol *col2);

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

#endif