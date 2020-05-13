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
	over.background = NULL;
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
	if (!col_battle_bounds(s, player_get_circle()->origin))
	{
		return 1;
	}
	else { return 0; }
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
		if (temp)
			entity_free(temp);
		over._active = 1;
		battle._active = 0;
	}
	else if (strcmp(s, "battle") == 0)
	{
		if (player_get_boss())
		{
			temp = entity_new();
			temp->position = vector2d(552, 552);
			temp->fpl = 0;
			temp->scene = scene_get("battle");
			temp->isEnm = true;
			temp->enm.boss = player_get_boss();
			enemy_new(&temp->enm);
			over._active = 0;
			battle._active = 1;
			return;
		}
		
		temp = entity_new();
		temp->position = vector2d(576, 576);
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