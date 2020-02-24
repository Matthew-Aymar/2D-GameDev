#include "my_room.h"
#include "simple_logger.h"
#include "my_player.h"
#include "my_collider.h"

static Room test_room = { 0 };
static Room room_tl = { 0 };	//Top Left
static Room room_t = { 0 };		//Top
static Room room_tr = { 0 };	//Top Right
static Room room_l = { 0 };		//Left
static Room room_c = { 0 };		//Current
static Room room_r = { 0 };		//Right
static Room room_bl = { 0 };	//Bottom Left
static Room room_b = { 0 };		//Bottom
static Room room_br = { 0 };	//Bottom Right
static int room_tiles = 240;

static int testlayout[240] = {	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
								1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1,
								1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
								1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
								1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
								1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
								1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
								1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
								1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
								1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
								1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
								2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 };

typedef struct Room_Manager_S
{
	Room *rooms_active[9];
	/*
		Currently rendered rooms in the scene:
		0	1	2
		3	4	5
		6	7	8
		4 being the current room the player is in
	*/
	Sprite *wall_forward;
	Sprite *wall_top;
	Sprite *grass_full;
}Room_Manager;

static Room_Manager room_manager = { 0 };

void room_swap_top()
{
	//free 6, 7, 8
	//swap 3 -> 6, 4 -> 7, 5 -> 8, 1 -> 3, 2 -> 4, 3 -> 5
	//create 3 new rooms in index 0, 1, 2
}

void room_swap_right()
{
	//free 0, 3, 6
	//swap 1 -> 0, 4 -> 3, 7 -> 6, 2 -> 1, 5 -> 4, 8 -> 7
	//create 3 new rooms in index 2, 5, 8
}

void room_swap_bot()
{
	//free 0, 1, 2
	//swap 3 -> 0, 4 -> 1, 5 -> 2, 6 -> 3, 7 -> 4, 8 -> 5
	//create 3 new rooms in index 6, 7, 8
}

void room_swap_left()
{
	//free 2, 5, 8
	//swap 1 -> 2, 4 -> 5, 7 -> 8, 0 -> 1, 3 -> 4, 6 -> 7
	//create 3 new rooms in index 0, 3, 61
}

void *room_new(Room *rm, int l[240], float xpos, float ypos)
{
	int x;
	Vector2D pos;
	slog("new room @ %f, %f", xpos, ypos);
	rm->origin = vector2d(xpos, ypos);
	for (x = 0; x < room_tiles; x++)
	{
		pos.x = rm->origin.x + (64 * (x - (20 * (int)(x * 0.05))));
		pos.y = rm->origin.y + (64 * (int)(x * 0.05));
		if (l[x] == 2)
		{
			rm->tiles[x] = tile_new(room_manager.wall_forward,
				pos, vector2d(64, 64), 1);
		}
		else if (l[x] == 1)
		{
			rm->tiles[x] = tile_new(room_manager.wall_top,
				pos, vector2d(64, 64), 1);
		}
		else
		{
			rm->tiles[x] = tile_new(room_manager.grass_full,
				pos, vector2d(64, 64), 0);
		}
	}
}

void room_update(Room *rm)
{
	for (int x = 0; x < room_tiles; x++)
	{
		tile_draw(&test_room.tiles[x]);
	}
}

void room_scroll(Room *rm, Vector2D movement)
{
	int x, y;
	Vector2D previous;
	float offx, offy;
	for (x = 0; x < room_tiles; x++)
	{
		if (rm->tiles[x].col.solid == 1)
		{
			previous.x = rm->tiles[x].col.origin.x;
			previous.y = rm->tiles[x].col.origin.y;
			rm->tiles[x].col.origin.x -= (movement.x);
			rm->tiles[x].col.origin.y -= (movement.y);
			if (col_rect_rect(player_get_rect(), &rm->tiles[x].col))
			{
				offx = rm->tiles[x].col.origin.x - previous.x;
				offy = rm->tiles[x].col.origin.y - previous.y;
				rm->tiles[x].col.origin.x = previous.x;
				rm->tiles[x].col.origin.y = previous.y;
				for (y = x - 1; y >= 0; y--)
				{
					if (rm->tiles[y].col.solid == 1)
					{
						rm->tiles[y].col.origin.x -= offx;
						rm->tiles[y].col.origin.y -= offy;
						rm->tiles[y].origin.x = rm->tiles[y].col.origin.x;
						rm->tiles[y].origin.y = rm->tiles[y].col.origin.y;
					}
					else
					{
						rm->tiles[y].origin.x += (movement.x);
						rm->tiles[y].origin.y += (movement.y);
					}
				}
				return;
			}
			else
			{
				rm->tiles[x].origin.x = rm->tiles[x].col.origin.x;
				rm->tiles[x].origin.y = rm->tiles[x].col.origin.y;
			}
			y++;
		}
		else
		{
			rm->tiles[x].origin.x -= (movement.x);
			rm->tiles[x].origin.y -= (movement.y);
		}
	}
	rm->origin.x -= (movement.x);
	rm->origin.y -= (movement.y);
}

void room_free(Room *rm)
{
	int x;
	for (x = 0; x < rm->maxtiles; x++)
	{
		tile_free(&rm->tiles[x]);
	}
	memset(rm, 0, sizeof(Room));
}

void room_manager_close()
{
	int x;
	for (x = 0; x < 9; x++)
	{
		room_free(room_manager.rooms_active[x]);
	}
	gf2d_sprite_free(room_manager.wall_forward);
	gf2d_sprite_free(room_manager.wall_top);
	gf2d_sprite_free(room_manager.grass_full);
	memset(&room_manager, 0, sizeof(Room_Manager));
}

void room_manager_init()
{
	int x;
	Vector2D pos;
	pos.x = -1280;
	pos.y = -768;
	room_manager.rooms_active[0] = &room_tl;
	room_manager.rooms_active[1] = &room_t;
	room_manager.rooms_active[2] = &room_tr;
	room_manager.rooms_active[3] = &room_l;
	room_manager.rooms_active[4] = &room_c;
	room_manager.rooms_active[5] = &room_r;
	room_manager.rooms_active[6] = &room_bl;
	room_manager.rooms_active[7] = &room_b;
	room_manager.rooms_active[8] = &room_br;

	room_manager.wall_forward = gf2d_sprite_load_image("images/Tiles/Wall_Forward.png");
	room_manager.wall_top = gf2d_sprite_load_image("images/Tiles/Wall_Top.png");
	room_manager.grass_full = gf2d_sprite_load_image("images/Tiles/Grass_Full.png");

	for (x = 0; x < 9; x++)
	{
		room_new(room_manager.rooms_active[x],
					testlayout, 
					pos.x + (1280 * (x - (3 * (int)(x * 0.334)))),
					pos.y + (768 * (int)(x * 0.334)));
	}

	atexit(room_manager_close);
}

void room_manager_update()
{
	int x;
	for (x = 0; x < 9; x++)
	{
		room_update(room_manager.rooms_active[x]);
		if (x == 4)
		{
			player_check_col(&test_room.tiles[x].col);
		}
	}
}

void room_manager_scroll(Vector2D movement)
{
	int x;
	for (x = 0; x < 9; x++)
	{
		room_scroll(room_manager.rooms_active[x], movement);
	}
}