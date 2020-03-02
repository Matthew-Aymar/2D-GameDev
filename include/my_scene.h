#ifndef __MY_SCENE_H__
#define __MY_SCENE_H__

#include <SDL.h>
#include "gf2d_sprite.h"

typedef struct Scene_S
{
	Uint8 _active;		//Check if this is the current scene that should be displayed
	Uint8 _anim;		//Check if the background should be updated every frame
	Uint8 _arena;		//Check if the scene should draw an arena
	Sprite *background;	//Image to use as the background for the scene
	float frame;		//Frame for animated backgrounds
	Vector2D origin;	//Origin point for scene arena, 0,0 if it should not have an arena
}Scene;

/*
	@brief Activate specified scene so it will be drawn & updated
	@param The scene to activate
*/
void scene_active(Scene *s);

/*
	@brief Get a scene to assign to an object
	@param The name of the scene to find
	@return a pointer to a scene
*/
Scene *scene_get(char *s);

/*
	@brief draws the active scene's background image
*/
void scene_draw(Scene *s);

/*
	@breif returns the active scene
*/
Scene *scene_get_active();

/*
	@brief Swap active scene from current to the one provided
	@param The name of the scene to find
*/
void scene_swap(char *s);

/*
	@brief initailizes the scenes for use
*/
void scene_init();

/*
	@free and close the scenes from memory
*/
void scene_free();

/*
	@breif draws the background arena relative to player movement
	@param the scene containing the arena
	@param players movement this update
*/
void scene_arena_draw(Scene *s, Vector2D movement);

/*
	@brief draw the arena rim so it can be placed over the player
	@param scene containing the arena
*/
void scene_rim_draw(Scene *s);
#endif