#include "my_enemy.h"
#include "simple_logger.h"

void enemy_new(Enemy *e)
{
	e->battle_col = col_new_circle(536, 536, 64, 0);
	e->hitstun = 0;
	e->lastatk = 0;
}

void enemy_on_hit(Enemy *e, Uint8 atk)
{
	if (!e || e->lastatk == atk)
	{
		return;
	}
	e->lastatk = atk;
}