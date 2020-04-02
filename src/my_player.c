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

/*Followers*/
static Follower follower1;
static Follower follower2;
static Follower follower3;
static Follower follower4;

Player *player_new()
{
	int x;
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

	p.player_ent->col = col_new_rect(p.player_ent->position.x+3, p.player_ent->position.y+3, 58, 58, 1);
	p.battle_col = col_new_circle(p.player_ent->position.x, p.player_ent->position.y, 32, 1);
	
	//TODO Switch to config file
	p.atk[0].sprite = gf2d_sprite_load_all(basic_attack, 192, 192, 10);
	p.atk[0].index = 0;
	p.atk[0].frame = 0;
	p.atk[0].start = 0;
	p.atk[0].end = 3;
	p.atk[0].active_s = 1;
	p.atk[0].active_e = 2;
	p.atk[0].col = col_new_circle(0, 0, 16, 0);
	p.atk[0].knockback = vector2d(-1, -1);
	p.atk[0].force = 3;

	p.atk[1].sprite = gf2d_sprite_load_all(basic_attack, 192, 192, 10);
	p.atk[1].index = 1;
	p.atk[1].frame = 0;
	p.atk[1].start = 3;
	p.atk[1].end = 6;
	p.atk[1].active_s = 4;
	p.atk[1].active_e = 5;
	p.atk[1].col = col_new_circle(0, 0, 16, 0);
	p.atk[1].knockback = vector2d(-1, -1);
	p.atk[1].force = 3;

	p.atk[2].sprite = gf2d_sprite_load_all(basic_attack, 192, 192, 10);
	p.atk[2].index = 2;
	p.atk[2].frame = 0;
	p.atk[2].start = 6;
	p.atk[2].end = 10;
	p.atk[2].active_s = 7;
	p.atk[2].active_e = 9;
	p.atk[2].col = col_new_circle(0, 0, 16, 0);
	p.atk[2].knockback = vector2d(1, 1);
	p.atk[2].force = 25;

	follower_new(&follower1, 0);
	follower_new(&follower2, 1);
	follower_new(&follower3, 2);
	//follower_new(&follower4, 3);

	follower1.pos = p.player_ent->position;
	follower2.pos = p.player_ent->position;
	follower3.pos = p.player_ent->position;
	follower4.pos = p.player_ent->position;

	//p.active = &follower1;

	p.attack = gf2d_sprite_load_all(basic_attack, 192, 192, 10);

	p.atknum = -1;
	p.attacking = false;

	p.player_ent->scene = scene_get("all");

	p.trackdir = vector2d(0, 1);

	p.team[0] = &follower1;
	p.team[1] = &follower2;
	p.team[2] = &follower3;
	p.team[3] = &follower4;
	p.team[0]->health -= 3;

	for (x = 0; x < 5; x++)
	{
		item_new(&p.items[x], x);
	}
	p.item_1 = 0;
	p.item_2 = 4;

	p.grounded = true;

	slog("Created new player & player entity.");
	return &p;
}

void player_check_movement(Uint8 W, Uint8 A, Uint8 S, Uint8 D)
{
	if (!p.grounded)
	{
		p.moving = false;
		return;
	}

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

		if (p.battle)
		{
			follower_update(&p, p.team[0]);
		}

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

		if (p.battle)
		{
			follower_update(&p, p.team[0]);
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

	if (p.moving == false)
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

	p.trackdir = vector2d(-1 * xdist, -1 * ydist);

	lastmove.x = xdist * (p.speed);
	lastmove.y = ydist * (p.speed);

	follower_scroll(p.team[0], lastmove);

	lastdir = p.dir;
}

void player_movement_overworld()
{
	float xdist = 0;
	float ydist = 0;

	if (p.moving == false)
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

	room_manager_swap(p.player_ent->position.x, p.player_ent->position.y);
	room_manager_scroll(lastmove);
	if (scene_get_active()->_arena == 1)
	{
		scene_arena_draw(scene_get_active(), lastmove);
	}

	lastdir = p.dir;
}

void player_check_actions(Uint8 left_click, Uint8 right_click, Uint8 space, float mx, float my, Uint8 num, Uint8 tab, Uint8 q, Uint8 e)
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

		if (p.can_interact)
		{
			p.interact->on_interact(&p);
			player_can_interact(false, NULL);
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
			if (left_click && p.can_atk)
			{
				p.attacking = true;
				p.atknum++;

				p.atk[p.atknum].frame = p.atk[p.atknum].start;

				ori = vector2d(p.battle_col.origin.x, p.battle_col.origin.y);
				p.atk[p.atknum].direction = vector2d(mx - ori.x, my - ori.y);
				vector2d_normalize(&p.atk[p.atknum].direction);

				p.atk[p.atknum].position = vector2d(ori.x + (p.atk[p.atknum].direction.x * 25) - 4,
													ori.y + (p.atk[p.atknum].direction.y * 25) - 4);
				p.atk[p.atknum].rotation = vector3d(96, 96, vector2d_angle(p.atk[p.atknum].direction) + 90);

				p.atk[p.atknum].reset = true;
			}
		}
		else if (p.battle)
		{
			if (p.atknum == 2)
			{
				p.atk[p.atknum].frame += 0.15;
			}
			else
			{
				p.atk[p.atknum].frame += 0.1;
			}

			if (p.atknum == 0)
			{
				if (p.atk[p.atknum].frame >= p.atk[p.atknum].end)
				{
					p.attacking = false;
					return;
				}
			}
			else if (p.atknum == 1)
			{
				if (p.atk[p.atknum].frame >= p.atk[p.atknum].end)
				{
					p.attacking = false;
					return;
				}
			}

			pos = vector2d(p.atk[p.atknum].position.x - 96, p.atk[p.atknum].position.y - 96);
			gf2d_sprite_draw(p.atk[p.atknum].sprite, pos, NULL, NULL, &p.atk[p.atknum].rotation, NULL, NULL, p.atk[p.atknum].frame);

			//TEST
			if (p.atk[p.atknum].frame >= p.atk[p.atknum].active_s && p.atk[p.atknum].frame < p.atk[p.atknum].active_e)
			{
				p.atk[p.atknum].col.origin.x = p.atk[p.atknum].position.x + (p.atk[p.atknum].direction.x * 25);
				p.atk[p.atknum].col.origin.y = p.atk[p.atknum].position.y + (p.atk[p.atknum].direction.y * 25);
				entity_check_hits(&p.atk[p.atknum]);
			}

			if (p.atk[p.atknum].frame >= p.atk[p.atknum].active_s && p.atk[p.atknum].frame < p.atk[p.atknum].active_e)
			{
				p.atk[p.atknum].col.origin.x = p.atk[p.atknum].position.x + (p.atk[p.atknum].direction.x * 25);
				p.atk[p.atknum].col.origin.y = p.atk[p.atknum].position.y + (p.atk[p.atknum].direction.y * 25);
				entity_check_hits(&p.atk[p.atknum]);
			}

			if (p.atk[p.atknum].frame >= p.atk[p.atknum].active_s && p.atk[p.atknum].frame < p.atk[p.atknum].active_e)
			{
				p.atk[p.atknum].col.origin.x = p.atk[p.atknum].position.x + (p.atk[p.atknum].direction.x * 25);
				p.atk[p.atknum].col.origin.y = p.atk[p.atknum].position.y + (p.atk[p.atknum].direction.y * 25);
				entity_check_hits(&p.atk[p.atknum]);
			}

			if (p.atknum == 2 && p.atk[p.atknum].frame >= p.atk[p.atknum].end)
			{
				p.attacking = false;
				p.atknum = -1;
				p.atk[p.atknum].frame = 0;
				p.attackcd = 3;
			}
		}
	}
	else
	{
		p.attackcd -= 0.1;
	}

	if (p.attacking)
	{
		p.speed = 1;
	}
	else
	{
		p.speed = 3;
	}

	//w == x; h == y; x == z; y == w
	if (mx > p.window.z &&
		mx < p.window.x + p.window.z &&
		my > p.window.w &&
		my < p.window.w + p.window.y)
	{
		if (my < p.window.w + 48)
		{
			p.selected = 1;
		}
		else if (my < p.window.w + 96)
		{
			p.selected = 2;
		}
		else if (my < p.window.w + 144)
		{
			p.selected = 3;
		}
		else if (my < p.window.w + 192)
		{
			p.selected = 4;
		}

		if (right_click)
		{
			p.show_status = true;
		}
		else
		{
			p.show_status = false;
		}

		p.can_atk = false;
	}
	else
	{
		p.selected = 0;
		p.picked = 0;
		p.follower_cd = 0;

		p.show_status = false;

		p.can_atk = true;
	}

	if (left_click && p.selected)
	{
		if (p.picked && p.follower_cd <= 0)
		{
			player_swap_follower(p.picked, p.selected);
			p.picked = 0;
			p.follower_cd = 1;
		}
		else
		{
			p.picked = p.selected;
		}
	}

	p.follower_cd -= 0.1;

	if (num && p.battle)
	{
		p.team[0]->mode = num - 1;
	}

	if (p.item_swapcd <= 0 && tab)
	{
		if (p.item_1 < 3)
		{
			p.item_1++;
		}
		else
		{
			p.item_1 = 0;
		}
		p.item_swapcd = 2;
	}
	else
	{
		p.item_swapcd -= 0.1;
	}

	if (q && !p.item_out)
	{
		p.item_out = p.item_1 + 1;
		p.items[p.item_1].use(&p);
	}

	if (e && !p.item_out)
	{
		p.item_out = p.item_2 + 1;
		p.items[p.item_2].use(&p);
	}

	p.mouse = vector2d(mx, my);

	if (p.item_out)
	{
		p.items[p.item_out - 1].update(&p.items[p.item_out-1], &p);
		if (p.moving)
		{
			item_scroll(&p.items[p.item_out - 1], lastmove);
		}
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

void player_draw_follower()
{
	follower_draw(p.team[0]);
}

void player_swap_follower(Uint8 slot1, Uint8 slot2)
{
	Follower *swap;

	swap = p.team[slot1 - 1];
	p.team[slot1 - 1] = p.team[slot2-1];
	p.team[slot2-1] = swap;

	p.team[0]->pos = p.player_ent->position;
	p.team[0]->shot_out = false;
}

void player_can_interact(Uint8 b, Interactable *i)
{
	p.can_interact = b;
	p.interact = i;
}

void player_shuffle_followers()
{
	follower_free(&follower1);
	srand(time(0) * 1);
	follower_new(&follower1, rand() % 6);
	follower_free(&follower2);
	srand(time(0) * 2);
	follower_new(&follower2, rand() % 6);
	follower_free(&follower3);
	srand(time(0) * 3);
	follower_new(&follower3, rand() % 6);
	follower_free(&follower4);
	srand(time(0) * 4);
	follower_new(&follower4, rand() % 6);
}

void player_clear_interact()
{
	interactable_clear(p.interact);
	p.can_interact = false;
}

Vector2D player_get_interact_pos()
{
	return interactable_get_pos(p.interact);
}

void player_follower_new()
{
	int x = 0;
	float r;
	for (x = 0; x < 4; x++)
	{
		if (!p.team[x]->set)
		{
			srand((unsigned)time(0) % 1000 * p.player_ent->position.x * p.player_ent->position.y);
			r = rand();
			r /= RAND_MAX;
			r *= 100;
			if (r < 16.67)
			{
				follower_new(p.team[x], 0);
			}
			else if (r < 33.34)
			{
				follower_new(p.team[x], 1);
			}
			else if (r < 50.01)
			{
				follower_new(p.team[x], 2);
			}
			else if (r < 66.68)
			{
				follower_new(p.team[x], 3);
			}
			else if (r < 83.85)
			{
				follower_new(p.team[x], 4);
			}
			else if (r < 100)
			{
				follower_new(p.team[x], 5);
			}
			return;
		}
	}
}

void player_restock()
{
	int x = 0;
	for (x = 0; x < 5; x++)
	{
		p.items[x].count = 3;
	}
}