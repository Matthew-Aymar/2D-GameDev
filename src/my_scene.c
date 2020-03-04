#include "my_scene.h"
#include "simple_logger.h"
#include "my_player.h"

static Scene over	= { 0 }; //overworld scene
static Scene battle = { 0 }; //battle scene
static Scene all	= { 0 }; //Just for objects that cross scenes

static Sprite *arena;
static Sprite *arena_rim;

static Entity *temp;
void scene_init()
{
	over.background = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
	over._active = 1;
	over._anim	 = 0;
	over._arena  = 0;
	over.origin  = vector2d(0, 0);

	battle.background = gf2d_sprite_load_all("images/backgrounds/bg_battle.png", 1200, 700, 14);
	battle._active	= 0;
	battle._anim	= 1;
	battle._arena	= 1;
	battle.origin   = vector2d(0, 0);

	arena = gf2d_sprite_load_image("images/backgrounds/Arena.png");
	arena_rim = gf2d_sprite_load_image("images/backgrounds/ArenaRim.png");

	all._active = 1;
}

int scene_arena_update(Scene *s)
{
	Vector2D p0, p1, p2, p3, p4, v0, v1, v2, temp, POINT;
	/*
			p1
	p2				p4
			p3

	v0 is the vector of the edge to check
	v1 is the perpendicular of that vector
	*/
	float mag1, mag2, x, y, dot1, dot2;
	Uint8 check1, check2, check3, check4;

	p0 = vector2d(player_get_circle()->origin.x, player_get_circle()->origin.y);
	p1 = vector2d(s->origin.x + 600, s->origin.y);
	p2 = vector2d(s->origin.x, s->origin.y + 600);
	p3 = vector2d(s->origin.x + 600, s->origin.y + 1200);
	p4 = vector2d(s->origin.x + 1200, s->origin.y + 600);

	//Very much a hack section, TODO: remove hard coding

	if (p0.x >= p1.x && p0.y >= p4.y - 64)
	{
		p1.x -= 132;
		p1.y -= 268;
		p4.x -= 48;
		p4.y -= 48;
		//v0 = p1 - p4
		v0 = vector2d(p1.x - p4.x, p1.y - p4.y);
		//v1 = (-v0.y, v0.x)
		v1 = vector2d(-(v0.y), v0.x);

		mag1 = v0.y / v0.x;
		mag2 = v1.y / v1.x;

		x = (-(p0.x * mag2) + p0.y + (p1.x * mag1) - p1.y) / (mag1 - mag2);
		y = mag1 * (x - p1.x) + p1.y;
		POINT = vector2d(x, y);

		dot1 = vector2d_dot_product(vector2d(POINT.x - p4.x, POINT.y - p4.y), v0);
		dot2 = vector2d_dot_product(vector2d(POINT.x - p4.x, POINT.y - p4.y), vector2d(POINT.x - p4.x, POINT.y - p4.y));
		if (dot1 >= 0 && dot1 >= dot2)
		{
			check4 = true;
			return 1;
		}
		else
		{
			check4 = false;
			return 0;
		}
	}
	else if (p0.x < p1.x && p0.y >= p2.y - 64)
	{
		p3.y -= 168;
		p4.x -= 232;
		//v0 = p4 - p3
		v0 = vector2d(p4.x - p3.x, p4.y - p3.y);
		//v1 = (-v0.y, v0.x)
		v1 = vector2d(-(v0.y), v0.x);

		mag1 = v0.y / v0.x;
		mag2 = v1.y / v1.x;

		x = (-(p0.x * mag2) + p0.y + (p4.x * mag1) - p4.y) / (mag1 - mag2);
		y = mag1 * (x - p4.x) + p4.y;
		POINT = vector2d(x, y);

		dot1 = vector2d_dot_product(vector2d(POINT.x - p3.x, POINT.y - p3.y), v0);
		dot2 = vector2d_dot_product(vector2d(POINT.x - p3.x, POINT.y - p3.y), vector2d(POINT.x - p3.x, POINT.y - p3.y));
		if (dot1 >= 0 && dot1 >= dot2)
		{
			check3 = true;
			return 1;
		}
		else
		{
			check3 = false;
			return 0;
		}
	}
	else if (p0.x < p1.x && p0.y < p2.y)
	{
		p3.y += 132;
		p4.y -= 132;
		//v0 = p3 - p2
		v0 = vector2d(p3.x - p2.x, p3.y - p2.y);
		//v1 = (-v0.y, v0.x)
		v1 = vector2d(-(v0.y), v0.x);

		mag1 = v0.y / v0.x;
		mag2 = v1.y / v1.x;

		x = (-(p0.x * mag2) + p0.y + (p3.x * mag1) - p3.y) / (mag1 - mag2);
		y = mag1 * (x - p3.x) + p3.y;
		POINT = vector2d(x, y);

		dot1 = vector2d_dot_product(vector2d(POINT.x - p2.x, POINT.y - p2.y), v0);
		dot2 = vector2d_dot_product(vector2d(POINT.x - p2.x, POINT.y - p2.y), vector2d(POINT.x - p2.x, POINT.y - p2.y));
		if (dot1 >= 0 && dot1 >= dot2)
		{
			check2 = true;
			return 1;
		}
		else
		{
			check2 = false;
			return 0;
		}
	}
	else if (p0.x >= p1.x && p0.y < p4.y)
	{
		p1.y += 114;
		p2.x += 200;
		//v0 = p2 - p1
		v0 = vector2d(p2.x - p1.x, p2.y - p1.y);
		//v1 = (-v0.y, v0.x)
		v1 = vector2d(-(v0.y), v0.x);

		mag1 = v0.y / v0.x;
		mag2 = v1.y / v1.x;

		x = (-(p0.x * mag2) + p0.y + (p2.x * mag1) - p2.y) / (mag1 - mag2);
		y = mag1 * (x - p2.x) + p2.y;
		POINT = vector2d(x, y);

		dot1 = vector2d_dot_product(vector2d(POINT.x - p1.x, POINT.y - p1.y), v0);
		dot2 = vector2d_dot_product(vector2d(POINT.x - p1.x, POINT.y - p1.y), vector2d(POINT.x - p1.x, POINT.y - p1.y));
		if (dot1 >= 0 && dot1 >= dot2)
		{
			check1 = true;
			return 1;
		}
		else
		{
			check1 = false;
			return 0;
		}
	}
}

void scene_arena_draw(Scene *s, Vector2D movement)
{
	if (movement.x == 0 && movement.y == 0)
	{
		gf2d_sprite_draw_image(arena, s->origin);
		return;
	}
	
	s->origin.x -= movement.x;
	s->origin.y -= movement.y;
	entity_scroll(movement, temp);
	if (!scene_arena_update(s))
	{
		s->origin.x += movement.x;
		s->origin.y += movement.y;
		entity_scroll(vector2d(-movement.x, -movement.y), temp);
	}

	gf2d_sprite_draw_image(arena, s->origin);
}

void scene_rim_draw(Scene *s)
{
	gf2d_sprite_draw_image(arena_rim, s->origin);
}

void scene_draw(Scene *s)
{
	if (s == NULL)
	{
		slog("null scene provided for draw!");
		return;
	}
	if (s->_anim)
	{
		s->frame += 0.1;
		if (s->frame > 13)
			s->frame = 0;
		gf2d_sprite_draw
			(
			s->background,
			vector2d(0,10),
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			s->frame
			);
	}
	else
	{
		gf2d_sprite_draw_image(s->background, s->origin);
	}
}

Scene *scene_get_active()
{
	if (over._active)
		return &over;
	else if (battle._active)
		return &battle;
}

Scene *scene_get(char *s)
{
	if (strcmp(s, "over") == 0)
	{
		return &over;
	}
	else if (strcmp(s, "battle") == 0)
	{
		return &battle;
	}
	else if (strcmp(s, "all") == 0)
	{
		return &all;
	}
}

void scene_swap(char *s)
{
	if (strcmp(s, "over") == 0)
	{
		over._active = 1;
		battle._active = 0;
	}
	else if (strcmp(s, "battle") == 0)
	{
		temp = entity_new();
		temp->position = vector2d(536, 536);
		temp->sprite = gf2d_sprite_load_image("images/dummy.png");
		temp->fpl = 0;
		temp->scene = scene_get("battle");
		temp->isEnm = true;
		enemy_new(&temp->enm);
		over._active = 0;
		battle._active = 1;
	}
}

void scene_free()
{
	gf2d_sprite_free(over.background);
	gf2d_sprite_free(battle.background);
	memset(&over, 0, sizeof(Scene));
	memset(&battle, 0, sizeof(Scene));
	memset(&all, 0, sizeof(Scene));
}