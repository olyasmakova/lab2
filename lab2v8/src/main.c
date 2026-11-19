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

        case 1: {
            if (player != NULL) {
                printf("Персонаж '%s' уже существует. Точно удалить и создать нового? (1 = да, 0 = нет)\n", player->name);
                int confirm = getIntSafe("");
                if (confirm != 1) {
                    printf("Ок, оставляем старого.\n\n");
                    break;
                }
                deletePlayer(&player);
            }
            player = createPlayer();
            if (player == NULL) {
                printf("!Ошибка! Не удалось выделить память под персонажа.\n\n");
            }
            else {
                printf("Персонаж '%s' создан!\n\n", player->name);
            }
            break;
        }

        case 2:
            if (player == NULL) {
                printf("Персонаж ещё не создан. Сначала выберите пункт [1].\n\n");
                break;
            }
            showPlayerCard(player, current_day, current_hour, item_names);
            break;

        case 3: {
            int sub;
            do {
                printf("\n=== ВРЕМЯ И РЕЖИМ ДНЯ ===\n");
                printf("[1] Посмотреть на часы\n");
                printf("[2] Пропустить время (Ждать)\n");
                printf("[3] Отдых (Лечь спать)\n");
                printf("[0] Назад\n");
                sub = getIntSafe("Выберите действие: ");

                switch (sub) {
                case 1:
                    printf("Сейчас день %d, время: %02d:00\n\n", current_day, current_hour);
                    break;
                case 2:
                    workTime(&current_day, &current_hour);
                    break;
                case 3:
                    current_day++;
                    current_hour = SLEEP_HOUR;
                    if (player != NULL) {
                        player->hp = player->max_hp;
                        player->stamina = player->max_stamina;
                    }
                    printf("Вы выспались. Наступило утро: день %d, %02d:00\n\n", current_day, current_hour);
                    break;
                case 0:
                    break;
                default:
                    printf("Неверный выбор.\n\n");
                    break;
                }
            } while (sub != 0);
            break;
        }

        case 4:
            if (player == NULL) {
                printf("Персонаж ещё не создан. Сначала выберите пункт [1].\n\n");
                break;
            }
            printInventory(player->inventory, item_names);
            break;

        case 5:
            if (player == NULL) {
                printf("Персонаж ещё не создан. Сначала выберите пункт [1].\n\n");
                break;
            }
            putItem(player->inventory, item_names);
            break;

        case 6:
            if (player == NULL) {
                printf("Персонаж ещё не создан. Сначала выберите пункт [1].\n\n");
                break;
            }
            dropItem(player->inventory, item_names);
            break;

        case 7:
            if (player == NULL) {
                printf("Персонаж ещё не создан. Сначала выберите пункт [1].\n\n");
                break;
            }
            quickAccess(player->inventory, item_names);
            break;

        case 8:
            if (player == NULL) {
                printf("Персонаж ещё не создан. Сначала выберите пункт [1].\n\n");
                break;
            }
            searchItemByName(player->inventory, item_names);
            break;

        case 9:
            if (player == NULL) {
                printf("Персонаж ещё не создан. Сначала выберите пункт [1].\n\n");
                break;
            }
            writeDiary(player->name, current_day, current_hour, player->inventory, item_names);
            break;

        case 10:
            if (player == NULL) {
                printf("Персонаж ещё не создан. Сначала выберите пункт [1].\n\n");
                break;
            }
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
            analyzeLogs("input.txt", "output.txt");
            break;

        default:
            printf("Неверный выбор. Попробуйте еще раз.\n\n");
            break;
        }
    }
}
