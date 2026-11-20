#ifndef UTILS_H
#define UTILS_H

#include "config.h"

void clearInputBuffer(void);
int getIntSafe(const char* prompt);
void workTime(int* day, int* hour);
void loadItemNames(char names[][MAX_NAME_LEN]);
void writeDiary(const char* farmerName, int day, int hour, const int inventory[], const char names[][MAX_NAME_LEN]);
void analyzeLogs(const char* inputFile, const char* outputFile);
void printMemoryFree(void* ptr);

#endif // UTILS_H