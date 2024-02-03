#include <SDL.h>
#include "simple_logger.h"

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "entity.h"
#include "player.h"
#include "e_fighter.h"

\
Entity* player = NULL;
Entity* fighter = NULL;


#define MAX_FIGHTERS 10 // Adjust the maximum number of fighters as needed
Entity* fighters[MAX_FIGHTERS] = { NULL }; // Array to store pointers to fighters


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

    // Code for other functionalities...

    return interval; // Return the interval for the timer
}




int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    const Uint8 * keys;
    Sprite *sprite;
    
    int mx,my;
    float mf = 0;
    Sprite *mouse;
    Color mouseColor = gfc_color8(255,100,255,200);
    
    
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
    gf2d_sprite_init(1024);
    entity_system_init(1024);
    SDL_ShowCursor(SDL_DISABLE);

    //Timers for Mobs
    SDL_TimerID timerTime = SDL_AddTimer(2000, callback_mob, NULL);
    
    /*demo setup*/
    sprite = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16,0);
    player = player_new();
    /*main game loop*/
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        SDL_GetMouseState(&mx,&my);
        mf+=0.1;
        if (mf >= 16.0)mf = 0;
        entity_system_think();
        entity_system_update();
        for (int i = 0; i < MAX_FIGHTERS; ++i) {
            if (fighters[i] != NULL) {
                fighter_pursue(fighters[i], player);
            }
        }
        
        

        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
            gf2d_sprite_draw_image(sprite,vector2d(0,0));
            
            entity_system_draw();

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

        gf2d_graphics_next_frame();// render current draw frame and skip to the next frame
        
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }

    entity_free(player);
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
