#include <stdlib.h>
#include "simple_logger.h"
#include "my_entity.h"
#include "my_scene_manager.h"
#include "my_player.h"

static Player p = { 0 };
static int lastdir;
static Vector2D lastmove;

/*Overworld Sprites*/
static char *over_forward	= "images/PlayerSprites/MaribelleWalk_F.png";
static char *over_left		= "images/PlayerSprites/MaribelleWalk_L.png";
static char *over_backward  = "images/PlayerSprites/MaribelleWalk_B.png";
static char *over_right		= "images/PlayerSprites/MaribelleWalk_R.png";
static char *over_idle		= "images/PlayerSprites/MaribelleIdle.png";

/*Battle Sprites*/
Player *player_new()
{
	p.player_ent = entity_new();
	p.player_ent->position = vector2d(256, 256);
	if (p.player_ent == NULL)
	{
		slog("could not create player entity!");
		return NULL;
	}

	p.battle = false;
	p.moving = false;
	p.dir = 4;
	p.speed = 3;

	p.over_forward = gf2d_sprite_load_all(over_forward, 64, 64, 4);
	p.over_left = gf2d_sprite_load_all(over_left, 64, 64, 4);
	p.over_backward = gf2d_sprite_load_all(over_backward, 64, 64, 4);
	p.over_right = gf2d_sprite_load_all(over_right, 64, 64, 4);
	p.over_idle = gf2d_sprite_load_all(over_idle, 64, 64, 4);

	p.player_ent->sprite = p.over_forward;
	p.player_ent->fpl = 4;

	p.player_ent->col = col_new_rect(p.player_ent->position.x, p.player_ent->position.y, 64, 64, 0);
	
	p.player_ent->scene = scene_by_name("all");

	slog("Created new player & player entity.");
	return &p;
}

void player_check_movement(Uint8 W, Uint8 A, Uint8 S, Uint8 D)
{
	//Quick escape checks
	if (W && S)
	{
		p.moving = false;
		return;
	}
	if (A && D)
	{
		p.moving = false;
		return;
	}
	if (!W && !S && !A && !D)
	{
		p.moving = false;
		p.player_ent->sprite = p.over_idle;
		p.player_ent->frame = lastdir / 2;
		return;
	}

	if (p.battle)
	{
		if (W)
		{
			p.moving = true;

			if (D)
			{
				p.dir = 1;
				return;
			}
			else if (A)
			{
				p.dir = 7;
				return;
			}
			else
			{
				p.dir = 0;
				return;
			}
		}
		else if (S)
		{
			p.moving = true;

			if (D)
			{
				p.dir = 3;
				return;
			}
			else if (A)
			{
				p.dir = 5;
				return;
			}
			else
			{
				p.dir = 4;
				return;
			}
		}
		else
		{
			if (D)
			{
				p.moving = true;
				p.dir = 2;
			}
			else if (A)
			{
				p.moving = true;
				p.dir = 6;
			}
		}

		//player_movement_battle();
	}
	else
	{
		if (W)
		{
			p.moving = true;
			p.dir = 0;
		}
		else if (A)
		{
			p.moving = true;
			p.dir = 6;
		}
		else if (S)
		{
			p.moving = true;
			p.dir = 4;
		}
		else if (D)
		{
			p.moving = true;
			p.dir = 2;
		}
		else
		{
			p.moving = false;
		}

		player_movement_overworld();
	}
}

void player_movement_overworld()
{
	float xdist = 0;
	float ydist = 0;
	Vector2D d;

	if (p.moving = false)
		return;

	if (p.dir != lastdir || p.player_ent->sprite == p.over_idle)
	{
		if (p.dir == 0)
		{
			p.player_ent->sprite = p.over_backward;
		}
		else if (p.dir == 6)
		{
			p.player_ent->sprite = p.over_left;
		}
		else if (p.dir == 4)
		{
			p.player_ent->sprite = p.over_forward;
		}
		else if (p.dir == 2)
		{
			p.player_ent->sprite = p.over_right;
		}
	}

	if (p.dir == 0)
	{
		xdist = 0;
		ydist = -1;
	}
	else if (p.dir == 6)
	{
		xdist = -1;
		ydist = 0;
	}
	else if (p.dir == 4)
	{
		xdist = 0;
		ydist = 1;
	}
	else if (p.dir == 2)
	{
		xdist = 1;
		ydist = 0;
	}

	lastmove.x = xdist * p.speed;
	lastmove.y = ydist * p.speed;

	d.x = p.player_ent->position.x + (xdist * p.speed);
	d.y = p.player_ent->position.y + (ydist * p.speed);
	//p.player_ent->position = d;
	//p.player_ent->col.origin = d;

	room_scroll(p.current, lastmove);

	lastdir = p.dir;
}

void player_free(Player *self)
{
	if (!self)return;
	gf2d_sprite_free(self->player_ent->sprite);
	gf2d_sprite_free(self->over_backward);
	gf2d_sprite_free(self->over_forward);
	gf2d_sprite_free(self->over_idle);
	gf2d_sprite_free(self->over_left);
	gf2d_sprite_free(self->over_right);
	memset(self, 0, sizeof(Entity));

	slog("Freed player");
}

void player_check_col(RectCol *other)
{
	if (col_rect_rect(&p.player_ent->col, other) && other->solid == 1)
	{
		slog("Colliding!");
		/*p.player_ent->position.x -= lastmove.x;
		p.player_ent->position.y -= lastmove.y;*/
		lastmove.x = 0;
		lastmove.y = 0;
	}
}

RectCol *player_get_rect()
{
	return &p.player_ent->col;
}

void player_set_room(Room *rm)
{
	p.current = rm;
}