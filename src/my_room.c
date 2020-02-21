#include "my_room.h"
#include "simple_logger.h"
#include "my_player.h"
#include "my_collider.h"

static Room test_room = { 0 };
static int room_tiles = 240;

Room *room_new(int l[240], float xpos, float ypos)
{
	int x;
	Vector2D pos;
	test_room.origin = vector2d(xpos, ypos);
	for (x = 0; x < room_tiles; x++)
	{
		pos.x = test_room.origin.x + (64 * (x - (20 * (int)(x * 0.05))));
		pos.y = test_room.origin.y + (64 * (int)(x * 0.05));
		if (l[x] == 1)
		{
			test_room.tiles[x] = tile_new(gf2d_sprite_load_image("images/Tiles/Wall_Top.png"),
				pos, vector2d(64, 64), 1);
		}
		else
		{
			test_room.tiles[x] = tile_new(gf2d_sprite_load_image("images/Tiles/Grass_Full.png"),
				pos, vector2d(64, 64), 0);
		}
	}
	return &test_room;
}

void room_update(Room *rm)
{
	for (int x = 0; x < room_tiles; x++)
	{
		tile_draw(&test_room.tiles[x]);
		player_check_col(&test_room.tiles[x].col);
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
					rm->tiles[y].col.origin.x -= offx;
					rm->tiles[y].col.origin.y -= offy;
					rm->tiles[y].origin.x = rm->tiles[y].col.origin.x;
					rm->tiles[y].origin.y = rm->tiles[y].col.origin.y;
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