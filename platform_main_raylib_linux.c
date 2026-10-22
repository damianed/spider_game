#include <stdio.h>
#include "raylib.h"
#include "rlgl.h"
#include "game.h"
#include <dlfcn.h>
#include <time.h>
#include <sys/stat.h>

#define GAME_SO_PATH "./build/game.so"

static updateAndRenderGame_t UPDATE_AND_RENDER_GAME_FUNC;
static void *GAME_LIB_PTR;

bool loadGameLib() {
	GAME_LIB_PTR = dlopen(GAME_SO_PATH, RTLD_NOW);
	if (GAME_LIB_PTR != NULL) {
		char *function_name = "updateAndRenderGame";
		UPDATE_AND_RENDER_GAME_FUNC = (updateAndRenderGame_t) dlsym(GAME_LIB_PTR, function_name);
		if (!UPDATE_AND_RENDER_GAME_FUNC) {
			printf("Could not find %s symbol in %s shared library\n", function_name, GAME_SO_PATH);
			return 0;
		}
	} else {
		printf("Cound not load %s\n error: %s\n", GAME_SO_PATH, dlerror());
		return 0;
	}

	return 1;
}

int unloadGameLib() {
	if (GAME_LIB_PTR) {
		dlclose(GAME_LIB_PTR);
	}
	return 0;
}

struct timespec getFileLastMod(char *filepath) {
	struct stat statbuf = {0};
	stat(filepath, &statbuf);

	return statbuf.st_mtim;
}

int wasFileModified(char *filepath, struct timespec since) {
	struct timespec new_mod_time = getFileLastMod(filepath);
	if (since.tv_sec == new_mod_time.tv_sec) {
		return since.tv_nsec < new_mod_time.tv_nsec;
	}

	return since.tv_sec < new_mod_time.tv_sec;
}

void reloadGameLibIfChanged(struct timespec *since_out) {
    if (wasFileModified(GAME_SO_PATH, *since_out)) {
        printf("Game lib modified reloading...\n");
        unloadGameLib();
        UPDATE_AND_RENDER_GAME_FUNC = NULL;
#define MAX_TRIES 100
        int tries = 0;
        while (!loadGameLib()) {
            if (tries++ >= MAX_TRIES) {
                fprintf(stderr, "Failed to reload game lib, max tries exceeded\n");
                return;
            }
        }
        printf("Game lib successfully reloaded\n");
        *since_out = getFileLastMod(GAME_SO_PATH);
        return;
    }

    return;
}

static inline Color gameColorToRaylib(GameColor color) {
    return *((Color *)(&color));
}

void platformClearBackground(GameColor color) {
    ClearBackground(gameColorToRaylib(color));
}

void platformDrawCircle(int center_x, int center_y, float radius, GameColor color) {
    DrawCircle(center_x, center_y, radius, gameColorToRaylib(color));
}

void platformDrawText(char *text, int pos_x, int pos_y, int font_size, GameColor color) {
    DrawText(text, pos_x, pos_y, font_size, gameColorToRaylib(color));
}

void platformDrawLine(int start_x, int start_y, int end_x, int end_y, GameColor color) {
    DrawLine(start_x, start_y, end_x, end_y, gameColorToRaylib(color));
}

void platformDrawPoly(int center_x, int center_y, int sides, int radius, float rotation, GameColor color) {
    DrawPolyLines((Vector2){center_x, center_y}, sides, radius, rotation, gameColorToRaylib(color));
}

int main() {
    struct timespec game_lib_last_mod;
    if (!loadGameLib()) {
        fprintf(stderr, "Failed to load game dynamic library\n");
        return 0;
    }

    game_lib_last_mod = getFileLastMod(GAME_SO_PATH);

    int window_width  = 800;
    int window_height = 600;
    InitWindow(window_width, window_height, "Spider Game");
    int current_monitor = GetCurrentMonitor();
    int refresh_rate    = GetMonitorRefreshRate(current_monitor);

    SetTargetFPS(refresh_rate);

    PlatformFunctions pFunctions = {
        platformClearBackground,
        platformDrawCircle,
        platformDrawText,
        platformDrawLine,
        platformDrawPoly,
    };

    GameInput game_input;
    GameMemory game_memory   = {0};
    game_memory.world_width  = window_width;
    game_memory.world_height = window_height;
    while(!WindowShouldClose()) {
        reloadGameLibIfChanged(&game_lib_last_mod);

        game_input = (GameInput){0};

        if (IsWindowResized()) {
            window_width = GetScreenWidth();
            window_height = GetScreenHeight();
            game_memory.world_width = window_width;
            game_memory.world_height = window_height;
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            game_input.mouse_buttons[GAME_MOUSE_BUTTON_LEFT].state |= GAME_BUTTON_IS_DOWN;
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            game_input.mouse_buttons[GAME_MOUSE_BUTTON_LEFT].state |= GAME_BUTTON_PRESSED;
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            game_input.mouse_buttons[GAME_MOUSE_BUTTON_LEFT].state |= GAME_BUTTON_RELEASED;
        }

        if (IsKeyReleased(KEY_R)) {
            //TODO: This was jsut done as a way to quickly have R as reset key (sets keyboard_button[0] as R)
            // this needs to change so each letter index corresponds to their position in the alphabet
            game_input.keyboard_buttons[0].state |= GAME_BUTTON_PRESSED;
        }

        Vector2 mouse_pos_v2 = GetMousePosition();
        game_input.mouse_position = (Position) {mouse_pos_v2.x, mouse_pos_v2.y};

        BeginDrawing();
        {
            game_memory.delta_time = GetFrameTime();

            if (UPDATE_AND_RENDER_GAME_FUNC) {
                UPDATE_AND_RENDER_GAME_FUNC(&game_memory, &game_input, pFunctions);
            }
            //DrawLineBezier((Vector2){0, 0}, (Vector2){100, 100}, 3, BLACK);                   // Draw line segment cubic-bezier in-out interpolation
        } EndDrawing();
    }

    CloseWindow();
    return 0;
}
