PLATFORM_COMPILE_FILES="platform_main_raylib_linux.c"
PLATFORM_LINKER_FLAGS="-lraylib -lGL -lm -lpthread -ldl -lrt -lX11"

GAME_COMPILE_FILES="game.c"

SHARED_COMP_FLAGS="-DDEBUG -ggdb -Wall -Wextra -Werror -std=gnu99 -Wvla -Wno-unused-parameter"

#TODO: add a project path as -D var so I can know where to get the game.so
mkdir -p build
gcc $SHARED_COMP_FLAGS -shared -fPIC $GAME_COMPILE_FILES -o build/game.so

gcc $SHARED_COMP_FLAGS $PLATFORM_COMPILE_FILES -o build/main $PLATFORM_LINKER_FLAGS
