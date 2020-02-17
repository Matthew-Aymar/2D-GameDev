#include <stdlib.h>
#include "simple_logger.h"
#include "gf2d_sprite.h"
#include "my_entity.h"
#include "my_scene_manager.h"

typedef struct SceneManager_S
{
	Uint8 maxScenes;		/*Max number of scenes to be loaded into memory*/
	Scene *sceneList;		/*List of available scenes*/
	Scene *active_scene;	/*Scene currently being used/interacted with*/
	Scene *parallel_scene;	/*Scene loaded in the background*/
}SceneManager;

static SceneManager scene_manager = { 0 };

void scene_manager_close()
{
	int i;
	for (i = 0; i < scene_manager.maxScenes; i++)
	{
		if (scene_manager.sceneList[i]._inuse)
		{
			scene_free(&scene_manager.sceneList[i]);
		}
	}
	scene_manager.maxScenes = 0;
	free(scene_manager.sceneList);
	scene_manager.sceneList = NULL;
	slog("scene manager closed");
}

void scene_manager_init(Uint8 max)
{
	Sprite *empty;
	if (scene_manager.sceneList != NULL)
	{
		scene_manager_close();
	}
	if (!max)
	{
		slog("cannot initialize a zero size scene list!");
		return;
	}
	scene_manager.sceneList = (Scene *)malloc(sizeof(Scene)* max);
	if (scene_manager.sceneList == NULL)
	{
		slog("failed to allocate %i scenes for the scene manager", max);
		return;
	}
	scene_manager.maxScenes = max;
	memset(scene_manager.sceneList, 0, sizeof(Scene)*max);
	slog("Scene manager initialized");
	atexit(scene_manager_close);

	scene_new("all", NULL);
}

void scene_new(char *n, Sprite *bg)
{
	Scene *s;
	int i;
	for (i = 0; i < scene_manager.maxScenes; i++)
	{
		if (scene_manager.sceneList[i]._inuse == 1)
			continue;
		s = &scene_manager.sceneList[i];
		break;
	}
	if (s == NULL)
	{
		slog("out of open slots in memory!");
	}

	if (scene_manager.active_scene == NULL && n != "all")
	{
		scene_manager.active_scene = s;
	}

	s->_inuse	= true;
	s->name		= n;
	s->bg		= bg;
	slog("created new scene with name %s", n);
}

void scene_load(Scene *s)
{
	if (scene_manager.active_scene == NULL)
	{
		scene_manager.active_scene = s;
	}
	else
	{
		scene_free(s);
	}
}

void scene_load_parallel(Scene *s)
{
	if (scene_manager.parallel_scene == NULL)
	{
		scene_manager.parallel_scene = s;
	}
	else
	{
		scene_free(s);
	}
}

void scene_transition()
{
	Scene *z;
	z = scene_manager.active_scene;
	scene_manager.active_scene = scene_manager.parallel_scene;
	scene_manager.parallel_scene = z;
}

void scene_free(Scene *s)
{
	int i;
	if (!s)
		return;
	gf2d_sprite_free(s->bg);
	memset(s, 0, sizeof(Scene));
}

Sprite *scene_background()
{
	return scene_manager.active_scene->bg;
}

void *scene_entity(Scene *s)
{
	Entity *temp;
	temp = entity_new();
	temp->scene = s;
	return temp;
}

Scene *scene_active()
{
	return scene_manager.active_scene;
}

Scene *scene_by_name(char *n)
{
	int i = 0;

	for (i = 0; i < scene_manager.maxScenes; i++)
	{
		if (scene_manager.sceneList[i]._inuse)
		{
			if (scene_manager.sceneList[i].name == n)
			{
				return &scene_manager.sceneList[i];
			}
		}
	}
}

char *scene_name()
{
	return scene_manager.active_scene->name;
}