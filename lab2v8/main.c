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
            // Пункт 1: показать время (с %02d, чтобы было 08:00, а не 8:00)
            printf("Сейчас день %d, время: %02d:00\n", current_day, current_hour);
            break;

        case 2: {
            // Пункт 2: промотать время
            int add_hours;
            printf("Сколько часов хотите промотать? ");
            if (scanf("%d", &add_hours) != 1 || add_hours < 0) {
                while (getchar() != '\n'); // очистка буфера
                printf("Ошибка: введите неотрицательное число.\n\n");
                break;
            }

            current_hour += add_hours;
            // Если часы ушли за 24, вычитаем и увеличиваем день
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
