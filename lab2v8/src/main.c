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

void searchItemByName(const int inventory[], const char names[][MAX_NAME_LEN]) {
    char searchName[MAX_NAME_LEN];

    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    printf("Введите название предмета для поиска (например Мотыга): ");

    // тут строка читается
    if (fgets(searchName, sizeof(searchName), stdin) == NULL) {
        printf("Ошибка ввода.\n\n");
        return;
    }

    // убираем \n если он есть 
    size_t len = strlen(searchName);
    if (len > 0 && searchName[len - 1] == '\n') {
        searchName[len - 1] = '\0';
    }

    int found = 0; // нашли хоть один предмет или нет

    for (int i = 0; i < MAX_ITEMS; i++) {
        // сравниваем название предмета в слоте с тем что ввели
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

void writeDiary(const char* farmerName, int day, int hour, const int inventory[], const char names[][MAX_NAME_LEN]) {
    FILE* diary = fopen("diary.txt", "a");
    if (diary == NULL) {
        printf("!Ошибка! Не удалось открыть diary.txt для записи.\n\n");
        return;
    }

    // тут вывод на экран для игрока
    printf("\n=== ЗАПИСЬ В ДНЕВНИКЕ ===\n");
    printf("Фермер: %s\n", farmerName);
    printf("Дата: День %d, Час %d\n", day, hour);
    printf("Содержимое инвентаря:\n");

    int hasItems = 0;
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (inventory[i] != 0) {
            printf("  Слот %d: %s (ID: %d)\n", i, names[inventory[i]], inventory[i]);
            hasItems = 1;
        }
    }
    if (!hasItems) {
        printf("  Инвентарь пуст.\n");
    }
    printf("====================================\n\n");

    // а тут то же самое в файлик
    fprintf(diary, "=== ЗАПИСЬ В ДНЕВНИКЕ ===\n");
    fprintf(diary, "Фермер: %s\n", farmerName);
    fprintf(diary, "Дата: День %d, Час %d\n", day, hour);
    fprintf(diary, "Содержимое инвентаря:\n");

    hasItems = 0;
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (inventory[i] != 0) {
            fprintf(diary, "  Слот %d: %s (ID: %d)\n", i, names[inventory[i]], inventory[i]);
            hasItems = 1;
        }
    }
    if (!hasItems) {
        fprintf(diary, "  Инвентарь пуст.\n");
    }
    fprintf(diary, "=========================\n\n");

    fclose(diary);
    printf("Запись сохранена в diary.txt\n\n");
}

void analyzeLogs(const char* inputFile, const char* outputFile) {
    FILE* in = fopen(inputFile, "r");
    if (in == NULL) {
        printf("!Ошибка! Не удалось открыть %s для чтения. Создайте файл в папке запуска.\n\n", inputFile);
        return;
    }

    FILE* out = fopen(outputFile, "w");
    if (out == NULL) {
        printf("!Ошибка! Не удалось создать %s для записи.\n\n", outputFile);
        fclose(in);
        return;
    }

    char buffer[512];
    int lineNum = 0;
    int countInfo = 0, countWarn = 0, countError = 0;

    while (fgets(buffer, sizeof(buffer), in) != NULL) {
        lineNum++;

        // Ищем теги
        if (strstr(buffer, "[INFO]") != NULL) {
            countInfo++;
        }
        if (strstr(buffer, "[WARN]") != NULL) {
            countWarn++;
        }
        if (strstr(buffer, "[ERROR]") != NULL) {
            countError++;
        }

        // если warn или error пишем в output без тега и с номером строки
        if (strstr(buffer, "[WARN]") != NULL || strstr(buffer, "[ERROR]") != NULL) {
            // находим начало текста после тега
            const char* start = NULL;
            if (strstr(buffer, "[WARN]") != NULL) {
                start = strstr(buffer, "[WARN]");
                if (start != NULL) start += 6; // пропускаем warn (6 символов)
            }
            else if (strstr(buffer, "[ERROR]") != NULL) {
                start = strstr(buffer, "[ERROR]");
                if (start != NULL) start += 7; // пропускаем error (7 символов)
            }

            // eсли нашли пропускаем возможные пробелы после тега
            if (start != NULL) {
                while (*start == ' ') start++;
            }
            else {
                // если тег есть но логика не сработала пишем как есть
                start = buffer;
            }

            fprintf(out, "%d: %s", lineNum, start);
        }
    }

    fclose(in);
    fclose(out);

    // dывод статистики на экран
    printf("\n=== АНАЛИЗ ЛОГОВ (%s) ===\n", inputFile);
    printf("Всего строк: %d\n", lineNum);
    printf("[INFO]: %d\n", countInfo);
    printf("[WARN]: %d\n", countWarn);
    printf("[ERROR]: %d\n", countError);
    printf("===========================\n\n");
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
        printf("[7] Поиск предмета по названию\n");
        printf("[8] Сделать запись в дневник (diary.txt)\n");
        printf("[9] Анализатор логов (input.txt → output.txt)\n");
        printf("Выберите действие: ");

        if (scanf("%d", &choice) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            printf("Ошибка. Введите число от 0 до 9.\n\n");
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

        case 7: //а тут обработка вывода
            searchItemByName(inventory, item_names);
            break;

        case 8:
            writeDiary(farmer_name, current_day, current_hour, inventory, item_names);
            break;

        case 9:
            analyzeLogs("input.txt", "output.txt");
            break;

        default:
            printf("Неверный выбор. Попробуйте еще раз.\n\n");
            break;
        }
    }

    return 0;
}