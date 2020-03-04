#include "my_collider.h"
#include "simple_logger.h"

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

CirCol col_new_circle(float posx, float posy, float rad, Uint8 solid)
{
	CirCol c;
	Vector2D o;
	o.x = posx + rad;
	o.y = posy + rad;
	
	c.origin = o;
	c.radius = rad;
	c.solid = solid;
	return c;
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

int col_circle_circle(CirCol *col1, CirCol *col2)
{
	Vector2D pos1, pos2, diff;
	float dist;

	diff = vector2d(col2->origin.x - col1->origin.x, col2->origin.y - col1->origin.y);
	dist = SDL_sqrtf(SDL_powf(diff.x, 2) + SDL_powf(diff.y, 2));

	if (dist <= col1->radius + col2->radius)
	{
		return 1;
	}
	else { return 0; }
}

void col_free_rect(RectCol *col)
{
	if (!col)return;
	memset(col, 0, sizeof(RectCol));
}