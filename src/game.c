#include <SDL.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"

int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    const Uint8 * keys;
    Sprite *sprite;
    
    int mx,my;
    float mf = 0;
    Sprite *mouse;
    Vector4D mouseColor = {255,100,255,200};
    
	//:)
	int lastmx, lastmy;
	int currmx, currmy;
	int dir = 0;
	char d;
	Vector2D scale = vector2d(3, 3);
	Vector2D scalePos = vector2d(0, 0);
	Vector2D flip = vector2d(1, 0);
	Vector2D *s = &scale;
	Vector2D *sp = &scalePos;
	Vector2D *f = &flip;
	Vector4D color = { 255, 255, 255, 255 };
	//:(

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
    
    /*demo setup*/
    sprite = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16);

	//:)
	mouse = gf2d_sprite_load_all("images/MyImages/IsoBoiSheet.png", 32, 32, 4);
	//:(

    /*main game loop*/
	while (!done)
	{
		SDL_PumpEvents();   // update SDL's internal event structures
		keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame

		/*update things here*/
		//SDL_GetMouseState(&mx,&my);

		//:)
		//Just messing around for a moment.
		SDL_GetMouseState(&currmx, &currmy);
		if (currmx != lastmx || currmy != lastmy)
		{
			mx = currmx;
			my = currmy;
		}
		else
		{
			if (keys[SDL_SCANCODE_W])
			{
				if (!keys[SDL_SCANCODE_S])
				{
					dir = 4;
				}
				if (my >= 3)
				{
					my -= 3;
				}
				else
				{
					my = 0;
				}
				if (keys[SDL_SCANCODE_A])
				{
					dir = 5;
				}
				else if (keys[SDL_SCANCODE_D])
				{
					dir = 3;
				}
			}
			if (keys[SDL_SCANCODE_A])
			{
				if (!keys[SDL_SCANCODE_W] && !keys[SDL_SCANCODE_S])
				{
					dir = 6;
				}
				if (mx >= 3)
				{
					mx -= 3;
				}
				else
				{
					mx = 0;
				}
			}
			if (keys[SDL_SCANCODE_S])
			{
				if (!keys[SDL_SCANCODE_W])
				{
					dir = 0;
				}
				if (my <= 685)
				{
					my += 3;
				}
				else
				{
					my = 688;
				}
				if (keys[SDL_SCANCODE_A])
				{
					dir = 7;
				}
				else if (keys[SDL_SCANCODE_D])
				{
					dir = 1;
				}
			}
			if (keys[SDL_SCANCODE_D])
			{
				if (!keys[SDL_SCANCODE_W] && !keys[SDL_SCANCODE_S])
				{
					dir = 2;
				}
				if (mx <= 1165)
				{
					mx += 3;
				}
				else
				{
					mx = 1168;
				}
			}

			if (!keys[SDL_SCANCODE_W] && !keys[SDL_SCANCODE_A] && !keys[SDL_SCANCODE_S] && !keys[SDL_SCANCODE_D])
			{
				dir = -1;
			}

			if (keys[SDL_SCANCODE_W] && keys[SDL_SCANCODE_S])
			{
				dir = -1;
			}

			if (keys[SDL_SCANCODE_A] && keys[SDL_SCANCODE_D])
			{
				dir = -1;
			}
		}
		//:(

        //mf+=0.1;
        //if (mf >= 16.0)mf = 0;
        //:)
		if (dir != -1)
		{
			mf = (dir * 4.0) + 0.1 + SDL_fmod(mf, 4.0);
			if (mf >= ((dir * 4.0) + 4.0))
			{
				mf = dir * 4.0;
			}
		}
		else
		{
			mf = (int)mf;
		}
		//:(
        
        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
            gf2d_sprite_draw_image(sprite,vector2d(0,0));
            
            //UI elements last
            /*gf2d_sprite_draw(
                mouse,
                vector2d(mx,my),
                NULL,
                NULL,
                NULL,
                NULL,
                &mouseColor,
                (int)mf);*/

			//:)
			gf2d_sprite_draw(
				mouse,
				vector2d(mx, my),
				s,
				sp,
				NULL,
				NULL,
				&color,
				(int)mf);
			//:(

        gf2d_grahics_next_frame();// render current draw frame and skip to the next frame
        
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());

		//:)
		SDL_GetMouseState(&lastmx, &lastmy);
		//:(
    }
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
