#include "my_tile.h"
#include "simple_logger.h"

Tile tile_new(Sprite *s, Vector2D pos, Vector2D size, int solid)
{
	Tile t;
	if (s == NULL)
	{
		t.sprite = gf2d_sprite_load_image("images/Tiles/Grass_Full.png");
	}
	else
	{
		t.sprite = s;
	}
	t.origin = pos;
	t.size = size;
	t.col = col_new_rect(pos.x, pos.y, size.x, size.y, solid);
	return t;
}

void tile_draw(Tile *t)
{
	if (!t)
	{
		slog("null tile passed");
		return;
	}
	gf2d_sprite_draw(
		t->sprite,
		t->origin,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		0);
}

void tile_free(Tile *t)
{
	if (!t) return;
	gf2d_sprite_free(t->sprite);
	memset(t, 0, sizeof(Tile));
}