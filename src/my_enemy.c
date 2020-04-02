#include "my_enemy.h"
#include "simple_logger.h"
#include "my_entity.h"

void enemy_new(Enemy *e)
{
	e->battle_col = col_new_circle(536, 536, 64, 0);
	e->hitstun = 0;
	e->lastatk = -1;
}

void enemy_on_hit(void *ent, Attack *atk)
{
	Entity *entity;
	entity = (Entity *)ent;
	if (atk->reset)
	{
		entity->enm.lastatk = -1;
		atk->reset = false;
	}
	if (!entity || entity->enm.lastatk == atk->index)
	{
		return;
	}
	entity->enm.lastatk = atk->index;

	entity->forcedir.x = atk->knockback.x * atk->direction.x;
	entity->forcedir.y = atk->knockback.y * atk->direction.y;
	vector2d_normalize(&entity->forcedir);
	entity->forcemag = atk->force;
	entity->accel = - (atk->force * 0.34);
 }

void enemy_on_col(void *ent, Item *item)
{
	Vector2D diff;
	float dist;
	Entity *entity;
	entity = (Entity *)ent;

	if (item->id == 3)
	{
		diff = vector2d(entity->position.x + 64 - item->trackpos.x - 16, entity->position.y + 64 - item->trackpos.y - 16);
		dist = SDL_sqrtf(SDL_powf(diff.x, 2) + SDL_powf(diff.y, 2));
		entity->forcedir.x = -diff.x;
		entity->forcedir.y = -diff.y;
		vector2d_normalize(&entity->forcedir);
		entity->forcemag = dist;
		entity->accel = -(30.75 * (dist / 60));
	}
	else
	{
		entity->forcedir.x = entity->position.x + 64 - item->pos.x;
		entity->forcedir.y = entity->position.y + 64 - item->pos.y;
		vector2d_normalize(&entity->forcedir);
		entity->forcemag = item->force;
		entity->accel = -(item->force * 0.34);
	}
}