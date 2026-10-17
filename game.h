#include <stdbool.h>
#include <stdint.h>

#define s8  int8_t
#define u8  uint8_t
#define s32 int
#define u32 uint32_t
#define s64 int64_t
#define u64 uint64_t

typedef struct {
    float x;
    float y;
} Position;

// GameColor, 4 components, R8G8B8A8 (32bit)
typedef struct GameColor {
    u8 r;        // GameColor red value
    u8 g;        // GameColor green value
    u8 b;        // GameColor blue value
    u8 a;        // GameColor alpha value
} GameColor;

typedef struct {
    float delta_time;
    s32   world_width;
    s32   world_height;
} GameMemory;

//BITMASK OF GAME STATE
typedef enum {
    GAME_BUTTON_UNKNOWN  = 0,
    GAME_BUTTON_PRESSED  = 1, //Button was pressed on last frame
    GAME_BUTTON_RELEASED = 2, //Button was released on last frame
    GAME_BUTTON_IS_DOWN  = 4,
} GameButtonState;

//MOUSE BUTTONS INDEXES
typedef enum {
    GAME_MOUSE_BUTTON_LEFT,
    GAME_MOUSE_BUTTON_RIGHT, //Button was pressed on last frame
    GAME_MOUSE_BUTTON_FOWARD, //Button was released on last frame
    GAME_MOUSE_BUTTON_BACKWARD,
} GameMouseButtons;

typedef struct {
    GameButtonState state;
} GameButton;

typedef struct {
    GameButton  mouse_buttons[4];
    Position    mouse_position;
    GameButton  keyboard_buttons[27];
} GameInput;

typedef void (*platformClearBackground_t) (GameColor color);
typedef void (*platformDrawCircle_t)      (s32 center_x, s32 center_y, float radius, GameColor color);
typedef void (*platformDrawText_t)        (char *text, s32 pos_x, s32 pos_y, s32 font_size, GameColor color);
typedef void (*platformDrawLine_t)        (s32 start_x, s32 start_y, s32 end_x, s32 end_y, GameColor color);
typedef void (*platformDrawPoly_t)        (s32 center_x, s32 center_y, s32 sides, s32 radius, float rotation, GameColor color);

typedef struct {
    platformClearBackground_t pClearBackground;
    platformDrawCircle_t      pDrawCircle;
    platformDrawText_t        pDrawText;
    platformDrawLine_t        pDrawLine;
    platformDrawPoly_t        pDrawPoly;
} PlatformFunctions;

typedef void (*updateAndRenderGame_t)(GameMemory *game_memory, GameInput *game_input, PlatformFunctions pFunctions);

#define GAME_BLACK  (GameColor){   0,   0,   0,  255 }
#define GAME_BLUE   (GameColor){   0, 121, 241,  255 }
#define GAME_GREEN  (GameColor){   0, 228,  48,  255 }
#define GAME_RED    (GameColor){ 230,  41,  55,  255 }
#define GAME_ORANGE (GameColor){ 255, 161,   0,  255 }
#define GAME_WHITE  (GameColor){ 255, 255, 255,  255 }
