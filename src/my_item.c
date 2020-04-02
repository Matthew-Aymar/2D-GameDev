#include "my_item.h"
#include "simple_logger.h"
#include "my_player.h"
#include "my_entity.h"
#include "my_room.h"

static char *potion		= "images/Items/Potion.png";
static char *bomb		= "images/Items/Bomb.png";
static char *flash		= "images/Items/Flash_Powder.png";
static char *chain		= "images/Items/Chain_Knife.png";
static char *looper		= "images/Items/Looper.png";

static char *potion_out = "images/Items/Potion_Out.png";
static char *potion_burst = "images/Items/Potion_Burst.png";

static char *bomb_out = "images/Items/Bomb_Out.png";
static char *bomb_burst = "images/Items/Bomb_Burst.png";

static char *flash_burst = "images/Items/Flash_Powder_Burst.png";

static char *chain_out = "images/Items/Chain_Knife_Out.png";

static char *looper_out = "images/Items/Looper_Out.png";

void item_new(Item *i, Uint8 id)
{
	switch (id)
	{
	case 0:
		//Potion
		slog("potion");
		i->icon = gf2d_sprite_load_image(potion);
		i->use = item_use_potion;
		i->update = item_update_potion;
		break;
	case 1:
		//Bomb
		slog("bomb");
		i->icon = gf2d_sprite_load_image(bomb);
		i->use = item_use_bomb;
		i->update = item_update_bomb;
		i->force = 100;
		break;
	case 2:
		//Flash Powder
		slog("flash");
		i->icon = gf2d_sprite_load_image(flash);
		i->use = item_use_flash;
		i->update = item_update_flash;
		break;
	case 3:
		//Chain Knife
		slog("chain");
		i->icon = gf2d_sprite_load_image(chain);
		i->use = item_use_chain;
		i->update = item_update_chain;
		break;
	case 4:
		//Looper
		slog("looper");
		i->icon = gf2d_sprite_load_image(looper);
		i->use = item_use_looper;
		i->update = item_update_looper;
		break;
	}
	i->id = id;
	i->count = 3;
}

/////////////
// POTION
/////////////

void item_use_potion(void *p)
{
	Item *i;
	Player *player;
	Vector2D dir;
	player = (Player *)p;
	i = &player->items[player->item_1];
	if (i->count >= 1)
	{
		if (player->battle)
		{
			i->out = gf2d_sprite_load_all(potion_out, 32, 32, 10);
			i->pos = player->player_ent->position;
			i->trackdir = vector2d((i->pos.x - player->mouse.x), (i->pos.y - player->mouse.y));
			vector2d_normalize(&i->trackdir);
			i->col = col_new_circle(i->pos.x, i->pos.y, 64, 0);
			i->frame = 0;
			i->moving = true;
			i->count--;
		}
		else
		{
			if (player->team[0]->health < player->team[0]->health_max)
			{
				player->team[0]->health = player->team[0]->health_max;
				i->count--;
				player->item_out = 0;
			}
		}
	}
}

void item_update_potion(Item *i, void *p)
{
	Player *player;
	player = (Player *)p;
	if (i->frame <= 10 && i->moving)
	{
		i->pos.x -= i->trackdir.x * 2.5;
		i->pos.y -= i->trackdir.y * 2.5;

		gf2d_sprite_draw(
			i->out,
			vector2d(i->pos.x - 16, i->pos.y - 16),
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			i->frame
			);

		i->frame += 0.1;
	}
	else
	{
		if (i->frame >= 10 && i->moving)
		{
			i->out = gf2d_sprite_load_all(potion_burst, 128, 128, 5);
			i->frame = 0;
			i->moving = false;
			i->col.origin = i->pos;
		}

		gf2d_sprite_draw(
			i->out,
			vector2d(i->pos.x - 64, i->pos.y - 64),
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			i->frame
		);

		if (col_circle_circle(&i->col, &player->team[0]->col))
		{
			player->team[0]->health = player->team[0]->health_max;
		}

		i->frame += 0.1;
		if (i->frame >= 5)
		{
			player->item_out = 0;
		}
	}
}

/////////////
// BOMB
/////////////
void item_use_bomb(void *p)
{
	Item *i;
	Player *player;
	player = (Player *)p;
	i = &player->items[player->item_1];
	if (i->count >= 1)
	{
		if (player->battle)
		{
			i->out = gf2d_sprite_load_all(bomb_out, 32, 32, 19);
			i->pos.x = player->player_ent->position.x + 32;
			i->pos.y = player->player_ent->position.y + 32;
			i->col = col_new_circle(i->pos.x, i->pos.y, 64, 0);
			i->frame = 0;
			i->moving = true;
			i->collided = false;
			i->count--;
		}
		else
		{
			if (player->can_interact == 2)
			{
				i->pos = player_get_interact_pos();
				player_clear_interact();
				i->out = gf2d_sprite_load_all(bomb_burst, 128, 128, 9);
				i->frame = 0;
				i->moving = false;
				i->collided = true;
				i->count--;
			}
		}
	}
}

void item_update_bomb(Item *i, void *p)
{
	Player *player;
	player = (Player *)p;

	if (i->frame <= 19 && i->moving)
	{
		gf2d_sprite_draw(
			i->out,
			vector2d(i->pos.x - 16, i->pos.y - 16),
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			i->frame
			);

		i->frame += 0.1;
	}
	else
	{
		if (i->frame >= 10 && i->moving)
		{
			i->out = gf2d_sprite_load_all(bomb_burst, 128, 128, 9);
			i->frame = 0;
			i->moving = false;
			i->col.origin = i->pos;
		}

		gf2d_sprite_draw(
			i->out,
			vector2d(i->pos.x - 64, i->pos.y - 64),
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			i->frame
		);

		i->frame += 0.1;

		i->col.origin = i->pos;

		if (!i->collided)
		{
			if (entity_check_col(&i->col, i))
			{
				i->collided = true;
			}
		}

		if (i->frame >= 9)
		{
			player->item_out = 0;
		}
	}
}

/////////////
// FLASH
/////////////

void item_update_flash(Item *i, void *p)
{
	Player *player;
	player = (Player *)p;
	Vector3D rotation;

	if (player->battle)
	{
		rotation = vector3d(64, 64, vector2d_angle(i->trackdir) - 90);

		gf2d_sprite_draw(
			i->out,
			vector2d(i->pos.x, i->pos.y),
			NULL,
			NULL,
			&rotation,
			NULL,
			NULL,
			i->frame
			);

		i->frame += 0.1;
		if (i->frame >= 10 && !i->collided)
		{
			if (entity_check_col(&i->col, i))
			{
				i->collided = true;
			}
		}

		if (i->frame >= 13)
		{
			player->item_out = 0;
		}
	}
	else
	{
		rotation = vector3d(64, 64, i->trackpos.x);

		gf2d_sprite_draw(
			i->out,
			vector2d(i->pos.x - 64, i->pos.y - 64),
			NULL,
			NULL,
			&rotation,
			NULL,
			NULL,
			i->frame
			);

		i->frame += 0.1;

		if (i->frame >= 10 && !i->collided)
		{
			player_clear_interact();
			i->collided = true;
		}

		if (i->frame >= 13)
		{
			player->item_out = 0;
		}
	}
}

void item_use_flash(void *p)
{
	Item *i;
	Player *player;
	player = (Player *)p;
	i = &player->items[player->item_1];
	if (i->count >= 1)
	{
		if (player->battle)
		{
			i->out = gf2d_sprite_load_all(flash_burst, 128, 128, 13);
			i->pos.x = player->player_ent->position.x - 32;
			i->pos.y = player->player_ent->position.y - 32;
			i->trackdir = vector2d((i->pos.x - player->mouse.x), (i->pos.y - player->mouse.y));
			vector2d_normalize(&i->trackdir);
			i->pos.x -= i->trackdir.x * 64;
			i->pos.y -= i->trackdir.y * 64;
			i->col = col_new_circle(i->pos.x, i->pos.y, 64, 0);
			i->col.origin.x += i->trackdir.x * 64;
			i->col.origin.y += i->trackdir.y * 64;
			i->moving = true;
			i->frame = 0; //used as a ttl
			i->collided = false;
			i->force = 5;
			i->count--;
		}
		else
		{
			if (player->can_interact == 5)
			{
				i->pos = player_get_interact_pos();
				i->out = gf2d_sprite_load_all(flash_burst, 128, 128, 13);
				i->frame = 0;
				i->moving = false;
				i->collided = false;
				if (player->dir == 0)
				{
					i->trackpos.x = 0;
				}
				else if (player->dir == 2)
				{
					i->trackpos.x = 90;
				}
				else if (player->dir == 4)
				{
					i->trackpos.x = 180;
				}
				else if (player->dir == 6)
				{
					i->trackpos.x = 270;
				}
				i->count--;
			}
		}
	}
}

/////////////
// CHAIN
/////////////

void item_update_chain(Item *i, void *p)
{
	Vector3D rotation;
	Vector2D dir, diff;
	float dist;
	Player *player;
	player = (Player *)p;

	if (i->frame >= 0 && !i->collided)
	{
		i->pos.x -= i->trackdir.x * 25;
		i->pos.y -= i->trackdir.y * 25;

		i->col.origin = i->pos;
		i->col2.origin = i->pos;

		rotation = vector3d(16, 16, vector2d_angle(i->trackdir) + 180);
		gf2d_sprite_draw(
			i->out,
			i->pos,
			NULL,
			NULL,
			&rotation,
			NULL,
			NULL,
			0
			);

		if (player->battle && entity_check_col(&i->col, i))
		{
			i->collided = true;
			player->item_out = false;
		}
		else if (!player->battle)
		{
			if (room_check_col(&i->col2))
			{
				i->collided = true;
				player->grounded = false;
			}
		}
	}
	
	if (!player->battle && i->collided)
	{
		rotation = vector3d(8, 8, vector2d_angle(i->trackdir) + 180);
		gf2d_sprite_draw(
			i->out,
			i->pos,
			NULL,
			NULL,
			&rotation,
			NULL,
			NULL,
			0
			);

		dir.x = i->pos.x - player->player_ent->position.x;
		dir.y = i->pos.y - player->player_ent->position.y;
		diff = dir;
		vector2d_normalize(&dir);

		dir.x *= 7;
		dir.y *= 7;

		item_scroll(i, dir);
		room_manager_scroll(dir);

		dist = SDL_sqrtf(SDL_powf(diff.x, 2), SDL_powf(diff.y, 2));
		if (dist <= 3)
		{
			i->frame = 0;
			player->item_out = false;
			player->grounded = true;
			i->count++;
		}
	}

	i->frame -= 0.1;
	if (i->frame < 0)
	{
		player->item_out = false;
		player->grounded = true;
	}
}

void item_use_chain(void *p)
{
	Item *i;
	Player *player;
	player = (Player *)p;
	i = &player->items[player->item_1];
	if (i->count >= 1)
	{	
		if (player->battle)
		{
			i->out = gf2d_sprite_load_image(chain_out);
			i->pos.x = player->player_ent->position.x + 16;
			i->pos.y = player->player_ent->position.y + 16;
			i->trackpos = i->pos;
			i->trackdir = vector2d((i->pos.x - player->mouse.x), (i->pos.y - player->mouse.y));
			vector2d_normalize(&i->trackdir);
			i->col = col_new_circle(i->pos.x, i->pos.y, 16, 0);
			i->moving = true;
			i->frame = 5; //used as a ttl
			i->collided = false;
			i->force = 50;
			i->count--;
		}
		else if (!player->moving)
		{
			i->out = gf2d_sprite_load_image(chain_out);
			i->pos.x = player->player_ent->position.x + 16;
			i->pos.y = player->player_ent->position.y + 16;
			i->trackdir = vector2d((i->pos.x - player->mouse.x), (i->pos.y - player->mouse.y));
			vector2d_normalize(&i->trackdir);
			i->col2 = col_new_rect(i->pos.x, i->pos.y, 32, 32, 1);
			i->moving = true;
			i->collided = false;
			i->frame = 25;
			i->count--;
		}
	}
}

/////////////
// LOOPER
/////////////

void item_update_looper(Item *i, void *p)
{
	Player *player;
	player = (Player *)p;

	gf2d_sprite_draw(
		i->out,
		i->pos,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		i->frame
		);

	i->frame += 0.1;
	if (i->frame >= 3)
	{
		i->frame = 0;
	}

	i->trackpos = player->mouse;
	i->trackdir.x = i->trackpos.x - i->pos.x - 32;
	i->trackdir.y = i->trackpos.y - i->pos.y - 32;
	vector2d_normalize(&i->trackdir);

	i->pos.x += i->trackdir.x * 3;
	i->pos.y += i->trackdir.y * 3;

	i->col.origin.x = i->pos.x - 32;
	i->col.origin.y = i->pos.y - 32;

	if (entity_check_col(&i->col, &i) && !i->collided)
	{
		i->collided = true;
		player->item_out = false;
		player->battle = false;
		player_follower_new();
		scene_swap("over");
	}
}

void item_use_looper(void *p)
{
	Item *i;
	Player *player;
	player = (Player *)p;
	i = &player->items[player->item_2];
	if (i->count >= 1)
	{
		i->out = gf2d_sprite_load_all(looper_out, 64, 64, 3);
		i->pos.x = player->player_ent->position.x - 32;
		i->pos.y = player->player_ent->position.y - 32;
		i->frame = 0;
		i->moving = true;
		i->collided = false;
		i->col = col_new_circle(i->pos.x, i->pos.y, 32, 1);
		i->trackpos = vector2d(player->mouse.x, player->mouse.y);
		i->count--;
	}
}

void item_scroll(Item *i, Vector2D movement)
{
	i->pos.x -= movement.x;
	i->pos.y -= movement.y;
}