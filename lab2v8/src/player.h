#ifndef PLAYER_H
#define PLAYER_H

#include "utils.h"

struct Player {
    char name[MAX_FARMER_NAME];
    int hp;
    int max_hp;
    int stamina;
    int max_stamina;
    int gold;
    int inventory[MAX_ITEMS];
    Relic active_relic;  // слот под экипированную реликвию
};

typedef enum {
    RELIC_NONE = 0,
    RELIC_GREED,      // удваивает золото за пройденную карту
    RELIC_QUICK,      // переход на карту стоит 1 стамину вместо 5
    RELIC_HEAL        // восстанавливает 1 хп за шаг
} RelicEffect;

typedef struct {
    RelicEffect effect;
    const char* name;
} Relic;

struct Player* createPlayer(void);
void deletePlayer(struct Player** player);
void showPlayerCard(const struct Player* player, int day, int hour, const char names[][MAX_NAME_LEN]);

void printInventory(const int inventory[], const char names[][MAX_NAME_LEN]);
void putItem(int inventory[], const char names[][MAX_NAME_LEN]);
void dropItem(int inventory[], const char names[][MAX_NAME_LEN]);
void quickAccess(int inventory[], const char names[][MAX_NAME_LEN]);
void searchItemByName(const int inventory[], const char names[][MAX_NAME_LEN]);

#endif 
