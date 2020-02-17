#include "my_level.h"
#include "simple_logger.h"
#include "gf2d_sprite.h"
#include "my_scene_manager.h"
#include "my_collider.h"

static Level current = { 0 };

void level_init(int maxrooms, int maxtiles)
{
	SJson *s, *v;
	int x;
	current.name = scene_name();
	s = sj_array_new();
	v = sj_string_new_bool(1);
	for (x = 0; x < maxrooms; x++)
	{
		sj_array_append(s, v);
	}
	current.pallete = maxtiles;
	current.roommax = maxrooms;
	current.tiles = (Sprite *)malloc(sizeof(Sprite)*current.pallete*current.roommax);
	current.rooms = (Room *)malloc(sizeof(Room)* current.roommax);
}

Room level_room_new(Room *r, int index)
{
	Room rm;
	SJson *s, *v;
	int x;
	Vector2D p;

	s = sj_array_new();
	v = sj_string_new_bool(1);
	for (x = 0; x < 240; x++)
	{
		sj_array_append(s, v);
	}
	rm.name = (char)v;
	rm.height = 20;
	rm.width = 12;
	rm.layout = s;
	
	p.x = 1280 * (index - (3 * (int)(index * 0.34)));
	p.y = 768 * (int)(index * 0.34);

	rm.pos = p;

	return rm;
}

void level_room_build(Room *r)
{
	int x, y, col_count = 0;
	RectCol *rect;
	int indexes[240];
	for (x = 0; x < sj_array_get_count(r->layout); x++)
	{
		if (sj_array_get_nth(r->layout, x))
		{
			indexes[y] = x;
			y++;
			col_count++;
		}
	}
	r->col = (RectCol *)malloc(sizeof(RectCol)*col_count);
	for (x = 0; x < col_count; x++)
	{
		rect = &r->col[x];
		y = sj_array_get_nth(r->layout, indexes[x]);
		rect->origin.x = 64 * (y - (20 * (int)(y * 0.05)));
		rect->origin.y = 64 * (int)(y * 0.05);
		rect->size.x = 64;
		rect->size.y = 64;
		rect->solid = 1;
	}

	for (x = 0; x < current.roommax; x++)
	{
		if (current.rooms[x]._inuse)
			continue;
		current.rooms[x] = *r;
		current.rooms[x]._inuse = true;
		break;
	}
}

void level_build_all(char *n)
{

}

void level_deconstruct()
{

}

void level_draw_all()
{

}
