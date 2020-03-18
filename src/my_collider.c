#include "my_collider.h"
#include "simple_logger.h"
#include "my_scene.h"

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

int col_battle_bounds(Scene *s, Vector2D point)
{
	Vector2D p0, p1, p2, p3, p4, v0, v1, v2, temp, POINT;
	/*
			p1
	p2				p4
			p3

	v0 is the vector of the edge to check
	v1 is the perpendicular of that vector
	*/
	float mag1, mag2, x, y, dot1, dot2;

	p0 = point;
	p1 = vector2d(s->origin.x + 600, s->origin.y);
	p2 = vector2d(s->origin.x, s->origin.y + 600);
	p3 = vector2d(s->origin.x + 600, s->origin.y + 1200);
	p4 = vector2d(s->origin.x + 1200, s->origin.y + 600);

	//Very much a hack section, TODO: remove hard coding

	if (p0.x >= p1.x && p0.y >= p4.y - 64)
	{
		p1.x -= 132;
		p1.y -= 268;
		p4.x -= 48;
		p4.y -= 48;
		//v0 = p1 - p4
		v0 = vector2d(p1.x - p4.x, p1.y - p4.y);
		//v1 = (-v0.y, v0.x)
		v1 = vector2d(-(v0.y), v0.x);

		mag1 = v0.y / v0.x;
		mag2 = v1.y / v1.x;

		x = (-(p0.x * mag2) + p0.y + (p1.x * mag1) - p1.y) / (mag1 - mag2);
		y = mag1 * (x - p1.x) + p1.y;
		POINT = vector2d(x, y);

		dot1 = vector2d_dot_product(vector2d(POINT.x - p4.x, POINT.y - p4.y), v0);
		dot2 = vector2d_dot_product(vector2d(POINT.x - p4.x, POINT.y - p4.y), vector2d(POINT.x - p4.x, POINT.y - p4.y));
		if (dot1 >= 0 && dot1 >= dot2)
		{
			return 0;
		}
		else
		{
			return 4;
		}
	}
	else if (p0.x < p1.x && p0.y >= p2.y - 64)
	{
		p3.y -= 168;
		p4.x -= 232;
		//v0 = p4 - p3
		v0 = vector2d(p4.x - p3.x, p4.y - p3.y);
		//v1 = (-v0.y, v0.x)
		v1 = vector2d(-(v0.y), v0.x);

		mag1 = v0.y / v0.x;
		mag2 = v1.y / v1.x;

		x = (-(p0.x * mag2) + p0.y + (p4.x * mag1) - p4.y) / (mag1 - mag2);
		y = mag1 * (x - p4.x) + p4.y;
		POINT = vector2d(x, y);

		dot1 = vector2d_dot_product(vector2d(POINT.x - p3.x, POINT.y - p3.y), v0);
		dot2 = vector2d_dot_product(vector2d(POINT.x - p3.x, POINT.y - p3.y), vector2d(POINT.x - p3.x, POINT.y - p3.y));
		if (dot1 >= 0 && dot1 >= dot2)
		{
			return 0;
		}
		else
		{
			return 3;
		}
	}
	else if (p0.x < p1.x && p0.y < p2.y)
	{
		p3.y += 132;
		p4.y -= 132;
		//v0 = p3 - p2
		v0 = vector2d(p3.x - p2.x, p3.y - p2.y);
		//v1 = (-v0.y, v0.x)
		v1 = vector2d(-(v0.y), v0.x);

		mag1 = v0.y / v0.x;
		mag2 = v1.y / v1.x;

		x = (-(p0.x * mag2) + p0.y + (p3.x * mag1) - p3.y) / (mag1 - mag2);
		y = mag1 * (x - p3.x) + p3.y;
		POINT = vector2d(x, y);

		dot1 = vector2d_dot_product(vector2d(POINT.x - p2.x, POINT.y - p2.y), v0);
		dot2 = vector2d_dot_product(vector2d(POINT.x - p2.x, POINT.y - p2.y), vector2d(POINT.x - p2.x, POINT.y - p2.y));
		if (dot1 >= 0 && dot1 >= dot2)
		{
			return 0;
		}
		else
		{
			return 2;
		}
	}
	else if (p0.x >= p1.x && p0.y < p4.y)
	{
		p1.y += 114;
		p2.x += 200;
		//v0 = p2 - p1
		v0 = vector2d(p2.x - p1.x, p2.y - p1.y);
		//v1 = (-v0.y, v0.x)
		v1 = vector2d(-(v0.y), v0.x);

		mag1 = v0.y / v0.x;
		mag2 = v1.y / v1.x;

		x = (-(p0.x * mag2) + p0.y + (p2.x * mag1) - p2.y) / (mag1 - mag2);
		y = mag1 * (x - p2.x) + p2.y;
		POINT = vector2d(x, y);

		dot1 = vector2d_dot_product(vector2d(POINT.x - p1.x, POINT.y - p1.y), v0);
		dot2 = vector2d_dot_product(vector2d(POINT.x - p1.x, POINT.y - p1.y), vector2d(POINT.x - p1.x, POINT.y - p1.y));
		if (dot1 >= 0 && dot1 >= dot2)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
}

void col_free_rect(RectCol *col)
{
	if (!col)return;
	memset(col, 0, sizeof(RectCol));
}