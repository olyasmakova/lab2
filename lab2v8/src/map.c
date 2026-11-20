#define _CRT_SECURE_NO_WARNINGS
#include "map.h"
#include "player.h"
#include <stdio.h>
#include <stdlib.h>

// это типа старый createMap
static char** allocGrid(int width, int height) {
    char** grid = (char**)malloc(height * sizeof(char*));
    if (grid == NULL) return NULL;

    for (int i = 0; i < height; i++) {
        grid[i] = (char*)malloc(width * sizeof(char));
        if (grid[i] == NULL) {
            for (int j = 0; j < i; j++) free(grid[j]);
            free(grid);
            return NULL;
        }
    }
    return grid;
}

// динамическое выделение самой структуры карты
struct Map* createMapObject(void) {
    struct Map* m = (struct Map*)malloc(sizeof(struct Map));
    if (m == NULL) return NULL;

    m->grid = NULL;
    m->width = 0;
    m->height = 0;
    m->heroX = 0;
    m->heroY = 0;
    m->underHero = CHAR_EMPTY;
    m->mode = MODE_FOREST;
    m->mapsTravelled = 0;
    m->active = 0;
    return m;
}

// освобождение карты (сначала строки, потом массив указателей, потом структура)
void freeMap(struct Map** mapPtr) {
    if (*mapPtr == NULL) return;

    struct Map* map = *mapPtr;
    if (map->grid != NULL) {
        for (int i = 0; i < map->height; i++) {
            free(map->grid[i]);
        }
        free(map->grid);
    }
    printMemoryFree(map);
    free(map);
    *mapPtr = NULL;
}

void printMap(const struct Map* map) {
    for (int y = 0; y < map->height; y++) {
        for (int x = 0; x < map->width; x++) {
            putchar(map->grid[y][x]);
        }
        putchar('\n');
    }
}

void printHUD(const struct Player* player, const struct Map* map, int day, int hour) {
    printf("\nHP: %d/%d | Выносливость: %d/%d | Золото: %d | Карта: %d | Время: день %d, %02d:00\n",
        player->hp, player->max_hp,
        player->stamina, player->max_stamina,
        player->gold,
        map->mapsTravelled,
        day, hour);

    if (player->active_relic.effect == RELIC_NONE)
        printf("Реликвия: нет\n");
    else
        printf("Реликвия: %s\n", player->active_relic.name);
}

// generateForest теперь пишет в структуру
void generateForest(struct Map* map) {
    int w = MAP_MIN_SIZE + rand() % (MAP_MAX_SIZE - MAP_MIN_SIZE + 1);
    int h = MAP_MIN_SIZE + rand() % (MAP_MAX_SIZE - MAP_MIN_SIZE + 1);

    map->grid = allocGrid(w, h);
    if (map->grid == NULL) return;
    map->width = w;
    map->height = h;

    // заполняем препятствиями
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            if (rand() % 100 < OBSTACLE_CHANCE) {
                map->grid[y][x] = (rand() % 2 == 0) ? CHAR_TREE : CHAR_ROCK;
            }
            else {
                map->grid[y][x] = (rand() % 2 == 0) ? CHAR_EMPTY : CHAR_GRASS;
            }
        }
    }

    // замыкаем карту стенами по краям
    for (int x = 0; x < w; x++) {
        map->grid[0][x] = CHAR_WALL;
        map->grid[h - 1][x] = CHAR_WALL;
    }
    for (int y = 0; y < h; y++) {
        map->grid[y][0] = CHAR_WALL;
        map->grid[y][w - 1] = CHAR_WALL;
    }

    // ставим вход в пещеру O
    int caveX, caveY;
    do {
        caveX = rand() % w;
        caveY = rand() % h;
    } while (map->grid[caveY][caveX] != CHAR_EMPTY && map->grid[caveY][caveX] != CHAR_GRASS);
    map->grid[caveY][caveX] = CHAR_CAVE;

    // ставим костёр
    int fireX, fireY;
    do {
        fireX = rand() % w;
        fireY = rand() % h;
    } while (map->grid[fireY][fireX] != CHAR_EMPTY && map->grid[fireY][fireX] != CHAR_GRASS);
    map->grid[fireY][fireX] = CHAR_CAMPFIRE;

    // ставим героя
    do {
        map->heroX = rand() % w;
        map->heroY = rand() % h;
    } while (map->grid[map->heroY][map->heroX] != CHAR_EMPTY && map->grid[map->heroY][map->heroX] != CHAR_GRASS);
    map->underHero = map->grid[map->heroY][map->heroX];
    map->grid[map->heroY][map->heroX] = CHAR_HERO;
}

//generateCave теперь пишет в структуру
void generateCave(struct Map* map) {
    int w = CAVE_MIN_SIZE + rand() % (CAVE_MAX_SIZE - CAVE_MIN_SIZE + 1);
    int h = CAVE_MIN_SIZE + rand() % (CAVE_MAX_SIZE - CAVE_MIN_SIZE + 1);

    map->grid = allocGrid(w, h);
    if (map->grid == NULL) return;
    map->width = w;
    map->height = h;

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            if (rand() % 100 < CAVE_WALL_CHANCE) {
                map->grid[y][x] = CHAR_WALL;
            }
            else {
                map->grid[y][x] = (rand() % 2 == 0) ? CHAR_EMPTY : CHAR_GRASS;
            }
        }
    }

    for (int x = 0; x < w; x++) {
        map->grid[0][x] = CHAR_WALL;
        map->grid[h - 1][x] = CHAR_WALL;
    }
    for (int y = 0; y < h; y++) {
        map->grid[y][0] = CHAR_WALL;
        map->grid[y][w - 1] = CHAR_WALL;
    }

    // ставим выход O (вернёт в лес)
    int exitX, exitY;
    do {
        exitX = rand() % w;
        exitY = rand() % h;
    } while (map->grid[exitY][exitX] != CHAR_EMPTY && map->grid[exitY][exitX] != CHAR_GRASS);
    map->grid[exitY][exitX] = CHAR_CAVE;

    // ставим героя
    do {
        map->heroX = rand() % w;
        map->heroY = rand() % h;
    } while (map->grid[map->heroY][map->heroX] != CHAR_EMPTY && map->grid[map->heroY][map->heroX] != CHAR_GRASS);
    map->underHero = map->grid[map->heroY][map->heroX];
    map->grid[map->heroY][map->heroX] = CHAR_HERO;
}

// переход через вход в пещеру O
// смена карты тратит стамину, время и приносит золото
int moveToNextMap(struct Map* map, struct Player* player, int* day, int* hour) {
    // освобождаем старую 
    for (int i = 0; i < map->height; i++) free(map->grid[i]);
    free(map->grid);
    map->grid = NULL;

    if (map->mode == MODE_FOREST) {
        map->mode = MODE_CAVE;
        printf("\nВы вошли в пещеру!\n");
        generateCave(map);
    }
    else {
        map->mode = MODE_FOREST;
        printf("\nВы вышли из пещеры в лес!\n");
        generateForest(map);
    }

    if (map->grid == NULL) {
        printf("!Ошибка! Не удалось создать новую карту.\n");
        map->active = 0;
        return 0;
    }

    map->mapsTravelled++;
    printf("=== Пройдено карт: %d ===\n", map->mapsTravelled);

    // переход на новую карту.стамина зависит от сапог-скороходов
    int mapChangeCost = FLOOR_STAMINA_COST;
    if (player->active_relic.effect == RELIC_QUICK) {
        mapChangeCost = RELIC_QUICK_COST;   // сапоги 1 вместо 5
        printf("Сапоги-скороходы облегчили переход! (-%d выносливости)\n", mapChangeCost);
    }
    player->stamina -= mapChangeCost;

    int reward = BASE_REWARD * map->mapsTravelled;
    if (player->active_relic.effect == RELIC_GREED) {
        reward *= 2;                        // кольцо жадности x2
        printf("Кольцо жадности удваивает награду!\n");
    }
    player->gold += reward;
    printf("Награда за новую карту: +%d золота!\n", reward);

    //шанс 20% на реликвию
    if (rand() % 100 < RELIC_DROP_CHANCE) {
        Relic found = generateRandomRelic();
        printf("\n!!! НАЙДЕНА РЕЛИКВИЯ: %s !!!\n", found.name);
        printf("[1] Надеть (старый талисман пропадёт)\n");
        printf("[2] Продать за 25 золотых\n");
        int choice = getIntSafe("Ваш выбор: ");
        if (choice == 1) {
            player->active_relic = found;
            printf("Вы надели: %s\n", found.name);
        }
        else {
            player->gold += RELIC_SELL_PRICE;
            printf("Вы продали реликвию за %d золота.\n", RELIC_SELL_PRICE);
        }
    }

    (*hour) += 1;
    if (*hour >= DAY_HOURS) {
        *hour -= DAY_HOURS;
        (*day)++;
    }
    return 1;
}

// moveHero  
// глобальные переменные заменены полями структуры
int moveHero(struct Map* map, int dx, int dy) {
    int newX = map->heroX + dx;
    int newY = map->heroY + dy;

    if (newX < 0 || newX >= map->width || newY < 0 || newY >= map->height) {
        printf("За край нельзя — карта окружена стенами!\n");
        return 0;
    }

    char target = map->grid[newY][newX];

    if (target == CHAR_TREE || target == CHAR_ROCK || target == CHAR_WALL) {
        printf("Там препятствие!\n");
        return 0;
    }

    map->grid[map->heroY][map->heroX] = map->underHero;

    map->heroX = newX;
    map->heroY = newY;
    map->underHero = target;

    if (target == CHAR_CAVE) {
        return 1; // переход обработается в startAdventure через moveToNextMap
    }

    map->grid[newY][newX] = CHAR_HERO;
    return 1;
}

void startAdventure(struct Map** mapPtr, struct Player* player, int* day, int* hour) {
    struct Map* map = createMapObject();
    if (map == NULL) {
        printf("!Ошибка! Не удалось выделить память под карту.\n\n");
        return;
    }

    map->mode = MODE_FOREST;
    map->active = 1;
    map->mapsTravelled = 1;
    generateForest(map);

    if (map->grid == NULL) {
        printf("!Ошибка! Не удалось выделить память под карту.\n\n");
        freeMap(&map);
        return;
    }
    *mapPtr = map;

    printf("\n=== ВЫ В ЛЕСУ ===\n");
    printf("Легенда: . , = пусто, T = дерево, R = камень, # = стена,\n");
    printf("         @ = ты, ^ = костёр (Q — домой), O = вход в пещеру\n\n");

    while (map->active) {
        printHUD(player, map, *day, *hour);
        printMap(map);

        printf("\n[WASD] — ходить, [Q] — выйти домой (только у костра)\n");
        printf("> ");

        int c = getchar();
        if (c == '\n') continue;
        if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))) continue;
        if (c >= 'A' && c <= 'Z') c = c + ('a' - 'A');

        int moved = 0;

        switch (c) {
        case 'w': moved = moveHero(map, 0, -1); break;
        case 's': moved = moveHero(map, 0, 1);  break;
        case 'a': moved = moveHero(map, -1, 0); break;
        case 'd': moved = moveHero(map, 1, 0);  break;
        case 'q':
            if (map->underHero == CHAR_CAMPFIRE) {
                map->active = 0;
                printf("\nВы вернулись домой у костра! Пройдено карт: %d\n\n", map->mapsTravelled);
            }
            else {
                printf("Нельзя выйти отсюда! Найдите костёр (^).\n");
            }
            break;
        default:
            printf("Неизвестная команда. Используйте WASD или Q.\n");
            break;
        }

        // наступили на O переход на новую ккрту
        if (moved && map->grid[map->heroY][map->heroX] == CHAR_HERO) {
            // обычный шаг
        }
        if (moved && map->underHero == CHAR_CAVE) {
            // moveHero вернул 1, переход делаем здесь
            if (!moveToNextMap(map, player, day, hour)) break;
        }

        // каждый успешный шаг тратит выносливость (кольцо жадности шаг становиться дороже)
        if (moved) {
            int stepCost = STEP_STAMINA_COST;
            if (player->active_relic.effect == RELIC_GREED) {
                stepCost = RELIC_GREED_STEP_COST;
            }
            player->stamina -= stepCost;

            // камень тролля. +1 HP за каждый успешный шаг (если есть куда лечить)
            if (player->active_relic.effect == RELIC_HEAL && player->hp < player->max_hp) {
                player->hp += 1;
                printf("Камень тролля восстанавливает 1 HP!\n");
            }
        }

        // экстренная эвакуация(стамина 0 или время дошло до 02:00)
        if (player->stamina <= 0 || *hour == NIGHT_HOUR) {
            int lost = player->gold / 2;
            player->gold -= lost;
            printf("\nВы падаете без сил! Спасательная служба доставила героя домой\n");
            printf("и забрала половину золота за услуги (-%d).\n", lost);
            map->active = 0;
        }
    }

    //завершение
    freeMap(mapPtr);
}
