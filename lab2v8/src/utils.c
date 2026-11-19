#define _CRT_SECURE_NO_WARNINGS
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// имбуля чистит буфер после неудачного scanf
void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// защита от дурака читает число, пока не получит корректное
int getIntSafe(const char* prompt) {
    int value;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &value) == 1) {
            clearInputBuffer();
            return value;
        }
        clearInputBuffer();
        printf("Ошибка: введите целое число.\n");
    }
}

// П2: промотать время, но не дальше 02:00 ночи
void workTime(int* day, int* hour) {
    int add = getIntSafe("Сколько часов хотите промотать? ");
    if (add < 1) {
        printf("Ошибка: можно ждать минимум 1 час.\n\n");
        return;
    }

    // ближайшие 02:00 (если уже после полуночи — сегодня, иначе завтра)
    int limit;
    if (*hour < NIGHT_HOUR)
        limit = (*day) * DAY_HOURS + NIGHT_HOUR;
    else
        limit = (*day + 1) * DAY_HOURS + NIGHT_HOUR;

    int total = (*day) * DAY_HOURS + *hour + add;
    if (total > limit) {
        total = limit;
        printf("Дальше 02:00 без сна ждать нельзя! Фермер валится с ног.\n");
    }

    *day = total / DAY_HOURS;
    *hour = total % DAY_HOURS;
    printf("Время промотано. Теперь день %d, %02d:00.\n\n", *day, *hour);
}

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

void writeDiary(const char* farmerName, int day, int hour, const int inventory[], const char names[][MAX_NAME_LEN]) {
    FILE* diary = fopen("diary.txt", "a");
    if (diary == NULL) {
        printf("!Ошибка! Не удалось открыть diary.txt для записи.\n\n");
        return;
    }

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
    if (!hasItems) printf("  Инвентарь пуст.\n");
    printf("====================================\n\n");

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
    if (!hasItems) fprintf(diary, "  Инвентарь пуст.\n");
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

        if (strstr(buffer, "[INFO]") != NULL) countInfo++;
        if (strstr(buffer, "[WARN]") != NULL) countWarn++;
        if (strstr(buffer, "[ERROR]") != NULL) countError++;

        if (strstr(buffer, "[WARN]") != NULL || strstr(buffer, "[ERROR]") != NULL) {
            const char* start = NULL;
            if (strstr(buffer, "[WARN]") != NULL) {
                start = strstr(buffer, "[WARN]");
                if (start != NULL) start += 6;
            }
            else if (strstr(buffer, "[ERROR]") != NULL) {
                start = strstr(buffer, "[ERROR]");
                if (start != NULL) start += 7;
            }

            if (start != NULL) {
                while (*start == ' ') start++;
            }
            else {
                start = buffer;
            }
            fprintf(out, "%d: %s", lineNum, start);
        }
    }

    fclose(in);
    fclose(out);

    printf("\n=== АНАЛИЗ ЛОГОВ (%s) ===\n", inputFile);
    printf("Всего строк: %d\n", lineNum);
    printf("[INFO]: %d\n", countInfo);
    printf("[WARN]: %d\n", countWarn);
    printf("[ERROR]: %d\n", countError);
    printf("===========================\n\n");
}

void printMemoryFree(void* ptr) {
    printf("Память структуры по адресу %p освобождена\n", ptr);
}
