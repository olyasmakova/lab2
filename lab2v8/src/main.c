#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <windows.h>

#define inventory_size 10

const char* getItemName(int id) {
    switch (id) {
    case 1: return "мотыга";
    case 2: return "семена пшеницы";
    case 3: return "вода";
    case 4: return "удобрения";
    case 5: return "корзинка";
    default: return "тут пусто";
    }
}

void printInventory(const int inventory[]) {
    printf("\n--- ИНВЕНТАРЬ ФЕРМЕРА ---\n");
    for (int i = 0; i < inventory_size; i++) {
        const char* name = getItemName(inventory[i]);

        if (inventory[i] == 0) {
            printf("Слот %d: [тут пусто]\n", i);
        }
        else {
            printf("Слот %d: [%s] (ID: %d)\n", i, name, inventory[i]);
        }
    }
    printf("--------------------------\n\n");
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    int current_day = 1;
    int current_hour = 8;

    int inventory[inventory_size] = { 1, 0, 2, 3, 0, 4, 5, 0, 1, 2 };
    int choice;

    while (1) {
        printf("=== МЕНЮ ФЕРМЕРА ===\n");
        printf("[0] Выход\n");
        printf("[1] Посмотреть на часы\n");
        printf("[2] Промотать время (Поработать)\n");
        printf("[3] Посмотреть инвентарь\n");
        printf("[4] Положить предмет в слот\n");
        printf("[5] Выбросить предмет\n");
        printf("[6] Быстрый доступ (В8)\n");
        printf("Выберите действие: ");

        // защита от дурака
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
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
            if (scanf("%d", &add_hours) != 1 || add_hours < 0) {
                while (getchar() != '\n');
                printf("Ошибка: введите неотрицательное число.\n\n");
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
            printInventory(inventory);
            break;

        case 4: {
            // П4: положить предмет
            int slot_index, item_id;
            printf("В какой слот (0-%d) положить предмет? ", inventory_size - 1);
            if (scanf("%d", &slot_index) != 1 || slot_index < 0 || slot_index >= inventory_size) {
                while (getchar() != '\n');
                printf("Ошибка: неверный номер слота.\n\n");
                break;
            }

            printf("Какой ID предмета положить? ");
            if (scanf("%d", &item_id) != 1 || item_id <= 0) {
                while (getchar() != '\n');
                printf("Ошибка: ID должен быть положительным числом.\n\n");
                break;
            }

            inventory[slot_index] = item_id;
            const char* itemName = getItemName(item_id);
            printf("Предмет '%s' (ID: %d) положен в слот %d.\n\n", itemName, item_id, slot_index);
            break;
        }

        case 5: {
            // П5: выбросить предмет
            int slot_index;
            const char* itemName = NULL;

            printf("Из какого слота (0-%d) выбросить предмет? ", inventory_size - 1);

            if (scanf("%d", &slot_index) != 1 || slot_index < 0 || slot_index >= inventory_size) {
                while (getchar() != '\n');
                printf("Ошибка: неверный номер слота.\n\n");
                break;
            }

            if (inventory[slot_index] == 0) {
                printf("В этом слоте и так пусто.\n\n");
            }
            else {
                itemName = getItemName(inventory[slot_index]);
                inventory[slot_index] = 0;

                printf("Предмет '%s' из слота %d выброшен.\n\n", itemName, slot_index);
            }
            break;
        }

        case 6: {
            // В8: быстрый доступ — найти предмет и переместить в слот 0
            int target_id;
            printf("Введите ID предмета для быстрого доступа: ");

            if (scanf("%d", &target_id) != 1 || target_id <= 0) {
                while (getchar() != '\n');
                printf("Ошибка: неверный ID.\n\n");
                break;
            }

            int found_index = -1;
            for (int i = 0; i < inventory_size; i++) {
                if (inventory[i] == target_id) {
                    found_index = i;
                    break;
                }
            }

            if (found_index == -1) {
                const char* missingName = getItemName(target_id);
                printf("Предмет '%s' не найден в инвентаре.\n\n", missingName);
            }
            else {
                const char* itemName = getItemName(inventory[found_index]);

                int temp = inventory[0];
                inventory[0] = inventory[found_index];
                inventory[found_index] = temp;

                printf("Предмет «%s» перемещён в слот 0.\n", itemName);
                printInventory(inventory);
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