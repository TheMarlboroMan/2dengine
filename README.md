# d2d

## what do I want to do with this

- Streamline the experience of creating a 2dgame by providing a set of commonly used tools.

## what I don't want is

- To replace the DFW jumpstart. That should be still usable and this library should be pluggable into it.

## opinions of 2d2

- motion vectors use doubles.
- spatiable elements use doubles for position and integers for size.

I have tested this with floats by changing the definitions and it works, but
I don't want to make template classes to speed up compilation times. If we
need floats, change, recompile and repackage.

## namespaces of d2d

### audio

    music_player
    sound_player
    sound_player_then

### collision

    aabb collision suite (faster, simpler, requires SPATIABLES)
        aabb_static_checker (allows checking of static positions against boxes 
            or spatiables, does not need axis separation, does NOT preserve 
            type when returning collisions).
        aabb_checker (checks a spatiable against vectors of N, returning those
            which collide and preserving their type)
        aabb_solver (feeds from the result of aabb_checker)
        aabb_phase (reusable collision solution, separates Y and X axis)
    definitions
    ray vs aabb collision suite:
        ray_aabb_checker (can preserve the type of lists being checked!)
        ray_aabb_finder (produces vectors of collisions).
        ray_aabb_solver (solves the collisions againts a single object)
        ray_aabb_phase (reusable one-shot collision solution) TODO: not really, more like a broad sweep xD
    ray_builder
    shaper (interface)
        shaper_default
    spatiable (interface)
        motion_axis_limiter
        tile
    tile_filter (free floating functions)
    tile_finder (interface)
        tile_finder_matrix
    tile_implementation (interface) <== SUCH A TERRIBLE NAME!
    tile_limits
    tiles_in_box
    tools (free floating functions)

### components

    particle (particle managers and particles, mostly particle pools, the rest must be implemented!).
    persistence
    timeouts

### motion

    accelerator
    definitions
    gravity
    mover

### storage

    attribute
    definitions 
    map_loader
    property_processor (interface)
    thing_processor (interface)

### tools

    algorithm
    to_ref

### video

    animation_manager
    animation_sprite_finder
    camera_map_limit
    scenery_tile
    drawing suite:
        debug_display
        scenery_tile_draw
        sprite_draw
        sprite_fill_draw
    spritesheet_manager
    tools

## howto

### types of tiles

    collision
    decoration

### how to load tiles

    the map format

### particles

    TODO:

## building this on windows.

- use msys2
- pacman -S --needed base-devel mingw-w64-x86_64-toolchain mingw-w64-x86_64-cmake mingw64/mingw-w64-x86_64-SDL2 mingw64/mingw-w64-x86_64-SDL2_image mingw64/mingw-w64-x86_64-SDL2_mixer mingw64/mingw-w64-x86_64-SDL2_ttf mingw-w64-x86_64-rapidjson git mingw-w64-x86_64-mesa msys2-runtime-devel mingw64/mingw-w64-x86_64-freeglut mingw64/mingw-w64-x86_64-glew mingw64/mingw-w64-x86_64-gdb vim
- for cmake, use cmake .. -G "MSYS Makefiles"
    - actually I didn't try anything else.
- order of dependencies:
    - log
    - tools
    - appenv
    - console
    - libdansdl2
    - ldtools
    - appconsole
    - dfw
- To devise dependencies of an executable ldd  executable
    - then grab them from C:/mysy64/mingw64/bin and put them besides the executable.
    - i did ldd executable | awk '{print $1}' > libs.txt and then looped that txt file.
- To debug it, you can use gdb, I found it works if you break main.cpp:linenum and take it from there.

## resources

- GGBotNet for the "public pixel" ttf font
- Surt, Sharm and Vk for the tileset at https://opengameart.org/content/simple-broad-purpose-tileset
- Brandon75689 for "cave theme" in test music at https://opengameart.org/content/cave-theme
- Rezoner for "happy arcade tune" in test music at https://opengameart.org/content/happy-arcade-tune
- pauliuw for "field of dreams" in test music at https://opengameart.org/content/the-field-of-dreams- 


