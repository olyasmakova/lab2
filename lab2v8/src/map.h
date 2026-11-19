#ifndef MAP_H
#define MAP_H

#include "utils.h"

struct Player; // предварительное объявлениечтобы не было цикла включений

struct Map {
    char** grid;        // двумерный динамический массив карты
    int width;
    int height;
    int heroX;          // где стоит герой
    int heroY;
    char underHero;     // что было под героем
    int mode;           // enum GameMode (1 = лес, 2 = пещера)
    int mapsTravelled;  // сколько карт пройдено
    int active;         // 1 = идёт приключение
};

struct Map* createMapObject(void);
void freeMap(struct Map** map);
void generateForest(struct Map* map);
void generateCave(struct Map* map);
int moveToNextMap(struct Map* map, struct Player* player, int* day, int* hour);
int moveHero(struct Map* map, int dx, int dy);
void printMap(const struct Map* map);
void printHUD(const struct Player* player, const struct Map* map, int day, int hour);
void startAdventure(struct Map** mapPtr, struct Player* player, int* day, int* hour);

#endif 
