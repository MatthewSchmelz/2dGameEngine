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
#include "e_mage.h"
#include "e_rogue.h"
#include "e_tank.h"
#include "e_paladin.h"
#include "e_archmage.h"
#include "e_hero.h"
#include "e_barbarian.h"
#include "e_ninja.h"
#include "e_shadowmage.h"
#include "e_bullet.h"
#include "shop.h"


Entity* player = NULL;
Entity* fighter = NULL;
Vector2D mLoc;


#define MAX_FIGHTERS 1000 // Adjust the maximum number of fighters as needed
Entity* fighters[MAX_FIGHTERS] = { NULL }; // Array to store pointers to fighters

#define MAX_BULLETS 1000 // Adjust the maximum number of fighters as needed
Entity* bullets[MAX_BULLETS] = { NULL }; // Array to store pointers to fighters


//Player Abilities Owned bool
int blast = 0, teleport = 0, stomp = 0, shield = 0, snap = 0;
//Map Abilities 
int gas = 0, spike = 0, healing = 0, fog = 0, walls =0;
//player abilities activated bools
int invuln = 0, stompOn = 0;

int pet =0;

int summontype =0;

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
        //Random spawn in fighters
        switch (summontype) {
        case 0:
            fighters[emptySlot] = fighter_new();
            summontype++;
            break;
        case 1:
            fighters[emptySlot] = mage_new();
            summontype++;
            break;
        case 2:
            fighters[emptySlot] = rogue_new();
            summontype++;
            break;
        case 3:
            fighters[emptySlot] = tank_new();
            summontype++;
            break;
        case 4:
            fighters[emptySlot] = paladin_new();
            summontype++;
            break;
        case 5:
            fighters[emptySlot] = archmage_new();
            summontype++;
            break;
        case 6:
            fighters[emptySlot] = hero_new();
            summontype++;
            break;
        case 7:
            fighters[emptySlot] = barbarian_new();
            summontype++;
            break;
        case 8:
            fighters[emptySlot] = ninja_new();
            summontype++;
            break;
        case 9:
            fighters[emptySlot] = shadowmage_new();
            summontype++;
            break;
        case 10:
            summontype = 0;
            break;
        default:
            summontype = 0;
            break;

        }

    }


    return interval; // Return the interval for the timer
}

//Callsback the shield function to make the player stop being immune
Uint32 callback_shield(Uint32 interval, void* param) {
    invuln = 0;
    return 0;
}

Uint32 callback_stomp(Uint32 interval, void* param) {
    stompOn = 0;
    invuln = 0;
    return 0;
}

Uint32 callback_poison(Uint32 interval, void* param) {
   
    for (int j = 0; j < MAX_FIGHTERS; j++) {
        if (fighters[j] != NULL) {
            if (gfc_crandom() > 0.6) {
                entity_damage(fighters[j]);
                
            }

        }
    }
    return interval;
}

Uint32 callback_regen(Uint32 interval, void* param) {

    if (player->health < 2) {
        player->health++;
    }
    return interval;
}

Uint32 callback_pet(Uint32 interval, void* param) {
    pet = 0;
    slog("enemies released");

    return 0;

}

Uint32 callback_stun(Uint32 interval, void* param) {
    pet = 1;
    slog("Enemies stunned");
    SDL_TimerID timerStun = SDL_AddTimer(5000, callback_pet, NULL);

    return interval;
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

        //Implement the perfifying
        if (!pet) {
            for (int i = 0; i < MAX_FIGHTERS; ++i) {
                if (fighters[i] != NULL) {

                    entity_pursue(fighters[i], player);
                    //Check if player has been collied with
                    if (gfc_circle_overlap(fighters[i]->hitbox, player->hitbox)) {
                        fighter_free(fighters[i]);
                        fighters[i] = NULL;

                        //Damage the player here

                    }
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
        if (gas) {
            gas = 0;
            SDL_TimerID timerGas = SDL_AddTimer(4000, callback_poison, NULL);
        }
        if (healing) {
            healing = 0;
            SDL_TimerID timerHeal = SDL_AddTimer(10000, callback_regen, NULL);
        }
        if (walls) {
            walls = 0;
            SDL_TimerID timerPet = SDL_AddTimer(20000, callback_stun, NULL);
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

            gf2d_draw_rect_filled(gfc_rect(180, 655, 30, 30), GFC_COLOR_RED);
        case 1:
           
            gf2d_draw_rect_filled(gfc_rect(140, 655, 30, 30), GFC_COLOR_RED);
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
        //Simply move
        if (gfc_input_command_pressed("teleport")) {
            slog("Player Teleported");
            player->position.x = player->position.x + (gfc_crandom()*200);
            player->position.y = player->position.y + (gfc_crandom()*200);

        }
        //double size of hitbox, remove damage and kill enemies inside?
        if (gfc_input_command_pressed("stomp")) {
            slog("Player Stomped");
            stompOn = 1;
            invuln = 1;
            SDL_TimerID timerStomp = SDL_AddTimer(1000, callback_stomp, NULL);

        }
        //Remove collision box
        if (gfc_input_command_pressed("shield")) {
            slog("Player Invlunerable");
            SDL_TimerID timerShield = SDL_AddTimer(5000, callback_shield, NULL);
            invuln = 1;
        }
        //Kill random enemy on the array
        if (gfc_input_command_pressed("snap")) {
            slog("Player Snapped");
            for (int j = 0; j < MAX_FIGHTERS; j++) {
                if (fighters[j] != NULL) {
                    entity_damage(fighters[j]);
                }
            }


        }

        //Hazard Checking


        //The Great bullet Collide Loop 
   

      
        for (int i = 0; i < MAX_BULLETS; ++i) {
            if (bullets[i] != NULL) {
                //For every bullet on the field, we must check every entity of the opposing team to see if it collides
                for (int j = 0; j < MAX_FIGHTERS; j++) {
                    if (fighters[j] != NULL) {

                        if(check_collision(bullets[i], fighters[j])){
                            slog("Found Collision");
                            
                            
                            entity_damage(fighters[j]);
                            if (!fighters[j]) {
                                fighters[j] = NULL;
                                currScore++;
                            }
                            else {
                                if (blast == 1) {
                                    entity_damage(fighters[j]);
                                    if (!fighters[j]) {
                                        fighters[j] = NULL;
                                        currScore++;
                                    }
                                }
                            }
                            
                            bullet_free(bullets[i]);
                            bullets[i] = NULL;
                            
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
