#ifndef __MY_SCENE_MANAGER_H__
#define __MY_SCENE_MANAGER_H__

#include "gf2d_sprite.h"

typedef struct Scene_S
{
	Uint8	_inuse;	/*Checks whether the scene is active in memory or not*/
	char	*name;	/*Identifier for the scene*/
	Sprite	*bg;	/*Background sprite*/
}Scene;

/*
	@brief creates a new scene and adds it to the scene manager
	@param name of scene & the associated background image
*/
void scene_new(char *n, Sprite *bg);

/*
	@brief initializes the scene manager in memory
	@param maximum scenes in project
	@note should be called before a scene is made
*/
void scene_manager_init(Uint8 max);

/*
	@brief loads a scene's contents to be used
	@param pointer to the scene to locate
*/
void scene_load(Scene *s);

/*
	@brief load a new scene in the background for smooth transitions
	@param point to the scene to locate
*/
void scene_load_parallel(Scene *s);

/*
	@brief moves current scene to the background and plays transition animation
	@param pointer to the scene to locate
*/
void scene_transition();

/*
	@brief frees the scene from memory
	@param pointer to the scene to locate
*/
void scene_free(Scene *s);

/*
	@brief returns the active scene's sprite to draw
*/
Sprite *scene_background();

/*
	@brief creates a new entity attached to a scene
	@param the scene to attach the entity too
*/
void *scene_entity(Scene *s);

/*
	@brief returns the active scene in the scene manager
*/
Scene *scene_active();

/*
	@brief returns a scene from the scene manager with name of n
	@param string of characters
*/
Scene *scene_by_name(char *n);

/*
	@brief returns the name of the active scene
*/
char *scene_name();

#endif