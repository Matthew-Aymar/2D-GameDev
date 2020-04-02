#ifndef __MY_TILE_H__
#define __MY_TILE_H__

#include "gf2d_sprite.h"
#include "my_collider.h"
#include "my_interactable.h"

typedef struct Tile_S
{
	Sprite *sprite;		/*Image to use for the tile*/
	RectCol col;		/*Collider of the tile*/
	Vector2D origin;	/*X, Y position in 2D space*/
	Vector2D size;		/*X, Y size of the tile & collider*/
	Interactable inter;	/*The interactable occupying this tile*/
	Uint8 interactable;	/*Whether this tile contains a interactable*/
}Tile;

/*
	@brief Create a new tile
	@param s - the sprite to use for the tile
	@param pos - the x,y position of the tile
	@param size - the x, y scale of the tile
	@param solid - whether the tile should have a collider or not
	@note creates a tile by new blank and new col
*/
Tile tile_new(Sprite *s, Vector2D pos, Vector2D size, int solid);

/*
	@brief draw the tile to the screen
	@param the tile to draw
	@note should be called every frame 
*/
void tile_draw(Tile *t);

/*
	@brief clears up the tiles space in memory
	@param t - the tile to free
*/
void tile_free(Tile *t);

#endif