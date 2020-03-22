#ifndef __MY_INTERACTABLE_H__
#define __MY_INTERACTABLE_H__

#include "gf2d_sprite.h"
#include "my_entity.h"
#include "my_collider.h"

typedef struct Interactable_S
{
	Entity ent;
	RectCol col;

}Interactable;

#endif