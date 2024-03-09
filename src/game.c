#include <SDL.h>
#include "simple_logger.h"
#include "gfc_input.h"

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"

#include "gf2d_draw.h"

#include "camera.h"
#include "font.h"
#include "entity.h"
#include "player.h"
#include "world.h"
#include "e_fighter.h"
#include "e_bullet.h"
#include "shop.h"


Entity* player = NULL;
Entity* fighter = NULL;
Vector2D mLoc;


#define MAX_FIGHTERS 10 // Adjust the maximum number of fighters as needed
Entity* fighters[MAX_FIGHTERS] = { NULL }; // Array to store pointers to fighters

#define MAX_BULLETS 1000 // Adjust the maximum number of fighters as needed
Entity* bullets[MAX_BULLETS] = { NULL }; // Array to store pointers to fighters


//Player Abilities bool
int blast = 0, teleport = 0, stomp = 0, shield = 0, snap = 0;
//Map Abilities 
int gas = 0, spike = 0, healing = 0, fog = 0, walls =0;



Uint32 callback_mob(Uint32 interval, void* param) {
    // Find an empty slot in the array
    int emptySlot = -1;
    for (int i = 0; i < MAX_FIGHTERS; ++i) {
        if (fighters[i] == NULL) {
            emptySlot = i;
            break;
        }
    }

    // Create a new fighter if there's an empty slot
    if (emptySlot != -1) {
        fighters[emptySlot] = fighter_new();
    }


    return interval; // Return the interval for the timer
}




int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    const Uint8 * keys;
    //Sprite *sprite;
    World* world;
    int souls = 0;
    int bshop = 0;
    int mx,my;
    float mf = 0;
    Sprite *mouse;
    int highscore;
    int currScore =0;
    Color mouseColor = gfc_color8(255,100,255,200);
    

    //Yoink the highscore from the json file
    SJson* json, * config;
    json = sj_load("config/stats.json");
    if (!sj_object_get_value_as_int(json, "highscore", &highscore)) {
        slog("Cannot find highscore");
    }


    /*program initializtion*/
    init_logger("gf2d.log",0);
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
    gf2d_sprite_init(10000);
    font_init();
    entity_system_init(10000);
    
    SDL_ShowCursor(SDL_DISABLE);
    camera_set_size(vector2d(1200, 700));
    


    gfc_input_init("config/input.cfg");
    //Timers for Mobs
    SDL_TimerID timerTime = SDL_AddTimer(2000, callback_mob, NULL);
    
    /*demo setup*/
    //sprite = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
    world = world_load("maps/dungeon.json");
    world_setup_camera(world);
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16,0);
    player = player_new();
    /*main game loop*/
    while(!done)
    {
        gfc_input_update();
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        font_cleanup();
        /*update things here*/
        //Handling the Mouse
        SDL_GetMouseState(&mx,&my);
        mf+=0.1;
        if (mf >= 16.0)mf = 0;

        mLoc.x = mx;
        mLoc.y = my;


        entity_system_think();
        entity_system_update();
        for (int i = 0; i < MAX_FIGHTERS; ++i) {
            if (fighters[i] != NULL) {
                fighter_pursue(fighters[i], player);
                //Check if player has been collied with
                if (gfc_circle_overlap(fighters[i]->hitbox, player->hitbox)) {
                    fighter_free(fighters[i]);
                    fighters[i] = NULL;

                    //Damage the player here

                }
            }
        }
        
    



        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
        //backgrounds drawn first
        //gf2d_sprite_draw_image(sprite,vector2d(0,0));
        world_draw(world);
            
        entity_system_draw();



       //Shop handling
        if (bshop) {
            shop(player);
        
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







        //UI elements last
        gf2d_draw_rect_filled(gfc_rect(0, 630, 10000, 200), GFC_COLOR_DARKGREY);
        gf2d_draw_rect_filled(gfc_rect(0, 640, 10000, 200), GFC_COLOR_GREY);
        font_draw_test("Health", FS_small, GFC_COLOR_WHITE, vector2d(10, 650));
        //Draw health bar
        switch (player->health) {
        case 2:
            gf2d_draw_rect_filled(gfc_rect(140, 655, 30, 30), GFC_COLOR_RED);
        case 1:
            gf2d_draw_rect_filled(gfc_rect(180, 655, 30, 30), GFC_COLOR_RED);
            break;
        default:
            break;
        }
        font_draw_test("Souls", FS_small, GFC_COLOR_WHITE, vector2d(300, 650));
        //Currency Available to the player
        char soulString[50];
        sprintf(soulString, "%d", souls);
        font_draw_test(soulString, FS_small, GFC_COLOR_WHITE, vector2d(420, 650));
        //
        font_draw_test("High Score", FS_small, GFC_COLOR_WHITE, vector2d(500, 650));
        //Total Fighters Killed overall(Load from config)
        char scoreString[50];
        sprintf(scoreString, "%d", highscore);
        font_draw_test(scoreString, FS_small, GFC_COLOR_WHITE, vector2d(700, 650));
        //Current Score for player
        font_draw_test("Score", FS_small, GFC_COLOR_WHITE, vector2d(800, 650));
        //Total Fighters Killed overall(Load from config)
        char curString[50];
        sprintf(curString, "%d", currScore);
        font_draw_test(curString, FS_small, GFC_COLOR_WHITE, vector2d(960, 650));

      

        //Shop Init
        if (gfc_input_command_pressed("shop")) {
            //Swap shop boolean
            switch (bshop) {
            case 0:
                bshop = 1;
                break;
            case 1:
                bshop = 0;
                break;
            }
        }
        //Bullet Firing
        if (gfc_input_command_pressed("fire")){
            
            int emptySlot = -1;
            for (int i = 0; i < MAX_BULLETS; ++i) {
                if (bullets[i] == NULL) {
                    emptySlot = i;
                    break;
                }
            }

            // Create a new fighter if there's an empty slot
            if (emptySlot != -1) {
                
                bullets[emptySlot] = bullet_new(player, mLoc);
            }
            else {
                slog("Bullet Array Full!");
            }

        }
        //Other abilites here


        //The Great bullet Collide Loop 
   

      
        for (int i = 0; i < MAX_BULLETS; ++i) {
            if (bullets[i] != NULL) {
                //For every bullet on the field, we must check every entity of the opposing team to see if it collides
                for (int j = 0; j < MAX_FIGHTERS; j++) {
                    if (fighters[j] != NULL) {

                        if(check_collision(bullets[i], fighters[j])){
                            slog("Found Collision");
                            fighter_free(fighters[j]);
                            bullet_free(bullets[i]);
                            fighters[j] = NULL;
                            bullets[i] = NULL;
                            currScore++;
                        }
                    }
                }
                   
            }
        }

        //Fix later 
        //Check to see if bullets are out of range
        for (int i = 0; i < MAX_BULLETS; ++i) {
            if (bullets[i] != NULL) {
                //For every bullet on the field, we must check every entity of the opposing team to see if it collides
                if ((bullets[i]->position.x < 0 || bullets[i]->position.x > 1200) || (bullets[i]->position.y < 0 || bullets[i]->position.y > 1500)) {
                    slog("bullet out of play area, cleaning up.");
                    bullet_free(bullets[i]);
                    bullets[i] = NULL;
                }
            }
        }
        
   



        gf2d_graphics_next_frame();// render current draw frame and skip to the next frame
        
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }

    SJson* temp;

    SJson* locations = sj_object_new();;
    //We are exiting! Check to see if the player has a new high score

    temp = sj_new_int(currScore);
    sj_object_insert(locations, "highscore", temp);

    sj_save(locations, "config/stats.json");



    entity_free(player);
    world_free(world);
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
