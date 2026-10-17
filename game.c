#include "game.h"

void drawSpiderWeb(int world_width, int world_height, PlatformFunctions pFunctions) {
    int web_radius = 100;
    Position center = {
        world_width - web_radius,
        0           + web_radius
    };

    //stright lines
    pFunctions.pDrawLine(
            world_width, 0,
            center.x - web_radius,    center.y + web_radius,
            GAME_BLACK
    );
    pFunctions.pDrawLine(
            world_width - web_radius, 0,
            center.x,                 center.y + web_radius,
            GAME_BLACK
    );
    pFunctions.pDrawLine(
            center.x - web_radius, 0,
            center.x + web_radius, center.y + web_radius,
            GAME_BLACK
    );
    pFunctions.pDrawLine(
            center.x - web_radius,  web_radius,
            center.x + web_radius,              center.y,
            GAME_BLACK
    );

    int sides = 8;
    pFunctions.pDrawPoly(center.x, center.y, sides, 20,  0, GAME_BLACK);
    pFunctions.pDrawPoly(center.x, center.y, sides, 40,  0, GAME_BLACK);
    pFunctions.pDrawPoly(center.x, center.y, sides, 60,  0, GAME_BLACK);
    pFunctions.pDrawPoly(center.x, center.y, sides, 80,  0, GAME_BLACK);
    pFunctions.pDrawPoly(center.x, center.y, sides, 100, 0, GAME_BLACK);
}

void updateGame(GameMemory *game_memory, GameInput *game_input) {
    //TODO: implement
}

void renderGame(GameMemory *game_memory, PlatformFunctions pFunctions) {
    pFunctions.pClearBackground(GAME_WHITE);
    drawSpiderWeb(game_memory->world_width, game_memory->world_height, pFunctions);
}

void updateAndRenderGame(GameMemory *game_memory, GameInput *game_input, PlatformFunctions pFunctions) {
    updateGame(game_memory, game_input);
    renderGame(game_memory, pFunctions);
}

