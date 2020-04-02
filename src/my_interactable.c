#include "my_interactable.h"
#include "my_player.h"
#include "simple_logger.h"
#include "my_scene.h"
#include "my_tile.h"

static int box_chance		= 20;
static int rock_chance		= 15;
static int healer_chance	= 15;
static int monster_chance	= 40;
static int shuffler_chance	= 10;

void interactable_box(void *player)
{
	Player *p;
	p = (Player *)player;
	player_restock();
	interactable_clear(p->interact);
}

void interactable_rock(void *player)
{
	//slog("rock");
}

void interactable_healer(void *player)
{
	Player *p;
	int x;
	p = (Player *)player;
	for (x = 0; x < 4; x++)
	{
		p->team[x]->health = p->team[x]->health_max;
	}
}

void interactable_shuffler(void *player)
{
	Player *p;
	int x;
	int i;
	p = (Player *)player;
	player_shuffle_followers();
}

void interactable_monster(void *player)
{
	scene_swap("battle");
	player_set_battle();
}

void interactable_new(Uint8 id, Interactable *i, Vector2D position, Vector2D offset)
{
	if (id == NULL)
		i->id = id + 1;
	else
		i->id = id;
	switch (id)
	{
		case 1:
			i->sprite = gf2d_sprite_load_image("images/Interactables/item_box.png");
			i->on_interact = interactable_box;
			break;
		case 2:
			i->sprite = gf2d_sprite_load_image("images/Interactables/broken_rock.png");
			i->on_interact = interactable_rock;
			break;
		case 3:
			i->sprite = gf2d_sprite_load_image("images/Interactables/npc_healer.png");
			i->on_interact = interactable_healer;
			break;
		case 4:
			i->sprite = gf2d_sprite_load_image("images/Interactables/npc_shuffler.png");
			i->on_interact = interactable_shuffler;
			break;
		case 5:
			i->sprite = gf2d_sprite_load_image("images/Interactables/npc_monster.png");
			i->on_interact = interactable_monster;
			break;
	}
}

void interactable_free(Interactable *i)
{
	memset(i, 0, sizeof(Interactable));
}

int interactable_chance(int num)
{
	if (num < box_chance)
	{
		return 1;
	}
	else if (num < rock_chance + box_chance)
	{
		return 2;
	}
	else if (num < rock_chance + box_chance + healer_chance)
	{
		return 3;
	}
	else if (num < rock_chance + box_chance + healer_chance + shuffler_chance)
	{
		return 4;
	}
	else if (num < rock_chance + box_chance + healer_chance + shuffler_chance + monster_chance)
	{
		return 5;
	}
	else
	{
		return 0;
	}
}

void interactable_clear(Interactable *i)
{
	Tile *t;
	t = (Tile *)i->tile;

	t->col.solid = false;
	t->interactable = false;
	interactable_free(&t->inter);
}

Vector2D interactable_get_pos(Interactable *i)
{
	Tile *t;
	Vector2D v;
	t = (Tile *)i->tile;

	v = vector2d(t->origin.x + 32, t->origin.y + 32);
	return v;
}

void interactable_get_tile(Interactable *i, void *t)
{
	i->tile = t;
}