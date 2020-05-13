#include "my_editor.h"
#include "simple_logger.h"

static int map[240];
static int testlayout[240] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
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

static Room edit;

static float clickcd = 0;

void editor_new_map()
{
	int x;
	for (x = 0; x < 240; x++)
	{
		map[x] = testlayout[x];
	}
	room_set_interactables(false);
	room_new(&edit, map, -40, -24, 0);
	room_set_interactables(true);

	slog("created new editor room");
}

void editor_update(float mx, float my, Uint8 click)
{
	int row, col, index;
	float temp;
	Vector2D size, pos;
	room_update(&edit);
	
	if (click && clickcd <= 0)
	{
		if (mx < 24)
			col = 0;
		else if (mx > 1176)
			col = 19;
		else
		{
			temp = (mx - 24) / 1152;
			temp *= 100;
			temp /= 5.556;
			col = (int)temp;
			col++;
		}

		if (my < 40)
			row = 0;
		else if (my > 680)
			row = 12;
		else
		{
			temp = (my - 40) / 640;
			temp *= 10;
			row = (int)temp;
			row++;
		}

		index = (row * 20) + col;

		if (map[index] < 2)
			map[index]++;
		else
			map[index] = 0;

		room_free(&edit);
		room_set_interactables(false);
		room_new(&edit, map, -40, -24, 0);
		room_set_interactables(true);

		clickcd = 2;
	}
	else
		clickcd -= 0.1;
}

void editor_save_map()
{
	int x;
	FILE *mapfile = fopen("map.txt", "w");
	char temp[240];
	for (x = 0; x < 240; x++)
	{
		temp[x] = map[x] + '0';
	}
	fputs(temp, mapfile);
	fclose(mapfile);
}