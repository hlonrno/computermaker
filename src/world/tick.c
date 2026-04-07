#pragma once

#include "chunk.h"
#include "world.h"

void world_tick(struct world *world) {
    for (int i = 0; i < world->chunks_size; i++) {
        for (int x = 0; x < CHUNK_X; x++) {
            for (int y = 0; y < CHUNK_Y; y++) {
                for (int z = 0; z < CHUNK_Z; z++) {
                    if (is_logic_block(world->chunks[i].blocks[x][y][z])) {
                        block_tick(&world->chunks[i].blocks[x][y][z]);
                    }
                }
            }
        }
    }
}
