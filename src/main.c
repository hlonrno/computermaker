#include <stdlib.h>
#include <string.h>
    
#include "cglm/util.h"
#include "config.h"
#include "state.h"
#include "gfx/camera.h"
#include "gfx/renderer.h"
#include "world/chunk.h"
#include "world/world.h"

void init(void) {
    config_open("res/config.comm");
    config_process();
    renderer_init(&state.renderer);
    state.renderer.wireframe = false;
    world_worldgen(&state.world);
    skybox_init(&state.world.skybox);
}

void destroy(void) {
    exit(0);
}

void tick(void) {
    if (window.mouse.moved) {
        camera_mouse_cb(&state.renderer.camera, window.mouse.x, window.mouse.y);
        window.mouse.moved = true;
    }

    if (window.keyboard.keys[GLFW_KEY_W].down) {
        camera_move(&state.renderer.camera, CAMERA_DIRECTION_FORWARD);
    }
    if (window.keyboard.keys[GLFW_KEY_A].down) {
        camera_move(&state.renderer.camera, CAMERA_DIRECTION_LEFT);
    }
    if (window.keyboard.keys[GLFW_KEY_S].down) {
        camera_move(&state.renderer.camera, CAMERA_DIRECTION_BACK);
    }
    if (window.keyboard.keys[GLFW_KEY_D].down) {
        camera_move(&state.renderer.camera, CAMERA_DIRECTION_RIGHT);
    }

    if (window.mouse.scrolled) {
        state.renderer.camera.perspective.fovy += glm_rad(window.mouse.scroll.y);
        window.mouse.scrolled = false;
    }

    if (window.mouse.buttons[GLFW_MOUSE_BUTTON_LEFT].down ||
        window.mouse.buttons[GLFW_MOUSE_BUTTON_RIGHT].down) {
        struct world_get_at_info info = world_get_at(
            &state.world, 
            state.renderer.camera.target[0],
            state.renderer.camera.target[1],
            state.renderer.camera.target[2]
        );
        if (window.mouse.buttons[GLFW_MOUSE_BUTTON_LEFT].down)
            info.chunk->blocks[info.x][info.y][info.z] = (block_t){.id = STUD};
        else info.chunk->blocks[info.x][info.y][info.z] = (block_t){.id = AIR};

        chunk_bake(info.chunk);

        if (window.mouse.buttons[GLFW_MOUSE_BUTTON_LEFT].down)
            window.mouse.buttons[GLFW_MOUSE_BUTTON_LEFT].down = false;
        else window.mouse.buttons[GLFW_MOUSE_BUTTON_RIGHT].down = false;
    }
    if (window.keyboard.keys[GLFW_KEY_O].down) {
        state.renderer.wireframe = !state.renderer.wireframe;
        window.keyboard.keys[GLFW_KEY_O].down = false;
    }
    if (window.keyboard.keys[GLFW_KEY_R].down) {
        memset(&state.renderer.camera.origin, 0, sizeof(state.renderer.camera.origin));
    }
}

void render(void) {
    skybox_draw(&state.world.skybox, &state.renderer);
    world_draw(&state.world);
}

int main() {    
    window_init();
    window_create("comm", 800, 600, init, destroy, tick, render);
    window_loop();
}
