#include <SDL.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "gf2d_windows.h"
#include "my_entity.h"
#include "my_player.h"
#include "my_room.h"
#include "my_scene.h"
#include "my_follower.h"
#include "my_enemy.h"
#include "my_editor.h"
#include "gfc_audio.h"

int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    const Uint8 *keys;
    
	Uint8 menu = 1, edit = 0;

    int mx,my;
    float mf = 0;
    Sprite *mouse;
    Vector4D mouseColor = {255,100,255,200};
    
	Player *currentPlayer;
	Uint8 W, A, S, D, left_click, right_click, space, num, tab, q, e;

	int fpscheck = 0;
	float avgfps = 0;

	Window *team;
	Sprite *div, *sel;
	int t;
	Window *item;
	Window *loop;
	Window *status;
	char stat1[32], stat2[32], stat3[32], stat4[32], stat5[32];
	char *stat_name1, *stat_name2, *stat_name3, *stat_name4, *stat_name5;
	char stat_temp[64];

	char *ng = "New Game";
	Vector2D ngorigin, ngbounds;
	Uint8 nghover;

	char *ct = "Editor";
	Vector2D ctorigin, ctbounds;
	Uint8 cthover;

	char *ex = "Exit";
	Vector2D exorigin, exbounds;
	Uint8 exhover;

	Sprite *enmhp;
	Vector2D enmhporigin;

	Sprite *folhp;
	Vector2D folhporigin;

	Sprite *plahp;
	Vector2D plahporigin;

	Sound *bgtrack;

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
    
	gf2d_action_list_init(128);

	gf2d_windows_init(32);

	gf2d_text_init("config/font.cfg");

	entity_manager_init(64);

	scene_init();

	currentPlayer = player_new();
	if (currentPlayer == NULL)
	{
		return 0;
	}

	room_manager_init();

	gfc_audio_init(16, 4, 0, 8, 0, 0);

    /*demo setup*/
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16);
	
	team = gf2d_window_load("config/testwindow.cfg");
	div = gf2d_sprite_load_image("images/divider.png");
	sel = gf2d_sprite_load_image("images/selected.png");
	currentPlayer->window = vector4d(team->dimensions.w, team->dimensions.h, team->dimensions.x, team->dimensions.y);
	item = gf2d_window_load("config/bauble1.cfg");
	item->no_draw_generic = true;
	item->background = gf2d_sprite_load_image("images/bauble.png");
	loop = gf2d_window_load("config/bauble2.cfg");
	loop->no_draw_generic = true;
	loop->background = gf2d_sprite_load_image("images/bauble.png");
	status = gf2d_window_load("config/status.cfg");
	status->no_draw_generic = true;

	ngbounds = gf2d_text_get_bounds(ng, FT_H1);
	ngorigin.x = 600 - (ngbounds.x * 0.5);
	ngorigin.y = 310 + (ngbounds.y * 0.5);

	ctbounds = gf2d_text_get_bounds(ct, FT_H1);
	ctorigin.x = 600 - (ctbounds.x * 0.5);
	ctorigin.y = 370 + (ctbounds.y * 0.5);

	exbounds = gf2d_text_get_bounds(ex, FT_H1);
	exorigin.x = 600 - (exbounds.x * 0.5);
	exorigin.y = 430 + (exbounds.y * 0.5);

	enmhp = gf2d_sprite_load_all("images/battle_healthbar.png", 404, 20, 26);
	enmhporigin.x = 600 - 202;
	enmhporigin.y = 16;

	folhp = gf2d_sprite_load_all("images/battle_followerbar.png", 10, 44, 11);
	folhporigin.x = team->dimensions.x + team->dimensions.w + 4;
	folhporigin.y = team->dimensions.y;

	plahp = gf2d_sprite_load_all("images/battle_playerbar.png", 244, 20, 16);
	plahporigin.x = 4;
	plahporigin.y = 720 - 4 - 20;

	bgtrack = gfc_sound_load("sounds/Zone.wav", 0.05, -1);
	gfc_sound_play(bgtrack, 100, 0.05, -1, -1);
    /*main game loop*/
	while (!done)
	{
		SDL_PumpEvents();   // update SDL's internal event structures
		keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
		/*update things here*/
		SDL_GetMouseState(&mx, &my);
		mf += 0.1;
		if (mf >= 16.0)mf = 0;

		if (menu)
		{
			gf2d_graphics_clear_screen();

			//Check if they are hovering over new game
			if (mx >= ngorigin.x && mx <= ngorigin.x + ngbounds.x &&
				my >= ngorigin.y && my <= ngorigin.y + ngbounds.y)
			{
				nghover = true;
				gf2d_text_draw_line(ng, FT_H1, gfc_color(0.65, 0.8, 1, 1), ngorigin);
			}
			else
			{
				nghover = false;
				gf2d_text_draw_line(ng, FT_H1, gfc_color(1, 1, 1, 1), ngorigin);
			}

			//Check if they are hovering over continue
			if (mx >= ctorigin.x && mx <= ctorigin.x + ctbounds.x &&
				my >= ctorigin.y && my <= ctorigin.y + ctbounds.y)
			{
				cthover = true;
				gf2d_text_draw_line(ct, FT_H1, gfc_color(0.65, 0.8, 1, 1), ctorigin);
			}
			else
			{
				cthover = false;
				gf2d_text_draw_line(ct, FT_H1, gfc_color(1, 1, 1, 1), ctorigin);
			}

			//Check if they are hovering over exit
			if (mx >= exorigin.x && mx <= exorigin.x + exbounds.x &&
				my >= exorigin.y && my <= exorigin.y + exbounds.y)
			{
				exhover = true;
				gf2d_text_draw_line(ex, FT_H1, gfc_color(0.65, 0.8, 1, 1), exorigin);
			}
			else
			{
				exhover = false;
				gf2d_text_draw_line(ex, FT_H1, gfc_color(1, 1, 1, 1), exorigin);
			}

			gf2d_sprite_draw(
				mouse,
				vector2d(mx, my),
				NULL,
				NULL,
				NULL,
				NULL,
				&mouseColor,
				(int)mf);

			if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))
			{
				left_click = true;
			}
			else { left_click = false; }

			if (left_click && nghover)
			{
				if (currentPlayer->health <= 0)
				{
					entity_manager_init(64);
					scene_init();
					currentPlayer = player_new();
					room_manager_init();
				}
				menu = false;
			}
			else if (left_click && cthover)
			{
				editor_new_map();
				edit = true;
				menu = false;
			}
			else if (left_click && exhover)
			{
				done = 1;
			}

			if (fpscheck >= 50)
			{
				slog("Rendering at %f FPS", avgfps / fpscheck);
				fpscheck = 0;
				avgfps = 0;
			}
			else
			{
				avgfps += gf2d_graphics_get_frames_per_second();
				fpscheck++;
			}

			gf2d_grahics_next_frame();

			continue;
		}

		if (edit)
		{
			gf2d_graphics_clear_screen();

			if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))
			{
				left_click = true;
			}
			else { left_click = false; }

			editor_update(mx, my, left_click);

			if (keys[SDL_SCANCODE_ESCAPE])
			{
				editor_save_map();
				menu = 1;
			}

			gf2d_sprite_draw(
				mouse,
				vector2d(mx, my),
				NULL,
				NULL,
				NULL,
				NULL,
				&mouseColor,
				(int)mf);

			gf2d_grahics_next_frame();

			continue;
		}

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
		else { right_click = false; }
		if (keys[SDL_SCANCODE_TAB])
		{
			tab = true;
		}
		else { tab = false; }
		if (keys[SDL_SCANCODE_Q])
		{
			q = true;
		}
		else { q = false; }
		if (keys[SDL_SCANCODE_E])
		{
			e = true;
		}
		else { e = false; }
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

		gf2d_windows_update_all();

        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
			scene_draw(scene_get_active());

			if (scene_get_active()->_arena)
			{
				scene_arena_draw(scene_get_active(), player_get_last());
			}

			room_manager_update();

			entity_draw_all();

			player_draw_follower();

			player_check_actions(left_click, right_click, space, mx, my, num, tab, q, e);

			if (currentPlayer->show_status)
			{
				status->no_draw_generic = false;
			}
			else
			{
				status->no_draw_generic = true;
			}

			if (scene_get_active()->_arena)
			{
				scene_rim_draw(scene_get_active());
			}
			
            //UI elements last
			gf2d_windows_draw_all();
			if (currentPlayer->show_status)
			{
				gf2d_text_draw_line(currentPlayer->team[currentPlayer->selected - 1]->name, FT_H1, gfc_color(1, 1, 1, 1), vector2d(status->dimensions.x + 10, status->dimensions.y + 5));
				memset(stat_temp, 0, sizeof stat_temp);
				sprintf(stat1, "%d/%d", currentPlayer->team[currentPlayer->selected - 1]->health, currentPlayer->team[currentPlayer->selected - 1]->health_max);
				stat_name1 = "Health: ";
				strcat(stat_temp, stat_name1);
				strcat(stat_temp, stat1);
				
				gf2d_text_draw_line(stat_temp, FT_H1, gfc_color(1, 1, 1, 1), vector2d(status->dimensions.x + 10, status->dimensions.y + 105));
				
				memset(stat_temp, 0, sizeof stat_temp);
				sprintf(stat2, "%d", currentPlayer->team[currentPlayer->selected - 1]->attack);
				stat_name2 = "Attack: ";
				strcat(stat_temp, stat_name2);
				strcat(stat_temp, stat2);

				gf2d_text_draw_line(stat_temp, FT_H1, gfc_color(1, 1, 1, 1), vector2d(status->dimensions.x + 10, status->dimensions.y + 155));
				
				memset(stat_temp, 0, sizeof stat_temp);
				sprintf(stat3, "%d", currentPlayer->team[currentPlayer->selected - 1]->magic);
				stat_name3 = "Magic: ";
				strcat(stat_temp, stat_name3);
				strcat(stat_temp, stat3);

				gf2d_text_draw_line(stat_temp, FT_H1, gfc_color(1, 1, 1, 1), vector2d(status->dimensions.x + 10, status->dimensions.y + 205));
				
				memset(stat_temp, 0, sizeof stat_temp);
				sprintf(stat4, "%d", currentPlayer->team[currentPlayer->selected - 1]->defense);
				stat_name4 = "Defense: ";
				strcat(stat_temp, stat_name4);
				strcat(stat_temp, stat4);

				gf2d_text_draw_line(stat_temp, FT_H1, gfc_color(1, 1, 1, 1), vector2d(status->dimensions.x + 10, status->dimensions.y + 255));
				
				memset(stat_temp, 0, sizeof stat_temp);
				sprintf(stat5, "%d", currentPlayer->team[currentPlayer->selected - 1]->speed);
				stat_name5 = "Speed: ";
				strcat(stat_temp, stat_name5);
				strcat(stat_temp, stat5);

				gf2d_text_draw_line(stat_temp, FT_H1, gfc_color(1, 1, 1, 1), vector2d(status->dimensions.x + 10, status->dimensions.y + 305));
			}
			gf2d_sprite_draw_image(item->background, vector2d(item->dimensions.x, item->dimensions.y));
			gf2d_sprite_draw_image(currentPlayer->items[currentPlayer->item_1].icon, vector2d(item->dimensions.x, item->dimensions.y));
			gf2d_sprite_draw_image(loop->background, vector2d(loop->dimensions.x, loop->dimensions.y));
			gf2d_sprite_draw_image(currentPlayer->items[currentPlayer->item_2].icon, vector2d(loop->dimensions.x, loop->dimensions.y));
			gf2d_sprite_draw_image(div, vector2d(team->dimensions.x, team->dimensions.y + 48));
			gf2d_sprite_draw_image(div, vector2d(team->dimensions.x, team->dimensions.y + 96));
			gf2d_sprite_draw_image(div, vector2d(team->dimensions.x, team->dimensions.y + 144));
			if (currentPlayer->battle && currentPlayer->next_boss)
			{
				gf2d_sprite_draw(
					enmhp,
					enmhporigin,
					NULL,
					NULL,
					NULL,
					NULL,
					NULL,
					((currentPlayer->enemy_health_max - currentPlayer->enemy_health) * 25)/currentPlayer->enemy_health_max);
			}
			else if (currentPlayer->battle)
			{
				gf2d_sprite_draw(
					enmhp,
					enmhporigin,
					NULL,
					NULL,
					NULL,
					NULL,
					NULL,
					currentPlayer->enemy_health_max - currentPlayer->enemy_health);
			}
				gf2d_sprite_draw(
					folhp,
					folhporigin,
					NULL,
					NULL,
					NULL,
					NULL,
					NULL,
					((currentPlayer->team[0]->health_max - currentPlayer->team[0]->health) * 10) / currentPlayer->team[0]->health_max);
		
				gf2d_sprite_draw(
					plahp,
					plahporigin,
					NULL,
					NULL,
					NULL,
					NULL,
					NULL,
					currentPlayer->health_max - currentPlayer->health);

			for (t = 0; t < 4; t++)
			{
				if (currentPlayer->selected == t + 1 || currentPlayer->picked == t + 1)
				{
					gf2d_sprite_draw_image(sel, vector2d(team->dimensions.x, team->dimensions.y + (t * 48)));
				}
				gf2d_sprite_draw_image(currentPlayer->team[t]->sprite, vector2d(team->dimensions.x, team->dimensions.y + (t * 48)));
			}

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
		
		if (keys[SDL_SCANCODE_ESCAPE])
		{
			menu = 1;
		}

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
		
		if (currentPlayer->health <= 0)
		{
			player_free(currentPlayer);
			room_manager_close();
			scene_free();
			entity_manager_close();
			menu = true;
		}

    }
	player_free(currentPlayer);
	
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
