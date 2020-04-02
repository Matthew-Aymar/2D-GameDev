#include "my_room.h"
#include "simple_logger.h"
#include "my_player.h"
#include "my_collider.h"

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

static int testlayout[240] = {	1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
								1, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 1,
								1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
								1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
								2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
								0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
								0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
								1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
								1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
								1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
								1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
								1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

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
	Scene	*scene;
}Room_Manager;

static Room_Manager room_manager = { 0 };

static int swap;
static int init = false;

void *room_new(Room *rm, int l[240], float xpos, float ypos, int index)
{
	int x;
	Vector2D pos;
	float offx, offy;
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
	rm->index = index;
}

void room_update(Room *rm)
{
	for (int x = 0; x < room_tiles; x++)
	{
		tile_draw(&rm->tiles[x]);
	}
}

void room_scroll(Room *rm, Vector2D movement)
{
	int x, y, z;
	Vector2D previous;
	float offx, offy;
	Vector2D pos;

	//If you have swapped the last frame
	if (swap)
	{
		if (swap == 1)
		{
			rm->origin.y -= 3;
		}
		else if (swap == 3)
		{
			rm->origin.x -= 3;
		}
		else if (swap == 5)
		{
			rm->origin.x += 3;
		}
		else if (swap == 7)
		{
			rm->origin.y += 3;
		}
		swap = 0;
		return;
	}

	//If it is the current/middle room
	if (rm->index == 4)
	{
		//Loops through all tiles in the room and check for collision
		for (x = 0; x < room_tiles; x++)
		{
			if (rm->tiles[x].col.solid == 1)
			{
				//Forcast tile collider positions
				previous.x = rm->tiles[x].col.origin.x;
				previous.y = rm->tiles[x].col.origin.y;
				rm->tiles[x].col.origin.x -= (movement.x);
				rm->tiles[x].col.origin.y -= (movement.y);
				//If it collides with the player
				if (col_rect_rect(player_get_rect(), &rm->tiles[x].col) || rm->index == swap)
				{
					//Revert all changes and return
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
					//check for the tiles interactable
					if (rm->tiles[x].interactable)
					{
						interactable_get_tile(&rm->tiles[x].inter, &rm->tiles[x]);
						player_can_interact(rm->tiles[x].inter.id, &rm->tiles[x].inter);
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
	}
	else
	{
		//Lock other room positions based on the middle room
		if (rm->index == 0 || rm->index == 3 || rm->index == 6)
		{
			rm->origin.x = room_manager.rooms_active[4]->origin.x - 1280;
		}
		else if (rm->index == 2 || rm->index == 5 || rm->index == 8)
		{
			rm->origin.x = room_manager.rooms_active[4]->origin.x + 1280;
		}
		else
		{
			rm->origin.x = room_manager.rooms_active[4]->origin.x;
		}

		if (rm->index == 0 || rm->index == 1 || rm->index == 2)
		{
			rm->origin.y = room_manager.rooms_active[4]->origin.y - 768;
		}
		else if (rm->index == 6 || rm->index == 7 || rm->index == 8)
		{
			rm->origin.y = room_manager.rooms_active[4]->origin.y + 768;
		}
		else
		{
			rm->origin.y = room_manager.rooms_active[4]->origin.y;
		}

		//Move tiles
		for (x = 0; x < room_tiles; x++)
		{
			pos.x = rm->origin.x + (64 * (x - (20 * (int)(x * 0.05))));
			pos.y = rm->origin.y + (64 * (int)(x * 0.05));
			rm->tiles[x].origin.x = pos.x;
			rm->tiles[x].origin.y = pos.y;
			rm->tiles[x].col.origin.x = pos.x;
			rm->tiles[x].col.origin.y = pos.y;
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
	//interactable_free(&rm->i);
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
	slog("Closed Room Manager");
}

void room_manager_init()
{
	int x;
	Vector2D pos;
	pos.x = -1320; //-1280 - an offset of 40 so the player starts centered
	pos.y = -800;  //-768 - an offset of 32
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
					pos.y + (768 * (int)(x * 0.334)), 
					x);
	}

	swap = 0;

	room_manager.scene = scene_get("over");

	slog("Initialized room manager");

	atexit(room_manager_close);

	init = true;
}

void room_manager_update()
{
	int x;
	if (!room_manager.scene->_active)
	{
		return;
	}
	for (x = 0; x < 9; x++)
	{
		room_update(room_manager.rooms_active[x]);
	}
}

void room_manager_scroll(Vector2D movement)
{
	int x;
	if (!room_manager.scene->_active)
	{
		return;
	}
	room_scroll(room_manager.rooms_active[4], movement);
	for (x = 0; x < 9; x++)
	{
		if (x == 4)
			continue;
		room_scroll(room_manager.rooms_active[x], movement);
	}
}

void room_manager_swap(float xpos, float ypos)
{
	Vector2D pos;
	int x;
	if (!room_manager.scene->_active)
	{
		return;
	}
	if (xpos + 32 < room_manager.rooms_active[4]->origin.x)
	{
		//swap left
		room_free(room_manager.rooms_active[2]);
		room_free(room_manager.rooms_active[5]);
		room_free(room_manager.rooms_active[8]);
		
		room_tr = room_t;
		room_tr.index = 2;
		room_r =  room_c;
		room_r.index = 5;
		room_br = room_b;
		room_br.index = 8;

		room_t = room_tl;
		room_t.index = 1;
		room_c = room_l;
		room_c.index = 4;
		room_b = room_bl;
		room_b.index = 7;

		room_new(&room_tl, testlayout, 
			room_manager.rooms_active[4]->origin.x - 1280, 
			room_manager.rooms_active[4]->origin.y + 768, 0);
		room_new(&room_l, testlayout,
			room_manager.rooms_active[4]->origin.x - 1280,
			room_manager.rooms_active[4]->origin.y, 3);
		room_new(&room_bl, testlayout,
			room_manager.rooms_active[4]->origin.x - 1280,
			room_manager.rooms_active[4]->origin.y - 768, 6);
		swap = 3;
	}
	else if (ypos + 32 > room_manager.rooms_active[4]->origin.y + 768)
	{
		//swap bot
		room_free(room_manager.rooms_active[0]);
		room_free(room_manager.rooms_active[1]);
		room_free(room_manager.rooms_active[2]);

		room_tl = room_l;
		room_tl.index = 0;
		room_t = room_c;
		room_t.index = 1;
		room_tr = room_r;
		room_tr.index = 2;

		room_l = room_bl;
		room_l.index = 3;
		room_c = room_b;
		room_c.index = 4;
		room_r = room_br;
		room_r.index = 5;

		room_new(&room_bl, testlayout,
			room_manager.rooms_active[4]->origin.x - 1280,
			room_manager.rooms_active[4]->origin.y - 768, 6);
		room_new(&room_b, testlayout,
			room_manager.rooms_active[4]->origin.x,
			room_manager.rooms_active[4]->origin.y - 768, 7);
		room_new(&room_br, testlayout,
			room_manager.rooms_active[4]->origin.x + 1280,
			room_manager.rooms_active[4]->origin.y - 768, 8);
		swap = 7;
	}
	else if (xpos + 32 > room_manager.rooms_active[4]->origin.x + 1280)
	{
		//swap right
		room_free(room_manager.rooms_active[0]);
		room_free(room_manager.rooms_active[3]);
		room_free(room_manager.rooms_active[6]);

		room_tl = room_t;
		room_tl.index = 0;
		room_l = room_c;
		room_l.index = 3;
		room_bl = room_b;
		room_bl.index = 6;

		room_t = room_tr;
		room_t.index = 1;
		room_c = room_r;
		room_c.index = 4;
		room_b = room_br;
		room_b.index = 7;

		room_new(&room_tr, testlayout,
			room_manager.rooms_active[4]->origin.x + 1280,
			room_manager.rooms_active[4]->origin.y + 768, 2);
		room_new(&room_r, testlayout,
			room_manager.rooms_active[4]->origin.x + 1280,
			room_manager.rooms_active[4]->origin.y, 5);
		room_new(&room_br, testlayout,
			room_manager.rooms_active[4]->origin.x + 1280,
			room_manager.rooms_active[4]->origin.y - 768, 8);
		swap = 5;
	}
	else if(ypos + 32 < room_manager.rooms_active[4]->origin.y)
	{
		//swap top
		room_free(room_manager.rooms_active[6]);
		room_free(room_manager.rooms_active[7]);
		room_free(room_manager.rooms_active[8]);

		room_bl = room_l;
		room_bl.index = 6;
		room_b = room_c;
		room_b.index = 7;
		room_br = room_r;
		room_br.index = 8;

		room_l = room_tl;
		room_l.index = 3;
		room_c = room_t;
		room_c.index = 4;
		room_r = room_tr;
		room_r.index = 5;

		room_new(&room_tl, testlayout,
			room_manager.rooms_active[4]->origin.x - 1280,
			room_manager.rooms_active[4]->origin.y + 768, 0);
		room_new(&room_t, testlayout,
			room_manager.rooms_active[4]->origin.x,
			room_manager.rooms_active[4]->origin.y + 768, 1);
		room_new(&room_tr, testlayout,
			room_manager.rooms_active[4]->origin.x + 1280,
			room_manager.rooms_active[4]->origin.y + 768, 2);
		swap = 1;
	}
}

int room_check_col(RectCol *col)
{
	int x;
	for (x = 0; x < room_tiles; x++)
	{
		if (room_manager.rooms_active[4]->tiles[x].col.solid == 1)
		{
			if (col_rect_rect(col, &room_manager.rooms_active[4]->tiles[x].col))
			{
				return true;
			}
		}
	}
	return false;
}