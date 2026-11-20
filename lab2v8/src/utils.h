#ifndef UTILS_H
#define UTILS_H

#define DAY_HOURS 24
#define MAX_ITEMS 10
#define MAX_NAME_LEN 64
#define MAX_FARMER_NAME 32

// константы приключения
#define MAP_MIN_SIZE    5
#define MAP_MAX_SIZE    30
#define OBSTACLE_CHANCE 20

#define CAVE_MIN_SIZE  5
#define CAVE_MAX_SIZE  8
#define CAVE_WALL_CHANCE 25

// символы на карте
#define CHAR_WALL   '#'
#define CHAR_EMPTY  '.'
#define CHAR_GRASS  ','
#define CHAR_TREE   'T'
#define CHAR_ROCK   'R'
#define CHAR_HERO   '@'
#define CHAR_CAVE   'O'
#define CHAR_CAMPFIRE '^'

// константы нового задания
#define START_DAY          1
#define START_HOUR         8
#define SLEEP_HOUR         8      // во сколько просыпаемся после сна
#define NIGHT_HOUR         2      // 02:00 граница ночи
#define START_MAX_HP       100
#define START_MAX_STAMINA  50
#define BASE_REWARD        10     // базовый множитель награды за пройденную карту
#define FLOOR_STAMINA_COST 5      // стамина за переход на новуюкарту
#define STEP_STAMINA_COST  1      // стамина за шаг

#define RELIC_DROP_CHANCE  20  // шанс 20% найти реликвию при переходе
#define RELIC_SELL_PRICE   25  // цена продажи реликвии на месте
#define RELIC_QUICK_COST   1   // стоимость перехода с сапогами скороходами

// перечисление типов карт
enum GameMode {
    MODE_FOREST = 1,
    MODE_CAVE = 2
};

void clearInputBuffer(void);
int getIntSafe(const char* prompt);
void workTime(int* day, int* hour);
void loadItemNames(char names[][MAX_NAME_LEN]);
void writeDiary(const char* farmerName, int day, int hour, const int inventory[], const char names[][MAX_NAME_LEN]);
void analyzeLogs(const char* inputFile, const char* outputFile);
void printMemoryFree(void* ptr);

#endif // UTILS_H