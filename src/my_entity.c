#include <stdlib.h>
#include "simple_logger.h"
#include "my_entity.h"

typedef struct
{
	Uint32	maxEnts;
	Entity	*entityList;
}EntityManager;

static EntityManager entity_manager = { 0 };

void entity_manager_close()
{
	int i;
	for (i = 0; i < entity_manager.maxEnts; i++)
	{
		if (entity_manager.entityList[i]._inuse)
		{
			entity_free(&entity_manager.entityList[i]);
		}
	}
	entity_manager.maxEnts = 0;
	free(entity_manager.entityList);
	entity_manager.entityList = NULL;
	slog("entity manager closed");
}

void entity_manager_init(Uint32 maxEnts)
{
	if (entity_manager.entityList != NULL)
	{
		entity_manager_close();
	}
	if (!maxEnts)
	{
		slog("cannot intialize a zero size entity list!");
		return;
	}
	entity_manager.entityList = (Entity *)malloc(sizeof(Entity)* maxEnts);
	if (entity_manager.entityList == NULL)
	{
		slog("failed to allocate %i entities for the entity manager", maxEnts);
		return;
	}
	entity_manager.maxEnts = maxEnts;
	memset(entity_manager.entityList, 0, sizeof(Entity)*maxEnts);
	slog("entity manager initialized");
	atexit(entity_manager_close);
}

Entity *entity_new()
{
	int i;
	for (i = 0; i < entity_manager.maxEnts; i++)
	{
		if (entity_manager.entityList[i]._inuse)continue;
		entity_manager.entityList[i]._inuse = 1;
		return &entity_manager.entityList[i];
	}
	slog("out of open entity slots in memory!");
	return NULL;
}


void entity_free(Entity *self)
{
	if (!self)return;
	gf2d_sprite_free(self->sprite);
	memset(self, 0, sizeof(Entity));
}

void entity_update(Entity *self)
{
	if (!self)return;
	if (self->fpl == 0)
	{
		return;
	}
	self->frame = self->frame + 0.1;
	if (self->frame > self->fpl)self->frame = 0;
}

void entity_update_all()
{
	int i;
	for (i = 0; i < entity_manager.maxEnts; i++)
	{
		if (!entity_manager.entityList[i]._inuse)
		{
			continue;
		}
		entity_update(&entity_manager.entityList[i]);
	}
}

void entity_draw(Entity *self)
{
	if (self == NULL)
	{
		slog("cannot draw sprite, NULL entity provided!");
		return;
	}
	gf2d_sprite_draw(
		self->sprite,
		self->position,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		(Uint32)self->frame);
}

void entity_draw_all()
{
	int i;
	for (i = 0; i < entity_manager.maxEnts; i++)
	{
		if (!entity_manager.entityList[i]._inuse)
			continue;
		entity_draw(&entity_manager.entityList[i]);
	}
}

void entity_scroll(Vector2D movement, Entity *e)
{
	e->position = vector2d(e->position.x - movement.x, e->position.y - movement.y);
	e->enm.battle_col.origin.x -= movement.x;
	e->enm.battle_col.origin.y -= movement.y;
}

void entity_check_hits(Uint8 atk, CirCol *atk_col)
{
	int x;
	for (x = 0; x < entity_manager.maxEnts; x++)
	{
		if (entity_manager.entityList[x].isEnm == 1)
		{
			if (col_circle_circle(atk_col, &entity_manager.entityList[x].enm.battle_col))
			{
				enemy_on_hit(&entity_manager.entityList[x].enm, atk);
			}
		}
	}
}