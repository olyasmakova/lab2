#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <windows.h>

#define inventory_size 10

void printInventory(const int inventory[]) {
    printf("Инвентарь:\n");
    for (int i = 0; i < inventory_size; i++) {
        printf("Слот %d: [%d]\n", i, inventory[i]);
    }
    printf("\n");
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

        // Защита от дурака
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
            printf("П1 бебебе\n\n");
            break;
        case 2:
            printf("П2 бебебе\n\n");
            break;
        case 3:
            printInventory(inventory);
            break;
        case 4:
            printf("П4 бебебе\n\n");
            break;
        case 5:
            printf("П5 бебебе\n\n");
            break;
        case 6:
            printf("П6 бебебе\n\n");
            break;
        default:
            printf("Неверный выбор. Попробуйте еще раз.\n\n");
            break;
        }
    }

    return 0;
}
