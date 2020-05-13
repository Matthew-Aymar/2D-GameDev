#include "my_enemy.h"
#include "simple_logger.h"
#include "my_entity.h"
#include "my_player.h"

static char *projectile_sprite = "images/FollowerSprites/projectile.png";

static char *djinn_sprite = "images/FollowerSprites/Djinn.png";
static char *leviathan_sprite = "images/FollowerSprites/Leviathan.png";
static char *leshen_sprite = "images/FollowerSprites/Leshen.png";
static char *basilisk_sprite = "images/FollowerSprites/Basilisk.png";
static char *griffin_sprite = "images/FollowerSprites/Griffin.png";
static char *golemn_sprite = "images/FollowerSprites/Golemn.png";

static char *djinn_boss = "images/FollowerSprites/Djinnboss.png";
static char *leviathan_boss = "images/FollowerSprites/Leviathanboss.png";

void enemy_new(Enemy *e)
{
	float r;
	e->hitstun = 0;
	e->lastatk = -1;
	e->speed = 2;
	e->state = 1;
	e->tracking = 2;
	e->lastr = 1;

	e->ranged.sprite = gf2d_sprite_load_image(projectile_sprite);
	e->ranged.knockback = vector2d(1, 1);
	e->ranged.force = 15;
	e->ranged.stun = 1;

	if (e->boss)
	{
		e->battle_col = col_new_circle(552, 552, 48, 0);
		e->speed = 2.5;

		e->ranged.damage = 2;

		e->health_max = 50;
		e->health = 50;

		if (e->boss == 1)
			e->sprite = gf2d_sprite_load_image(djinn_boss);
		else
			e->sprite = gf2d_sprite_load_image(leviathan_boss);

		e->id = 6;

		return;
	}

	e->battle_col = col_new_circle(536, 536, 32, 0);

	e->ranged.damage = 1;

	e->health_max = 25;
	e->health = 25;

	srand((unsigned)time(0) % 1000 * e->lastr * player_position().x * player_position().y);
	r = rand();
	r /= RAND_MAX;
	r *= 100;
	e->lastr = r;

	if (r < 16.67)
	{
		e->sprite = gf2d_sprite_load_image(djinn_sprite);
		e->id = 0;
	}
	else if (r < 33.34)
	{
		e->sprite = gf2d_sprite_load_image(leviathan_sprite);
		e->id = 1;
	}
	else if (r < 50.01)
	{
		e->sprite = gf2d_sprite_load_image(leshen_sprite);
		e->id = 2;
	}
	else if (r < 66.68)
	{
		e->sprite = gf2d_sprite_load_image(basilisk_sprite);
		e->id = 3;
	}
	else if (r < 83.85)
	{
		e->sprite = gf2d_sprite_load_image(griffin_sprite);
		e->id = 4;
	}
	else if (r < 100)
	{
		e->sprite = gf2d_sprite_load_image(golemn_sprite);
		e->id = 5;
	}
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
	entity->enm.hitstun = atk->stun;
	entity->enm.health -= atk->damage;

	if (entity->enm.health <= 0)
	{
		scene_swap("over");
		player_set_battle();
	}
 }

void enemy_on_col(void *ent, Item *item)
{
	Vector2D diff;
	float dist;
	Entity *entity;
	entity = (Entity *)ent;

	if (item->id == 3)
	{
		diff = vector2d(entity->position.x + 32 - item->trackpos.x - 16, entity->position.y + 32 - item->trackpos.y - 16);
		dist = SDL_sqrtf(SDL_powf(diff.x, 2) + SDL_powf(diff.y, 2));
		entity->forcedir.x = -diff.x;
		entity->forcedir.y = -diff.y;
		vector2d_normalize(&entity->forcedir);
		entity->forcemag = dist;
		entity->accel = -(30.75 * (dist / 60));
		entity->enm.hitstun = 5;
	}
	else
	{
		entity->forcedir.x = entity->position.x + 32 - item->pos.x;
		entity->forcedir.y = entity->position.y + 32 - item->pos.y;
		vector2d_normalize(&entity->forcedir);
		entity->forcemag = item->force;
		entity->accel = -(item->force * 0.34);
		entity->enm.hitstun = 15;
	}
}

void enemy_check_attack(void *ent)
{
	Entity *entity;
	entity = (Entity *)ent;
	if (!entity)
		return;
	if (!entity->enm.shot_out && entity->enm.cooldown <= 0)
	{
		if (player_get_boss())
		{
			entity->enm.ranged.position.x = entity->position.x + 48;
			entity->enm.ranged.position.y = entity->position.y + 48;
		}
		else
		{
			entity->enm.ranged.position.x = entity->position.x + 24;
			entity->enm.ranged.position.y = entity->position.y + 24;
		}
		entity->enm.ranged.speed = entity->enm.speed;
		entity->enm.ranged.launch.x = entity->enm.target.x - entity->position.x;
		entity->enm.ranged.launch.y = entity->enm.target.y - entity->position.y;
		vector2d_normalize(&entity->enm.ranged.launch);
		if (player_get_boss())
		{
			entity->enm.ranged.col = col_new_circle(entity->position.x + 48, entity->position.y + 48, 8, 8);
		}
		else
		{
			entity->enm.ranged.col = col_new_circle(entity->position.x + 24, entity->position.y + 24, 8, 8);
		}
		
		entity->enm.ranged.direction = entity->enm.ranged.launch;
		entity->enm.ranged.reset = true;
		entity->enm.shot_out = true;
	}
	else
	{
		entity->enm.cooldown -= 0.1;
	}
}

void enemy_update_attack(void *ent)
{
	Entity *entity;
	entity = (Entity *)ent;
	if (!entity)
		return;
	if (!entity->enm.shot_out)
	{
		return;
	}
	entity->enm.ranged.position.x += (entity->enm.ranged.launch.x * entity->enm.ranged.speed);
	entity->enm.ranged.position.y += (entity->enm.ranged.launch.y * entity->enm.ranged.speed);
	entity->enm.ranged.col.origin.x += (entity->enm.ranged.launch.x * entity->enm.ranged.speed);
	entity->enm.ranged.col.origin.y += (entity->enm.ranged.launch.y * entity->enm.ranged.speed);

	if (col_battle_bounds(scene_get("battle"), entity->enm.ranged.col.origin))
	{
		entity->enm.shot_out = false;
		entity->enm.cooldown = 30 / entity->enm.speed;
	}

	if (col_circle_circle(&entity->enm.ranged.col, player_get_circle()))
	{
		player_damage(entity->enm.ranged.damage);
		entity->enm.shot_out = false;
		entity->enm.cooldown = 30 / entity->enm.speed;
	}

	if (col_circle_circle(&entity->enm.ranged.col, player_get_follower_circle()))
	{
		player_follower_damage(entity->enm.ranged.damage);
		entity->enm.shot_out = false;
		entity->enm.cooldown = 30 / entity->enm.speed;
	}
}