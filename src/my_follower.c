#include "my_follower.h"
#include "my_player.h"
#include "simple_logger.h"
#include "my_entity.h"

Follower djinn;
Follower leviathan;
Follower leshen;
Follower basilisk;
Follower griffin;
Follower golemn;

static char *djinn_sprite = "images/FollowerSprites/Djinn.png";
static char *leviathan_sprite = "images/FollowerSprites/Leviathan.png";
static char *leshen_sprite = "images/FollowerSprites/Leshen.png";
static char *basilisk_sprite = "images/FollowerSprites/Basilisk.png";
static char *griffin_sprite = "images/FollowerSprites/Griffin.png";
static char *golemn_sprite = "images/FollowerSprites/Golemn.png";

static char *projectile_sprite = "images/FollowerSprites/projectile.png";

void follower_new(Follower *f, Uint8 id)
{
	if (f->set)
		return;

	switch (id)
	{
		case 0:
			//DJINN
			f->id = 0;
			f->health_max = 5;
			f->health = 5;
			f->speed = 4;
			f->attack = 5;
			f->magic = 3;
			f->defense = 2;
			f->sprite = gf2d_sprite_load_image(djinn_sprite);
			f->mode = 0;
			f->name = "Djinn";
		break;
		case 1:
			//LEVIATHAN
			f->id = 1;
			f->health_max = 7;
			f->health = 7;
			f->speed = 2;
			f->attack = 2;
			f->magic = 6;
			f->defense = 4;
			f->sprite = gf2d_sprite_load_image(leviathan_sprite);
			f->mode = 0;
			f->name = "Leviathan";
		break;
		case 2:
			//LESHEN
			f->id = 2;
			f->health_max = 5;
			f->health = 5;
			f->speed = 3;
			f->attack = 4;
			f->magic = 4;
			f->defense = 4;
			f->sprite = gf2d_sprite_load_image(leshen_sprite);
			f->mode = 0;
			f->name = "Leshen";
		break;
		case 3:
			//BASILISK
			f->id = 3;
			f->health_max = 3;
			f->health = 3;
			f->speed = 5;
			f->attack = 7;
			f->magic = 3;
			f->defense = 1;
			f->sprite = gf2d_sprite_load_image(basilisk_sprite);
			f->mode = 0;
			f->name = "Basilisk";
		break;
		case 4:
			//Wisp
			f->id = 4;
			f->health_max = 4;
			f->health = 4;
			f->speed = 4;
			f->attack = 5;
			f->magic = 3;
			f->defense = 2;
			f->sprite = gf2d_sprite_load_image(griffin_sprite);
			f->mode = 0;
			f->name = "Wisp";
		break;
		case 5:
			//GOLEMN
			f->id = 5;
			f->health_max = 10;
			f->health = 10;
			f->speed = 2;
			f->attack = 5;
			f->magic = 1;
			f->defense = 5;
			f->sprite = gf2d_sprite_load_image(golemn_sprite);
			f->mode = 0;
			f->name = "Golemn";
		break;
	}

	f->col = col_new_circle(0, 0, 24, 1);

	f->target = entity_get_closest(f->pos);

	f->ranged.sprite = gf2d_sprite_load_image(projectile_sprite);
	f->ranged.knockback = vector2d(1, 1);
	f->ranged.force = 15;
	f->ranged.stun = 1;
	f->ranged.damage = 1 + (int)(f->magic / 5);
	f->scene = scene_get("battle");

	f->set = true;
}

void follower_update(void *player, Follower *f)
{
	Player *p;
	p = (Player *)player;
	Vector2D trackpos;
	float dist;

	if (!f)
		return;

	if (f->mode == 0 || f->mode == 1)
	{
		trackpos.x = p->player_ent->position.x;
		trackpos.y = p->player_ent->position.y;

		f->forcedir.x = (trackpos.x + 32) - (f->pos.x + 24);
		f->forcedir.y = (trackpos.y + 32) - (f->pos.y + 24);

		f->forcedir.x += p->trackdir.x * 75;
		f->forcedir.y += p->trackdir.y * 75 + f->bounce;
	}
	else
	{
		trackpos.x = f->target.x;
		trackpos.y = f->target.y;

		f->forcedir.x = (trackpos.x + 32) - (f->pos.x + 24);
		f->forcedir.y = (trackpos.y + 32) - (f->pos.y + 24);
	}

	dist = SDL_sqrtf(SDL_powf(f->forcedir.x, 2) + SDL_powf(f->forcedir.y, 2));
	
	if (dist >= 2)
	{
		vector2d_normalize(&f->forcedir);
		f->forcemag = f->speed;
		f->moving = true;
	}
	else
	{
		f->forcedir = vector2d(0, 0);
		f->forcemag = 0;
		f->moving = false;
	}

	if (f->mode == 1 || f->mode == 2)
	{
		follower_check_attack(f);
	}

	if (f->moving)
	{
		f->pos.x += (f->forcedir.x * f->forcemag);
		f->pos.y += (f->forcedir.y * f->forcemag);
	}
	if (!f->bflip)
	{
		f->bounce += 0.25;
		if (f->bounce >= 7)
		{
			f->bflip = 1;
		}
	}
	else
	{
		f->bounce -= 0.25;
		if (f->bounce <= -7)
		{
			f->bflip = 0;
		}
	}

	f->col.origin.x = f->pos.x + 24;
	f->col.origin.y = f->pos.y + 24;

	follower_update_attack(f);
}

void follower_draw(Follower *f)
{
	if (!f)
		return;
	if (f->scene->_active)
	{
		gf2d_sprite_draw_image(f->sprite, f->pos);
		if (f->shot_out)
			gf2d_sprite_draw_image(f->ranged.sprite, f->ranged.position);
	}
}

void follower_check_attack(Follower *f)
{
	if (!f)
		return;
	f->target = entity_get_closest(f->pos);
	if (!f->shot_out && f->cooldown <= 0)
	{
		f->ranged.position = f->pos;
		f->ranged.speed = f->speed;
		f->ranged.launch.x = f->target.x - f->pos.x + 64;
		f->ranged.launch.y = f->target.y - f->pos.y + 64;
		vector2d_normalize(&f->ranged.launch);
		f->ranged.col = col_new_circle(f->pos.x, f->pos.y, 8, 8);
		f->ranged.direction = f->ranged.launch;
		f->ranged.reset = true;
		f->shot_out = true;
	}
	else
	{
		f->cooldown -= 0.1;
	}
}

void follower_update_attack(Follower *f)
{
	if (!f)
		return;
	if (!f->shot_out)
	{
		return;
	}
	f->ranged.position.x += (f->ranged.launch.x * f->ranged.speed);
	f->ranged.position.y += (f->ranged.launch.y * f->ranged.speed);
	f->ranged.col.origin.x += (f->ranged.launch.x * f->ranged.speed);
	f->ranged.col.origin.y += (f->ranged.launch.y * f->ranged.speed);

	if (col_battle_bounds(scene_get("battle"), f->ranged.col.origin) || entity_check_hits(&f->ranged))
	{
		f->shot_out = false;
		f->cooldown = 30 / f->speed;
	}
}

void follower_scroll(Follower *f, Vector2D movement)
{
	if (!f)
		return;
	f->pos.x -= movement.x;
	f->pos.y -= movement.y;
	f->col.origin.x -= movement.x;
	f->col.origin.y -= movement.y;
	if (f->shot_out)
	{
		f->ranged.position.x -= movement.x;
		f->ranged.position.y -= movement.y;
		f->ranged.col.origin.x -= movement.x;
		f->ranged.col.origin.y -= movement.y;
	}
}

void follower_free(Follower *f)
{
	if (!f)
		return;
	gf2d_sprite_free(f->sprite);
	memset(f, 0, sizeof(Follower));
}