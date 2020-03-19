#include <SDL.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "my_entity.h"
#include "my_player.h"
#include "my_room.h"
#include "my_scene.h"
#include "my_follower.h"

int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    const Uint8 *keys;
    Sprite *sprite;
    
    int mx,my;
    float mf = 0;
    Sprite *mouse;
    Vector4D mouseColor = {255,100,255,200};
    
	Player *currentPlayer;
	Uint8 W, A, S, D, left_click, right_click, space, num;

	int fpscheck = 0;
	float avgfps = 0;
    /*program initializtion*/
    init_logger("gf2d.log");
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
        1200,
        720,
        1200,
        720,
        vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);
    SDL_ShowCursor(SDL_DISABLE);
    
	entity_manager_init(64);

	scene_init();

	currentPlayer = player_new();
	if (currentPlayer == NULL)
	{
		return 0;
	}

	room_manager_init();

    /*demo setup*/
    sprite = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16);

    /*main game loop*/
	while (!done)
	{
		SDL_PumpEvents();   // update SDL's internal event structures
		keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
		/*update things here*/
		SDL_GetMouseState(&mx, &my);
		mf += 0.1;
		if (mf >= 16.0)mf = 0;

		if (keys[SDL_SCANCODE_W])
		{
			W = true;
		}
		else { W = false; }
		if (keys[SDL_SCANCODE_A])
		{
			A = true;
		}
		else { A = false; }
		if (keys[SDL_SCANCODE_S])
		{
			S = true;
		}
		else { S = false; }
		if (keys[SDL_SCANCODE_D])
		{
			D = true;
		}
		else { D = false; }
		if (keys[SDL_SCANCODE_SPACE])
		{
			space = true;
		}
		else { space = false; }
		if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))
		{
			left_click = true;
		}
		else { left_click = false; }
		if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT))
		{
			right_click = true;
		}
		if (keys[SDL_SCANCODE_1])
		{
			num = 1;
		}
		else if (keys[SDL_SCANCODE_2])
		{
			num = 2;
		}
		else if (keys[SDL_SCANCODE_3])
		{
			num = 3;
		}
		else if (keys[SDL_SCANCODE_4])
		{
			num = 4;
		}
		else
		{
			num = 0;
		}
		player_check_movement(W, A, S, D);
		entity_update_all(); //TODO: Make other ents not update when in a unactive scene

        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
            //gf2d_sprite_draw_image(sprite,vector2d(0,0));	
			scene_draw(scene_get_active());

			if (scene_get_active()->_arena)
			{
				scene_arena_draw(scene_get_active(), player_get_last());
			}

			room_manager_update();

			entity_draw_all();

			player_draw_follower();

			player_check_actions(left_click, right_click, space, mx, my, num);

			if (scene_get_active()->_arena)
			{
				scene_rim_draw(scene_get_active());
			}

            //UI elements last
            gf2d_sprite_draw(
                mouse,
                vector2d(mx,my),
                NULL,
                NULL,
                NULL,
                NULL,
                &mouseColor,
                (int)mf);
        gf2d_grahics_next_frame();// render current draw frame and skip to the next frame

        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition

		if (fpscheck >= 50)
		{
			slog("Rendering at %f FPS", avgfps/fpscheck);
			fpscheck = 0;
			avgfps = 0;
		}
		else
		{
			avgfps += gf2d_graphics_get_frames_per_second();
			fpscheck++;
		}
        
    }
	player_free(currentPlayer);

    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
