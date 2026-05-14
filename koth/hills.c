#include <libuya/stdio.h>
#include <libuya/spawnpoint.h>
#include <libuya/game.h>
#include <libuya/map.h>
#include "include/koth.h"

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

extern Cuboid bakisiIsles[];
extern Cuboid hovenGorge[];
extern Cuboid outpostX12[];
extern Cuboid korgonOutpost[];
extern Cuboid metropolis[];
extern Cuboid blackwaterCity[];
extern Cuboid commandCenter[];
extern Cuboid blackwaterDocks[];
extern Cuboid aquatosSewers[];
extern Cuboid marcadia[];

// array based on MAP_ID - 40, if hill moby (0x3000) doesn't exist.
Cuboid* vanillaMapCuboids[10] = {
    bakisiIsles,
    hovenGorge,
    outpostX12,
    korgonOutpost,
    metropolis,
    blackwaterCity,
    commandCenter,
    blackwaterDocks,
    aquatosSewers,
    marcadia
};

Cuboid bakisiIsles[] = {
    {
        .matrix = {
            {20, 0, 0, 0},
            {0, 20, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {373.6029, 414.3022, 200.7417, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {20, 0, 0, 0},
            {0, 30, 0, 0},
            {0, 0, 1, 0}
        },
        .pos = {519.58356, 398.7586, 201.38, 1},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {35, 0, 0, 0},
            {0, 35, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {409.012, 567.944, 201.250, 1},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {18, 0, 0, 0},
            {0, 18, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {281.9309, 429.3058, 201.3012, 1},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {30, 0, 0, 0},
            {0, 30, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {325.4687, 298.8129, 201.1440, 1},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {20, 0, 0, 0},
            {0, 20, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {567.0633, 259.2868, 202.3434, 1},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
};

Cuboid hovenGorge[] = {
    {
        .matrix = {
            {30, 0, 0, 0},
            {0, 30, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {352.0844, 334.7871, 67.0, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {25, 0, 0, 0},
            {0, 25, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {339.51, 240.437, 66.4, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {30, 0, 0, 0},
            {0, 30, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {260.38, 251.191, 63.68, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {25, 0, 0, 0},
            {0, 25, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {172.397, 251.956, 67.25, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {20, 0, 0, 0},
            {0, 20, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {166.521, 164.01, 66.20, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
};

Cuboid outpostX12[] = {
    {
        .matrix = {
            {25, 0, 0, 0},
            {0, 25, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {327.71, 257.880, 101.35, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {35, 0, 0, 0},
            {0, 35, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {478.67, 191.556, 102.10, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {20, 0, 0, 0},
            {0, 20, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {509.34, 248.783, 101.1, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {30, 0, 0, 0},
            {0, 30, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {389.9045, 236.870, 102.10, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {25, 0, 0, 0},
            {0, 25, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {373.82, 285.54, 101.1, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
};

Cuboid korgonOutpost[] = {
    {
        .matrix = {
            {33, 0, 0, 0},
            {0, 33, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {359.3894, 427.6866, 97.0, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {25, 0, 0, 0},
            {0, 25, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {322.8834, 337.7596, 97.1, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {20, 0, 0, 0},
            {0, 20, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {395.0782, 298.7676, 97.0844, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {27, 0, 0, 0},
            {0, 27, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {424.8894, 335.7746, 97.1, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {30, 0, 0, 0},
            {0, 30, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {215.4736, 338.2934, 97.1, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {15, 0, 0, 0},
            {0, 15, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {283.20, 291.9809, 97.3, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
};

Cuboid metropolis[] = {
    {
        .matrix = {
            {20, 0, 0, 0},
            {0, 11, 0, 0},
            {0, 0, 1, 0}
        },
        .pos = {755.4391, 339.3310, 338.8, 0},
        .imatrix = 0,
        .rot = {0, 0, 0.8125, 0}
    },
    {
        .matrix = {
            {66.915, 0, 0, 0},
            {0, 66.915, 0, 0},
            {0, 0, 1, 0}
        },
        .pos = {797.5479, 296.7849, 336.597, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {66.915, 0, 0, 0},
            {0, 66.915, 0, 0},
            {0, 0, 1, 0}
        },
        .pos = {712.8783, 381.7458, 336.597, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {18, 0, 0, 0},
            {0, 18, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {773.9068, 445.8956, 336.4594, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {18, 0, 0, 0},
            {0, 18, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {737.2782, 231.7361, 336.4694, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {20, 0, 0, 0},
            {0, 20, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {707.1735, 290.8173, 329.2778, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {20, 0, 0, 0},
            {0, 20, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {803.4766, 387.5046, 329.2790, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
};

Cuboid blackwaterCity[] = {
    {
        .matrix = {
            {25, 0, 0, 0},
            {0, 25, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {226.9902, 366.6711, 81.6625, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {25, 0, 0, 0},
            {0, 25, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {209.0136, 171.8414, 81.6825, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {30, 0, 0, 0},
            {0, 30, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {219.9686, 235.8673, 87.4906, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {30, 0, 0, 0},
            {0, 30, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {219.6191, 297.8083, 87.4906, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
};

Cuboid commandCenter[] = {
    {
        .matrix = {
            {40, 0, 0, 0},
            {0, 40, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {330.1714, 365.327, 35.0198, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
};

Cuboid blackwaterDocks[] = {
    {
        .matrix = {
            {20, 0, 0, 0},
            {0, 20, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {211.7765, 191.4453, 100.4812, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {15, 0, 0, 0},
            {0, 15, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {261.8332, 191.4044, 100.7406, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {15, 0, 0, 0},
            {0, 15, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {261.8378, 263.7712, 98.3135, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {30, 0, 0, 0},
            {0, 30, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {212, 263.6, 106.3031, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
};

Cuboid aquatosSewers[] = {
    {
        .matrix = {
            {16, 0, 0, 0},
            {0, 16, 0, 0},
            {0, 0, 1, 0}
        },
        .pos = {400.9128, 336.8955, 941.5, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
};

Cuboid marcadia[] = {
    {
        .matrix = {
            {10, 0, 0, 0},
            {0, 10, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {538.2250, 843.2510, 116.1824, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {20, 0, 0, 0},
            {0, 20, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {502.108, 864.6436, 117, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {15, 0, 0, 0},
            {0, 15, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {478.471, 869.456, 117.1281, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {15, 0, 0, 0},
            {0, 15, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {450.248, 889.508, 117, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {15, 0, 0, 0},
            {0, 15, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {433.1663, 862.41, 116.9276, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    }
};

const int MAP_CUBOID_COUNTS[10] = {
    ARRAY_SIZE(bakisiIsles),
    ARRAY_SIZE(hovenGorge),
    ARRAY_SIZE(outpostX12),
    ARRAY_SIZE(korgonOutpost),
    ARRAY_SIZE(metropolis),
    ARRAY_SIZE(blackwaterCity),
    ARRAY_SIZE(commandCenter),
    ARRAY_SIZE(blackwaterDocks),
    ARRAY_SIZE(aquatosSewers),
    ARRAY_SIZE(marcadia)
};

#include <malloc.h> // or stdlib depending on your PS2 SDK

Cuboid *createCuboid(VECTOR pos, int num)
{
    Cuboid *c = (Cuboid*)malloc(sizeof(Cuboid));
    if (!c)
        return NULL;

    // Initialize
    vector_copy(c->matrix.v0, (VECTOR){HILL_VANILLA_SCALE, 0, 0, 0});
    vector_copy(c->matrix.v1, (VECTOR){0, HILL_VANILLA_SCALE, 0, 0});
    vector_copy(c->matrix.v2, (VECTOR){0, 0, 2, 0});
    memcpy(c->pos, pos, sizeof(VECTOR));
    return c;
}

void getHillCuboids(hillPvar_t *this, bool isCustomMap, bool foundCustomMoby)
{
    if (isCustomMap) {
        if (foundCustomMoby) {
            memcpy(&this->hillCuboidIndex, kothInfo.hillMoby->pVar, 0x80);
            // get cuboid pointers from custom cuboid index.
            int i = 0;
            for (i; i < 32; ++i) {
                if (this->hillCuboidIndex[i] > 0)
                    this->hillCuboidPtrs[i] = spawnPointGet(this->hillCuboidIndex[i]);
            }
        } else {
            // get siege nodes for hills
            Moby* moby = mobyListGetStart();
            Moby* mobyEnd = mobyListGetEnd();
            int i = 0;
            while (moby < mobyEnd) {
                if (moby->oClass == MOBY_ID_SIEGE_NODE) {
                    // grab associated bolt crank from bridge peice
                    int boltCrankInstanceNum = *(int*)(moby->pVar + 0x3c);
                    Moby* list = mobyListGetStart();
                    Moby* boltCrank = list + boltCrankInstanceNum;
                    if (boltCrank->oClass == MOBY_ID_BOLT_CRANK) {
                        this->hillCuboidPtrs[i] = createCuboid(boltCrank->position, i);
                    }
                    ++i;
                }
                ++moby;
            }
            kothInfo.lastGameUsedMalloc = true;
        }
    } else {
        // get custom vanilla hills
        int mapId = GAME_MAP_ID - 40;
        int numCuboids = MAP_CUBOID_COUNTS[mapId];
        Cuboid* cuboids = vanillaMapCuboids[mapId];
        // Fill in the valid ones
        int i;
        for (i = 0; i < numCuboids; i++) {
            this->hillCuboidPtrs[i] = &cuboids[i];
        }
    }
}
