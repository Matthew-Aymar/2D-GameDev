#include "my_collider.h"

RectCol col_new_rect(float posx, float posy, float sizex, float sizey, Uint8 solid)
{
	RectCol r;
	Vector2D o, s;
	o.x = posx;
	o.y = posy;
	s.x = sizex;
	s.y = sizey;
	r.origin = o;
	r.size = s;
	r.solid = solid;
	return r;
}

int col_rect_rect(RectCol *col1, RectCol *col2)
{
	if (col1->origin.x + col1->size.x < col2->origin.x
		|| col2->origin.x + col2->size.x < col1->origin.x
		|| col1->origin.y + col1->size.y < col2->origin.y
		|| col2->origin.y + col2->size.y < col1->origin.y)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

void col_free_rect(RectCol *col)
{
	if (!col)return;
	memset(col, 0, sizeof(RectCol));
}