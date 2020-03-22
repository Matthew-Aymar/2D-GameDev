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

	slog("%f, %f : %f, %f", atk->knockback.x, atk->knockback.y, atk->direction.x, atk->direction.y);

	entity->forcedir.x = atk->knockback.x * atk->direction.x;
	entity->forcedir.y = atk->knockback.y * atk->direction.y;
	vector2d_normalize(&entity->forcedir);
	entity->forcemag = atk->force;
	entity->accel = - (atk->force * 0.34);
 }