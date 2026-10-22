#include "game.h"

typedef struct {
    int radius;
    Position center;
} SpiderWeb;

void drawSpiderWeb(SpiderWeb *web, int world_width, int world_height, PlatformFunctions pFunctions) {
    //stright lines
    pFunctions.pDrawLine(
            web->center.x + web->radius, web->center.y - web->radius,
            web->center.x - web->radius, web->center.y + web->radius,
            GAME_BLACK
    );
    pFunctions.pDrawLine(
            web->center.x, web->center.y - web->radius,
            web->center.x, web->center.y + web->radius,
            GAME_BLACK
    );
    pFunctions.pDrawLine(
            web->center.x - web->radius, web->center.y - web->radius,
            web->center.x + web->radius, web->center.y + web->radius,
            GAME_BLACK
    );
    pFunctions.pDrawLine(
            web->center.x - web->radius,  web->center.y,
            web->center.x + web->radius,  web->center.y,
            GAME_BLACK
    );

    int sides = 8;
    int steps = web->radius/5;
    int poly_radius = steps;
    while (poly_radius < web->radius) {
        pFunctions.pDrawPoly(web->center.x, web->center.y, sides, poly_radius,  0, GAME_BLACK);
        poly_radius += steps;
    }
    //pFunctions.pDrawPoly(web->center.x, web->center.y, sides, 40,  0, GAME_BLACK);
    //pFunctions.pDrawPoly(web->center.x, web->center.y, sides, 60,  0, GAME_BLACK);
    //pFunctions.pDrawPoly(web->center.x, web->center.y, sides, 80,  0, GAME_BLACK);
    //pFunctions.pDrawPoly(web->center.x, web->center.y, sides, 100, 0, GAME_BLACK);
}

void drawSpider(Position pos, PlatformFunctions pFunctions) {
    int spider_body_size = 20;
    //body
    pFunctions.pDrawCircle(pos.x, pos.y, spider_body_size, GAME_BLACK);
    //head
    pFunctions.pDrawCircle(pos.x, pos.y - spider_body_size, 10, GAME_BLACK);

    //legs
    int separation = (spider_body_size*2 - 10) / 4;
    int current_y_offset  = 5;
    int spider_leg_size   = 10;
        while (current_y_offset < spider_body_size * 2) {
        int start_x = pos.x;
        int start_y = pos.y - spider_body_size + current_y_offset;
        pFunctions.pDrawLine(start_x, start_y, start_x - spider_body_size - spider_leg_size, start_y, GAME_BLACK);
        pFunctions.pDrawLine(start_x, start_y, start_x + spider_body_size + spider_leg_size, start_y, GAME_BLACK);

        current_y_offset += separation;
    }
}

void updateGame(GameMemory *game_memory, GameInput *game_input) {
    //TODO: implement
}

void renderGame(GameMemory *game_memory, PlatformFunctions pFunctions) {
    pFunctions.pClearBackground(GAME_WHITE);

    SpiderWeb web = {
        .radius = 200,
        .center = {
            game_memory->world_width/2.0f,
            game_memory->world_height/2.0f
        }
    };

    drawSpiderWeb(&web, game_memory->world_width, game_memory->world_height, pFunctions);
    drawSpider(web.center, pFunctions);
}

void updateAndRenderGame(GameMemory *game_memory, GameInput *game_input, PlatformFunctions pFunctions) {
    updateGame(game_memory, game_input);
    renderGame(game_memory, pFunctions);
}

