#include "chunk.h"
#include "../state.h"
#include "../gfx/renderer.h"
#include "block.h"

chunk_t chunk_gen(int x, int z) {
    chunk_t chunk = {0};
    chunk.x = x;
    chunk.z = z;
    for (int x = 0; x < CHUNK_X; x++) {
        for (int y = 0; y < CHUNK_Y; y ++) {
            for (int z = 0; z < CHUNK_Z; z++) {
                chunk.blocks[x][y][z] = (block_t){.type = BLOCK_TYPE_VISIBLE, .texture = RENDERER_TEXTURE_STUD};
            }
        }
    }
    return chunk;
}

static bool has_neighbours(chunk_t *chunk, int x, int y, int z) {
    if (x == 0 ||
        y == 0 ||
        z == 0 ||
        x == CHUNK_X - 1 ||
        y == CHUNK_Y - 1 ||
        z == CHUNK_Z - 1
    ) return false;

    return (chunk->blocks[x + 1][y][z].type == BLOCK_TYPE_VISIBLE)
        && (chunk->blocks[x - 1][y][z].type == BLOCK_TYPE_VISIBLE)
        && (chunk->blocks[x][y + 1][z].type == BLOCK_TYPE_VISIBLE)
        && (chunk->blocks[x][y - 1][z].type == BLOCK_TYPE_VISIBLE)
        && (chunk->blocks[x][y][z + 1].type == BLOCK_TYPE_VISIBLE)
        && (chunk->blocks[x][y][z - 1].type == BLOCK_TYPE_VISIBLE);
}

void chunk_draw(chunk_t *chunk) {
    for (int x = 0; x < CHUNK_X; x++) {
        for (int y = 0; y < CHUNK_Y; y++) {
            for (int z = 0; z < CHUNK_Z; z++) {
                if (chunk->blocks[x][y][z].type == BLOCK_TYPE_INVISIBLE ||
                    has_neighbours(chunk, x, y, z)) {
                    continue;
                }

                renderer_box(&state.renderer, (vec3){
                    chunk->x + x,
                    y,
                    chunk->z + z
                }, chunk->blocks[x][y][z].texture);
            }
        }
    }
}