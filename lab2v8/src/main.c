#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#include "utils.h"
#include "player.h"
#include "map.h"

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    srand((unsigned)time(NULL));

    // персонажа изначально нет
    struct Player* player = NULL;
    struct Map* map = NULL;

    char item_names[MAX_ITEMS][MAX_NAME_LEN];
    loadItemNames(item_names);

    int current_day = START_DAY;
    int current_hour = START_HOUR;

    while (1) {
        printf("=== МЕНЮ ФЕРМЕРА ===\n");
        printf("[0] Выход\n");
        printf("[1] Создать / пересоздать персонажа\n");
        printf("[2] Карточка персонажа\n");
        printf("[3] Время и режим дня\n");
        printf("[4] Посмотреть инвентарь\n");
        printf("[5] Положить предмет в слот\n");
        printf("[6] Выбросить предмет\n");
        printf("[7] Быстрый доступ (В8)\n");
        printf("[8] Поиск предмета по названию\n");
        printf("[9] Сделать запись в дневник (diary.txt)\n");
        printf("[10] Отправиться в приключение\n");
        printf("[11] Анализатор логов (input.txt -> output.txt)\n");

        int choice = getIntSafe("Выберите действие: ");

        switch (choice) {
        case 0:
            deletePlayer(&player);
            freeMap(&map);
            printf("Пока, фермер!\n");
            return 0;

        case 1:
            ensurePlayer(&player);
            break;

        case 2:
            if (!playerExists(player)) break;
            showPlayerCard(player, current_day, current_hour, item_names);
            break;

        case 3:
            showTimeMenu(player, &current_day, &current_hour);
            break;

        case 4:
            if (!playerExists(player)) break;
            printInventory(player->inventory, item_names);
            break;

        case 5:
            if (!playerExists(player)) break;
            putItem(player->inventory, item_names);
            break;

        case 6:
            if (!playerExists(player)) break;
            dropItem(player->inventory, item_names);
            break;

        case 7:
            if (!playerExists(player)) break;
            quickAccess(player->inventory, item_names);
            break;

        case 8:
            if (!playerExists(player)) break;
            searchItemByName(player->inventory, item_names);
            break;

        case 9:
            if (!playerExists(player)) break;
            writeDiary(player->name, current_day, current_hour, player->inventory, item_names);
            break;

        case 10:
            if (!playerExists(player)) break;
            if (player->stamina <= 0) {
                printf("Выносливость на нуле! Сначала отдохните (пункт [3] -> Отдых).\n\n");
                break;
            }
            if (current_hour == NIGHT_HOUR) {
                printf("Уже 02:00 ночи — в приключение нельзя! Пора спать.\n\n");
                break;
            }
            startAdventure(&map, player, &current_day, &current_hour);
            break;

        case 11:
            analyzeLogs(INPUT_LOG_FILE, OUTPUT_LOG_FILE);
            break;

        default:
            printf("Неверный выбор. Попробуйте еще раз.\n\n");
            break;
        }
    }
}
