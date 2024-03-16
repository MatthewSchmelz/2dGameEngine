#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "entity.h"
#include "gf2d_draw.h"
#include "font.h"
#include "gfc_input.h"
#include "shop.h"


//We have 1200 Width and 720 height

extern int blast,teleport,stomp,shield,snap,gas,spike,healing,fog, walls;

void shop() {
	//Grab the mouse stte
	int mx, my;
	//Basic Shop Screen
	gf2d_draw_rect_filled(gfc_rect(0, 0, 10000, 10000), GFC_COLOR_DARKGREY);
	gf2d_draw_rect_filled(gfc_rect(0, 0, 10000, 10000), GFC_COLOR_GREY);
	font_draw_test("Shop", FS_large, GFC_COLOR_WHITE, vector2d(550, 20));

	SDL_GetMouseState(&mx, &my);
	
	//First button at 50 down 20 right, length 200 height 30	slog("failed to render text %s, missing font style %i", mx, my);
	//Give an extra 10 pixel leeway each way

	//Player Abilities

	if (mx > 40 && mx < 250 && my > 60 && my < 100) {
		gf2d_draw_rect_filled(gfc_rect(40, 60, 240, 90), GFC_COLOR_RED);
		if (gfc_input_command_pressed("buy")) {
			slog("Ability purchased");
			blast = 1;
		}
	}
	else {
		gf2d_draw_rect_filled(gfc_rect(40, 60, 240, 90), GFC_COLOR_DARKGREY);
	}
	font_draw_test("Blast MK 2", FS_large, GFC_COLOR_WHITE, vector2d(50, 80));

	if (mx > 40 && mx < 250 && my > 160 && my < 250) {
		gf2d_draw_rect_filled(gfc_rect(40, 160, 240, 90), GFC_COLOR_RED);
		if (gfc_input_command_pressed("buy")) {
			slog("Ability purchased");
			teleport = 1;
		}
	}
	else {
		gf2d_draw_rect_filled(gfc_rect(40, 160, 240, 90), GFC_COLOR_DARKGREY);
	}
	font_draw_test("Teleport", FS_large, GFC_COLOR_WHITE, vector2d(50, 180));

	if (mx > 40 && mx < 250 && my > 260 && my < 350) {
		gf2d_draw_rect_filled(gfc_rect(40, 260, 240, 90), GFC_COLOR_RED);
		if (gfc_input_command_pressed("buy")) {
			slog("Ability purchased");
			stomp = 1;
		}
	}
	else {
		gf2d_draw_rect_filled(gfc_rect(40, 260, 240, 90), GFC_COLOR_DARKGREY);
	}
	font_draw_test("Stomp", FS_large, GFC_COLOR_WHITE, vector2d(50, 280));

	if (mx > 40 && mx < 250 && my > 360 && my < 450) {
		gf2d_draw_rect_filled(gfc_rect(40, 360, 240, 90), GFC_COLOR_RED);
		if (gfc_input_command_pressed("buy")) {
			slog("Ability purchased");
			shield = 1;
		}
	}
	else {
		gf2d_draw_rect_filled(gfc_rect(40, 360, 240, 90), GFC_COLOR_DARKGREY);
	}
	font_draw_test("Soul Shield", FS_large, GFC_COLOR_WHITE, vector2d(50, 380));

	if (mx > 40 && mx < 250 && my > 460 && my < 550) {
		gf2d_draw_rect_filled(gfc_rect(40, 460, 240, 90), GFC_COLOR_RED);
		if (gfc_input_command_pressed("buy")) {
			slog("Ability purchased");
			snap = 1;
		}
	}
	else {
		gf2d_draw_rect_filled(gfc_rect(40, 460, 240, 90), GFC_COLOR_DARKGREY);
	}
	font_draw_test("String Snap", FS_large, GFC_COLOR_WHITE, vector2d(50, 480));


	//Dungeon Abilities

	if (mx > 740 && mx < 950 && my > 60 && my < 100) {
		gf2d_draw_rect_filled(gfc_rect(740, 60, 240, 90), GFC_COLOR_RED);
		if (gfc_input_command_pressed("buy")) {
			slog("Ability purchased");
			gas = 1;
		}
	}
	else {
		gf2d_draw_rect_filled(gfc_rect(740, 60, 240, 90), GFC_COLOR_DARKGREY);
	}
	font_draw_test("Poison Gas", FS_large, GFC_COLOR_WHITE, vector2d(750, 80));

	if (mx > 740 && mx < 950 && my > 160 && my < 250) {
		gf2d_draw_rect_filled(gfc_rect(740, 160, 240, 90), GFC_COLOR_RED);
		if (gfc_input_command_pressed("buy")) {
			slog("Ability purchased");
			spike = 1;
		}
	}
	else {
		gf2d_draw_rect_filled(gfc_rect(740, 160, 240, 90), GFC_COLOR_DARKGREY);
	}
	font_draw_test("Spike Floor", FS_large, GFC_COLOR_WHITE, vector2d(750, 180));

	if (mx > 740 && mx < 950 && my > 260 && my < 350) {
		gf2d_draw_rect_filled(gfc_rect(740, 260, 240, 90), GFC_COLOR_RED);
		if (gfc_input_command_pressed("buy")) {
			slog("Ability purchased");
			healing = 1;
		}
	}
	else {
		gf2d_draw_rect_filled(gfc_rect(740, 260, 240, 90), GFC_COLOR_DARKGREY);
	}
	font_draw_test("Regen Arena", FS_large, GFC_COLOR_WHITE, vector2d(750, 280));

	if (mx > 740 && mx < 950 && my > 360 && my < 450) {
		gf2d_draw_rect_filled(gfc_rect(740, 360, 240, 90), GFC_COLOR_RED);
		if (gfc_input_command_pressed("buy")) {
			slog("Ability purchased");
			fog = 1;
		}
	}
	else {
		gf2d_draw_rect_filled(gfc_rect(740, 360, 240, 90), GFC_COLOR_DARKGREY);
	}
	font_draw_test("Fog Gates", FS_large, GFC_COLOR_WHITE, vector2d(750, 380));

	if (mx > 740 && mx < 950 && my > 460 && my < 550) {
		gf2d_draw_rect_filled(gfc_rect(740, 460, 240, 90), GFC_COLOR_RED);
		if (gfc_input_command_pressed("buy")) {
			slog("Ability purchased");
			walls = 1;
		}
	}
	else {
		gf2d_draw_rect_filled(gfc_rect(740, 460, 240, 90), GFC_COLOR_DARKGREY);
	}
	font_draw_test("Confusion", FS_large, GFC_COLOR_WHITE, vector2d(750, 480));

}