#include <stdlib.h>
#include "simple_logger.h"
#include "my_entity.h"
#include "my_player.h"
#include "my_scene.h"

static Player p = { 0 };
static int lastdir;
static Vector2D lastmove;

static float TEMPSWAPCD = 0;

/*Overworld Sprites*/
static char *over_forward	= "images/PlayerSprites/MaribelleWalk_F.png";
static char *over_left		= "images/PlayerSprites/MaribelleWalk_L.png";
static char *over_backward  = "images/PlayerSprites/MaribelleWalk_B.png";
static char *over_right		= "images/PlayerSprites/MaribelleWalk_R.png";
static char *over_idle		= "images/PlayerSprites/MaribelleIdle.png";

/*Battle Sprites*/
static char *basic_attack = "images/PlayerSprites/basic_attack.png";
static char *sour_test = "images/PlayerSprites/sour_test.png";
static Sprite *sour;
static char *sweet_test = "images/PlayerSprites/sweet_test.png";
static Sprite *sweet;
Player *player_new()
{
	p.player_ent = entity_new();
	p.player_ent->position = vector2d(568, 328);
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

	p.player_ent->col = col_new_rect(p.player_ent->position.x, p.player_ent->position.y, 64, 64, 1);
	p.battle_col = col_new_circle(p.player_ent->position.x, p.player_ent->position.y, 32, 1);
	
	p.attack = gf2d_sprite_load_all(basic_attack, 192, 192, 10);
	p.atknum = 0;
	p.attacking = false;
	p.attackFrame = 0;
	p.attack_col = col_new_circle(0, 0, 32, 0);

	p.player_ent->scene = scene_get("all");

	sour = gf2d_sprite_load_image(sour_test);
	sweet = gf2d_sprite_load_image(sweet_test);

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
			}
			else if (A)
			{
				p.dir = 7;
			}
			else
			{
				p.dir = 0;
			}
		}
		else if (S)
		{
			p.moving = true;

			if (D)
			{
				p.dir = 3;
			}
			else if (A)
			{
				p.dir = 5;
			}
			else
			{
				p.dir = 4;
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

		player_movement_battle();
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

void player_movement_battle()
{
	float xdist = 0;
	float ydist = 0;
	Vector2D d;

	if (p.moving = false)
		return;

	if (p.dir != lastdir || p.player_ent->sprite == p.over_idle)
	{
		if (p.dir == 0 || p.dir == 1 || p.dir == 7)
		{
			p.player_ent->sprite = p.over_backward;
		}
		else if (p.dir == 6)
		{
			p.player_ent->sprite = p.over_left;
		}
		else if (p.dir == 4 || p.dir == 5 || p.dir == 3)
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
	else if (p.dir == 1)
	{
		xdist = 1;
		ydist = -1;
	}
	else if (p.dir == 6)
	{
		xdist = -1;
		ydist = 0;
	}
	else if (p.dir == 7)
	{
		xdist = -1;
		ydist = -1;
	}
	else if (p.dir == 4)
	{
		xdist = 0;
		ydist = 1;
	}
	else if (p.dir == 5)
	{
		xdist = -1;
		ydist = 1;
	}
	else if (p.dir == 2)
	{
		xdist = 1;
		ydist = 0;
	}
	else if (p.dir == 3)
	{
		xdist = 1;
		ydist = 1;
	}

	lastmove.x = xdist * p.speed;
	lastmove.y = ydist * p.speed;

	d.x = p.player_ent->position.x + (xdist * p.speed);
	d.y = p.player_ent->position.y + (ydist * p.speed);

	room_manager_swap(p.player_ent->position.x, p.player_ent->position.y);
	room_manager_scroll(lastmove);
	if (scene_get_active()->_arena == 1)
	{
		scene_arena_draw(scene_get_active(), lastmove);
	}
	lastdir = p.dir;
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

	room_manager_swap(p.player_ent->position.x, p.player_ent->position.y);
	room_manager_scroll(lastmove);
	if (scene_get_active()->_arena == 1)
	{
		scene_arena_draw(scene_get_active(), lastmove);
	}
	lastdir = p.dir;
}

void player_check_actions(Uint8 click, Uint8 space, float mx, float my)
{
	Vector2D ori, scale, offset, pos;
	scale = vector2d(4, 4);
	if (space)
	{
		if (TEMPSWAPCD >= 5)
		{
			if (!p.battle)
			{
				scene_swap("battle");
			}
			else
			{
				scene_swap("over");
			}
			player_set_battle();
			TEMPSWAPCD = 0;
		}
		else
		{
			TEMPSWAPCD += 0.1;
		}
	}
	else
	{
		TEMPSWAPCD = 0;
	}

	if (p.attackcd <= 0)
	{
		if (!p.attacking && p.battle)
		{
			if (click)
			{
				p.attacking = true;
				p.atknum++;

				ori = vector2d(p.battle_col.origin.x, p.battle_col.origin.y);
				p.atkdir = vector2d(mx - ori.x, my - ori.y);
				vector2d_normalize(&p.atkdir);

				p.atkpos = vector2d(ori.x + (p.atkdir.x * 25)-4, ori.y + (p.atkdir.y * 25)-4);
				p.atkrot = vector3d(96, 96, vector2d_angle(p.atkdir) + 90);
			}
		}
		else if (p.battle)
		{
			if (p.atknum == 3)
			{
				p.attackFrame += 0.15;
			}
			else
			{
				p.attackFrame += 0.1;
			}

			if (p.atknum == 1)
			{
				if (p.attackFrame >= 3)
				{
					p.attacking = false;
					return;
				}
			}
			else if (p.atknum == 2)
			{
				if (p.attackFrame >= 6)
				{
					p.attacking = false;
					return;
				}
			}
			
			pos = vector2d(p.atkpos.x - 96, p.atkpos.y - 96);
			gf2d_sprite_draw(p.attack, pos, NULL, NULL, &p.atkrot, NULL, NULL, p.attackFrame);

			gf2d_sprite_draw_image(sweet, p.atkpos);

			//TEST
			if (p.attackFrame >= 1 && p.attackFrame < 2)
			{
				p.attack_col.origin.x = p.atkpos.x + (p.atkdir.x * 25);
				p.attack_col.origin.y = p.atkpos.y + (p.atkdir.y * 25);
				entity_check_hits(p.atknum, &p.attack_col);
				gf2d_sprite_draw_image(sour, vector2d(p.atkpos.x + (p.atkdir.x * 25) - 32, p.atkpos.y + (p.atkdir.y * 25) - 32));
			}

			if (p.attackFrame >= 3 && p.attackFrame < 4)
			{
				p.attack_col.origin.x = p.atkpos.x + (p.atkdir.x * 25);
				p.attack_col.origin.y = p.atkpos.y + (p.atkdir.y * 25);
				entity_check_hits(p.atknum, &p.attack_col);
				gf2d_sprite_draw_image(sour, vector2d(p.atkpos.x + (p.atkdir.x * 25) - 32, p.atkpos.y + (p.atkdir.y * 25) - 32));
			}

			if (p.attackFrame >= 6 && p.attackFrame < 8)
			{
				p.attack_col.origin.x = p.atkpos.x + (p.atkdir.x * 25);
				p.attack_col.origin.y = p.atkpos.y + (p.atkdir.y * 25);
				entity_check_hits(p.atknum, &p.attack_col);
				gf2d_sprite_draw_image(sour, vector2d(p.atkpos.x + (p.atkdir.x * 25) - 32, p.atkpos.y + (p.atkdir.y * 25) - 32));
			}

			if (p.atknum == 3 && p.attackFrame >= 10)
			{
				p.attacking = false;
				p.atknum = 0;
				p.attackFrame = 0;
				p.attackcd = 3;
			}
		}
	}
	else
	{
		p.attackcd -= 0.1;
	}
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

RectCol *player_get_rect()
{
	return &p.player_ent->col;
}

CirCol *player_get_circle()
{
	return &p.battle_col;
}

Vector2D player_get_last()
{
	if (p.moving == false)
	{
		return vector2d(0, 0);
	}
	return lastmove;
}

void player_set_battle()
{
	p.battle = !p.battle;
}