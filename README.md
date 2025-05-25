# d2d

## what do I want to do with this

- Streamline the experience of creating a 2dgame by providing a set of commonly used tools.

## namespaces of d2d

### audio

    music_player
    sound_player
    sound_player_then

### collision

    aabb collision suite (faster, simpler, almost requires separation of axis).
        aabb_checker (produces booleans and vectors of pointers to spatiables)
        aabb_solver (feeds from the result of aabb_checker)
        aabb_phase
    definitions
    exception
    ray vs aabb collision suite:
        ray_aabb_checker (produces booleans and vectors of pointers to spatiables)
        ray_aabb_finder (produces vectors of collisions).
        ray_aabb_solver (solves the collisions againts a single object)
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

    exception
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
    exception
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
    debug_display
    exception
    scenery_tile
    scenery_tile_draw
    sprite_draw
    spritesheet_manager
    tools

## howto

### types of tiles

    collision
    decoration

### how to load tiles

    the map format

## resources

- Font Babas Neue by Dharma Type.
- Surt, Sharm and Vk for the tileset at https://opengameart.org/content/simple-broad-purpose-tileset
- Brandon75689 for "cave theme" in test music at https://opengameart.org/content/cave-theme
- Rezoner for "happy arcade tune" in test music at https://opengameart.org/content/happy-arcade-tune
- pauliuw for "field of dreams" in test music at https://opengameart.org/content/the-field-of-dreams- 

- Sounds: Little Robot Sound Factory, www.littlerobotsoundfactory.com

