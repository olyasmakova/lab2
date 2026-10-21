#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <windows.h>
#include <string.h>

#define MAX_ITEMS 10
#define MAX_NAME_LEN 64
#define MAX_FARMER_NAME 32

void loadItemNames(char names[][MAX_NAME_LEN]) {
    static const char* defaults[] = {
        "Пусто", "Мотыга", "Семена пшеницы", "Вода", "Удобрения",
        "Корзинка", "Морковь", "Лопата", "Картошка", "Лампочка"
    };

    FILE* file = fopen("items.txt", "r");
    if (file == NULL) {
        for (int i = 0; i < MAX_ITEMS; i++) {
            strncpy(names[i], defaults[i], MAX_NAME_LEN - 1);
            names[i][MAX_NAME_LEN - 1] = '\0';
        }
        printf("!Предупреждение! файл items.txt не найден. Использованы стандартные названия.\n");
        return;
    }

    for (int i = 0; i < MAX_ITEMS; i++) {
        char buffer[MAX_NAME_LEN];
        if (fgets(buffer, sizeof(buffer), file) != NULL) {
            size_t len = strlen(buffer);
            if (len > 0 && buffer[len - 1] == '\n') {
                buffer[len - 1] = '\0';
            }
            strncpy(names[i], buffer, MAX_NAME_LEN - 1);
            names[i][MAX_NAME_LEN - 1] = '\0';
        }
        else {
            strncpy(names[i], defaults[i], MAX_NAME_LEN - 1);
            names[i][MAX_NAME_LEN - 1] = '\0';
        }
    }
    fclose(file);
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

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    char farmer_name[MAX_FARMER_NAME];
    printf("Введите имя вашего фермера: ");
    if (fgets(farmer_name, sizeof(farmer_name), stdin) != NULL) {
        size_t len = strlen(farmer_name);
        if (len > 0 && farmer_name[len - 1] == '\n') {
            farmer_name[len - 1] = '\0';
        }
    }
    if (strlen(farmer_name) == 0) {
        strcpy(farmer_name, "Фермер");
    }

    char item_names[MAX_ITEMS][MAX_NAME_LEN];
    loadItemNames(item_names);

    int current_day = 1;
    int current_hour = 8;

    int inventory[MAX_ITEMS] = { 1, 0, 2, 3, 0, 4, 5, 0, 1, 2 };
    int choice;

    while (1) {
        printf("=== МЕНЮ ФЕРМЕРА (%s) ===\n", farmer_name);
        printf("[0] Выход\n");
        printf("[1] Посмотреть на часы\n");
        printf("[2] Промотать время (Поработать)\n");
        printf("[3] Посмотреть инвентарь\n");
        printf("[4] Положить предмет в слот\n");
        printf("[5] Выбросить предмет\n");
        printf("[6] Быстрый доступ (В8)\n");
        printf("Выберите действие: ");

        if (scanf("%d", &choice) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            printf("Ошибка. Введите число от 0 до 6.\n\n");
            continue;
        }

        switch (choice) {
        case 0:
            printf("Пока, фермер!\n");
            return 0;

        case 1:
            // П1: показать время
            printf("Сейчас день %d, время: %02d:00\n", current_day, current_hour);
            break;

        case 2: {
            // П2: промотать время
            int add_hours;
            printf("Сколько часов хотите промотать? ");

            if (scanf("%d", &add_hours) != 1) {
                int c;
                while ((c = getchar()) != '\n' && c != EOF);
                printf("Ошибка: введите корректное число часов.\n\n");
                break;
            }

            if (add_hours < 0) {
                printf("Ошибка: количество часов не может быть отрицательным.\n\n");
                break;
            }
    
            current_hour += add_hours;
            while (current_hour >= 24) {
                current_hour -= 24;
                current_day++;
            }
            printf("Время промотано. Теперь день %d, %02d:00.\n\n", current_day, current_hour);
            break;
        }

        case 3:
            printInventory(inventory, item_names);
            break;

        case 4: {
            // П4: положить предмет
            int slot_index, item_id;

            printf("В какой слот (0-%d) положить предмет? ", MAX_ITEMS - 1);
            if (scanf("%d", &slot_index) != 1) {
                int c;
                while ((c = getchar()) != '\n' && c != EOF);
                printf("Ошибка: введите корректный номер слота.\n\n");
                break;
            }
            if (slot_index < 0 || slot_index >= MAX_ITEMS) {
                printf("Ошибка: номер слота должен быть от 0 до %d.\n\n", MAX_ITEMS - 1);
                break;
            }

            printf("Какой ID предмета положить? ");
            if (scanf("%d", &item_id) != 1) {
                int c;
                while ((c = getchar()) != '\n' && c != EOF);
                printf("Ошибка: ID должен быть числом.\n\n");
                break;
            }
            if (item_id <= 0 || item_id >= MAX_ITEMS) {
                printf("Ошибка: ID должен быть от 1 до %d.\n\n", MAX_ITEMS - 1);
                break;
            }

            inventory[slot_index] = item_id;
            printf("Предмет '%s' (ID: %d) положен в слот %d.\n\n", item_names[item_id], item_id, slot_index);
            break;
        }


        case 5: {
            // П5: выбросить предмет
            int slot_index;

            printf("Из какого слота (0-%d) выбросить предмет? ", MAX_ITEMS - 1);
            if (scanf("%d", &slot_index) != 1) {
                int c;
                while ((c = getchar()) != '\n' && c != EOF);
                printf("Ошибка: введите корректный номер слота.\n\n");
                break;
            }
            if (slot_index < 0 || slot_index >= MAX_ITEMS) {
                printf("Ошибка: номер слота должен быть от 0 до %d.\n\n", MAX_ITEMS - 1);
                break;
            }

            if (inventory[slot_index] == 0) {
                printf("В этом слоте и так пусто.\n\n");
            }
            else {
                const char* itemName = item_names[inventory[slot_index]];
                inventory[slot_index] = 0;
                printf("Предмет '%s' из слота %d выброшен.\n\n", itemName, slot_index);
            }
            break;
        }


        case 6: {
            // В8: быстрый доступ — найти предмет и переместить в слот 0
            int target_id;

            printf("Введите ID предмета для быстрого доступа: ");
            if (scanf("%d", &target_id) != 1) {
                int c;
                while ((c = getchar()) != '\n' && c != EOF);
                printf("Ошибка: ID должен быть числом.\n\n");
                break;
            }
            if (target_id <= 0 || target_id >= MAX_ITEMS) {
                printf("Ошибка: ID должен быть от 1 до %d.\n\n", MAX_ITEMS - 1);
                break;
            }

            int found_index = -1;
            for (int i = 0; i < MAX_ITEMS; i++) {
                if (inventory[i] == target_id) {
                    found_index = i;
                    break;
                }
            }

            if (found_index == -1) {
                const char* missingName = item_names[target_id];
                printf("Предмет '%s' не найден в инвентаре.\n\n", missingName);
            }
            else {
                const char* itemName = item_names[inventory[found_index]];

                int temp = inventory[0];
                inventory[0] = inventory[found_index];
                inventory[found_index] = temp;

                printf("Предмет «%s» перемещён в слот 0.\n", itemName);
                printInventory(inventory, item_names);
            }
            break;
        }


        default:
            printf("Неверный выбор. Попробуйте еще раз.\n\n");
            break;
        }
    }

    return 0;
}