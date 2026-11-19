#define _CRT_SECURE_NO_WARNINGS
#include "player.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// [1] создание персонажа память под структуру выделяется динамически
struct Player* createPlayer(void) {
    struct Player* p = (struct Player*)malloc(sizeof(struct Player));
    if (p == NULL) return NULL;

    char buf[MAX_FARMER_NAME];
    printf("Введите имя вашего фермера: ");
    if (fgets(buf, sizeof(buf), stdin) != NULL) {
        size_t len = strlen(buf);
        if (len > 0 && buf[len - 1] == '\n') buf[len - 1] = '\0';
    }
    if (strlen(buf) == 0) strcpy(buf, "Фермер");

    strncpy(p->name, buf, MAX_FARMER_NAME - 1);
    p->name[MAX_FARMER_NAME - 1] = '\0';

    p->max_hp = START_MAX_HP;
    p->hp = p->max_hp;
    p->max_stamina = START_MAX_STAMINA;
    p->stamina = p->max_stamina;
    p->gold = 0;

    // стартовая реликвия, у героя пока ничего не надето
    p->active_relic.effect = RELIC_NONE;
    p->active_relic.name = NULL;

    // стартовый инвентарь
    static const int start_inventory[MAX_ITEMS] = { 1, 0, 2, 3, 0, 4, 5, 0, 1, 2 };
    for (int i = 0; i < MAX_ITEMS; i++) {
        p->inventory[i] = start_inventory[i];
    }

    return p;
}

// удаление персонажа с сообщением
void deletePlayer(struct Player** player) {
    if (*player == NULL) return;
    printMemoryFree(*player);
    free(*player);
    *player = NULL;
}

// [2] карточка персонажа
void showPlayerCard(const struct Player* player, int day, int hour, const char names[][MAX_NAME_LEN]) {
    printf("\n--- КАРТОЧКА ПЕРСОНАЖА ---\n");
    printf("Имя: %s\n", player->name);
    printf("Здоровье: %d/%d\n", player->hp, player->max_hp);
    printf("Выносливость: %d/%d\n", player->stamina, player->max_stamina);
    printf("Золото: %d\n", player->gold);
    printf("Сейчас день %d, время: %02d:00\n", day, hour);
    printInventory(player->inventory, names);
}

void printInventory(const int inventory[], const char names[][MAX_NAME_LEN]) {
    printf("\n--- ИНВЕНТАРЬ ФЕРМЕРА ---\n");
    for (int i = 0; i < MAX_ITEMS; i++) {
        int id = inventory[i];
        const char* name = names[id];

        if (id == 0) {
            printf("Слот %d: [%s]\n", i, name);
        }
        else {
            printf("Слот %d: [%s] (ID: %d)\n", i, name, id);
        }
    }
    printf("--------------------------\n\n");
}

// П4: положить предмет в слот
void putItem(int inventory[], const char names[][MAX_NAME_LEN]) {
    int slot_index, item_id;

    printf("В какой слот (0-%d) положить предмет? ", MAX_ITEMS - 1);
    if (scanf("%d", &slot_index) != 1) {
        clearInputBuffer();
        printf("Ошибка: введите корректный номер слота.\n\n");
        return;
    }
    if (slot_index < 0 || slot_index >= MAX_ITEMS) {
        printf("Ошибка: номер слота должен быть от 0 до %d.\n\n", MAX_ITEMS - 1);
        return;
    }

    printf("Какой ID предмета положить? ");
    if (scanf("%d", &item_id) != 1) {
        clearInputBuffer();
        printf("Ошибка: ID должен быть числом.\n\n");
        return;
    }
    if (item_id <= 0 || item_id >= MAX_ITEMS) {
        printf("Ошибка: ID должен быть от 1 до %d.\n\n", MAX_ITEMS - 1);
        return;
    }

    inventory[slot_index] = item_id;
    printf("Предмет '%s' (ID: %d) положен в слот %d.\n\n", names[item_id], item_id, slot_index);
}

// П5: выбросить предмет из слота
void dropItem(int inventory[], const char names[][MAX_NAME_LEN]) {
    int slot_index;

    printf("Из какого слота (0-%d) выбросить предмет? ", MAX_ITEMS - 1);
    if (scanf("%d", &slot_index) != 1) {
        clearInputBuffer();
        printf("Ошибка: введите корректный номер слота.\n\n");
        return;
    }
    if (slot_index < 0 || slot_index >= MAX_ITEMS) {
        printf("Ошибка: номер слота должен быть от 0 до %d.\n\n", MAX_ITEMS - 1);
        return;
    }

    if (inventory[slot_index] == 0) {
        printf("В этом слоте и так пусто.\n\n");
    }
    else {
        const char* itemName = names[inventory[slot_index]];
        inventory[slot_index] = 0;
        printf("Предмет '%s' из слота %d выброшен.\n\n", itemName, slot_index);
    }
}

// В8: быстрый доступ — переместить предмет в слот 0
void quickAccess(int inventory[], const char names[][MAX_NAME_LEN]) {
    int target_id;

    printf("Введите ID предмета для быстрого доступа: ");
    if (scanf("%d", &target_id) != 1) {
        clearInputBuffer();
        printf("Ошибка: ID должен быть числом.\n\n");
        return;
    }
    if (target_id <= 0 || target_id >= MAX_ITEMS) {
        printf("Ошибка: ID должен быть от 1 до %d.\n\n", MAX_ITEMS - 1);
        return;
    }

    int found_index = -1;
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (inventory[i] == target_id) {
            found_index = i;
            break;
        }
    }

    if (found_index == -1) {
        printf("Предмет '%s' не найден в инвентаре.\n\n", names[target_id]);
    }
    else {
        const char* itemName = inventory[found_index] == 0 ? names[0] : names[inventory[found_index]];

        int temp = inventory[0];
        inventory[0] = inventory[found_index];
        inventory[found_index] = temp;

        printf("Предмет «%s» перемещён в слот 0.\n", itemName);
        printInventory(inventory, names);
    }
}

// поиск предмета по названию
void searchItemByName(const int inventory[], const char names[][MAX_NAME_LEN]) {
    char searchName[MAX_NAME_LEN];

    clearInputBuffer();

    printf("Введите название предмета для поиска (например Мотыга): ");

    if (fgets(searchName, sizeof(searchName), stdin) == NULL) {
        printf("Ошибка ввода.\n\n");
        return;
    }

    size_t len = strlen(searchName);
    if (len > 0 && searchName[len - 1] == '\n') {
        searchName[len - 1] = '\0';
    }

    int found = 0;

    for (int i = 0; i < MAX_ITEMS; i++) {
        if (strcmp(names[inventory[i]], searchName) == 0) {
            printf("Найден! слот %d — '%s' (ID: %d)\n", i, names[inventory[i]], inventory[i]);
            found = 1;
        }
    }

    if (!found) {
        printf("Предмет с названием '%s' не найден в инвентаре.\n", searchName);
    }
    printf("\n");
}
