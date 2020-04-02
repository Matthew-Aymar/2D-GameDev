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
	Uint8 num;
	Vector2D normal, res;
	float dot;
	if (!self)return;

	if (!(self->forcedir.x == 0 && self->forcedir.y == 0))
	{
		self->forcemag += self->accel;
		if (self->accel >= 0.5 || self->accel <= -0.5)
			self->accel *= 0.5;

		self->position.x += (self->forcedir.x * self->forcemag);
		self->position.y += (self->forcedir.y * self->forcemag);

		if (self->isEnm)
		{
			self->enm.battle_col.origin.x += (self->forcedir.x * self->forcemag);
			self->enm.battle_col.origin.y += (self->forcedir.y * self->forcemag);
			num = col_battle_bounds(scene_get_active(), self->enm.battle_col.origin);
			if (num)
			{
				if (num == 1)
				{
					normal = vector2d(1,-1);
				}
				else if (num == 2)
				{
					normal = vector2d(1, 1);
				}
				else if (num == 3)
				{
					normal = vector2d(-1, 1);
				}
				else if (num == 4)
				{
					normal = vector2d(-1, -1);
				}
				vector2d_normalize(&normal);
				dot = (normal.x * self->forcedir.x) + (normal.y * self->forcedir.y);
				res = vector2d(2 * dot * normal.x, 2 * dot * normal.y);
				res = vector2d(self->forcedir.x - res.x, self->forcedir.y - res.y);
				self->forcedir = res;
				if (self->forcemag * 1.15 < 10)
					self->forcemag *= 1.15;
				self->position.x += res.x * 15;
				self->position.y += res.y * 15;
				self->enm.battle_col.origin.x += res.x * 15;
				self->enm.battle_col.origin.y += res.y * 15;
			}
		}

		if (self->forcemag <= 0.1)
		{
			self->accel = 0;
			self->forcedir = vector2d(0, 0);
		}
	}

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
	if (self->scene != scene_get_active() && self->scene != scene_get("all"))
	{
		return;
	}
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
	e->col.origin.x -= movement.x;
	e->col.origin.y -= movement.y;
}

int entity_check_hits(Attack *atk)
{
	int x;
	for (x = 0; x < entity_manager.maxEnts; x++)
	{
		if (entity_manager.entityList[x].isEnm == 1)
		{
			if (col_circle_circle(&atk->col, &entity_manager.entityList[x].enm.battle_col))
			{
				enemy_on_hit(&entity_manager.entityList[x], atk);
				return 1;
			}
		}
	}
	return 0;
}

int entity_check_col(CirCol *col, Item *item)
{
	int x;
	for (x = 0; x < entity_manager.maxEnts; x++)
	{
		if (entity_manager.entityList[x].isEnm == 1)
		{
			if (col_circle_circle(col, &entity_manager.entityList[x].enm.battle_col))
			{
				enemy_on_col(&entity_manager.entityList[x], item);
				return 1;
			}
		}
	}
	return 0;
}

Vector2D entity_get_closest(Vector2D point)
{
	int x;
	float dist, temp;
	Vector2D enm;
	dist = INT_MAX;
	for (x = 0; x < entity_manager.maxEnts; x++)
	{
		if (entity_manager.entityList[x].isEnm == 1)
		{
			temp = SDL_powf(entity_manager.entityList[x].position.x - point.x, 2);
			temp += SDL_powf(entity_manager.entityList[x].position.y - point.y, 2);
			temp = SDL_sqrtf(temp);
			if (temp < dist)
			{
				dist = temp;
				enm = entity_manager.entityList[x].position;
			}
		}
	}
	return enm;
}