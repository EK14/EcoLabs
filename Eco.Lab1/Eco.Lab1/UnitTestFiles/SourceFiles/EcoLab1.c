/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   EcoLab1
 * </сводка>
 *
 * <описание>
 *   Данный исходный файл является точкой входа
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2018 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */


/* Eco OS */
#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"
#include "IdEcoInterfaceBus1.h"
#include "IdEcoFileSystemManagement1.h"
#include "IdEcoLab1.h"
#include "IEcoCalculatorY.h"
#include "IEcoCalculatorX.h"

#include "IEcoConnectionPointContainer.h"
#include "IEcoConnectionPoint.h"
#include "IEcoLab1Events.h"
#include "CEcoLab1Sink.h"

void fillIntArray(int *arr, int arrSize) {
    for (size_t i = 0; i < arrSize; i ++) {
        arr[i] = rand() % 10;
    }
}

void fillFloatArray(float arr[], int size) {
    for (int i = 0; i < size; ++i) {
        arr[i] = (float)rand() + ((float)rand() / RAND_MAX);
    }
}

void fillDoubleArray(int *arr, int arrSize) {
    for (int i = 0; i < arrSize; ++i) {
        arr[i] = (double)rand() + ((double)rand() / RAND_MAX);
    }
}

void fillStringArray(char **arr, int arrSize, IEcoMemoryAllocator1 *pIMem) {
    for (size_t i = 0; i < arrSize; i++) {
        arr[i] = (char *) pIMem->pVTbl->Alloc(pIMem, 20 * sizeof(char));
        size_t str_size = rand() % 10;
        for (size_t j = 0; j < str_size; ++j) {
            arr[i][j] = (char) (rand() % ('z' - 'a' + 1) + 'a');
        }
        arr[i][str_size] = 0;
    }
}

// Указатель на функцию сравнения для типа int
int compareInt(const void *a_ptr, const void *b_ptr) {
    const int a = *(int *)a_ptr;
    const int b = *(int *)b_ptr;
    return (a > b) - (a < b);
}

// Указатель на функцию сравнения для типа float
int compareFloat(const void *a, const void *b) {
    if (*(float*)a < *(float*)b) return -1;
    if (*(float*)a > *(float*)b) return 1;
    return 0;
}

// Указатель на функцию сравнения для типа double
int compareDouble(const void *a, const void *b) {
    if (*(double*)a < *(double*)b) return -1;
    if (*(double*)a > *(double*)b) return 1;
    return 0;
}

int compareString(const void *a_ptr, const void *b_ptr) {
    char *a = *(char **)a_ptr, *b = *(char **)b_ptr;
    return strcmp(a, b);
}

void testIntSort(IEcoLab1 *pIEcoLab1, FILE *file, int arrSize, IEcoMemoryAllocator1 *pIMem) {
    clock_t start, end;
    int* arr;

    //treeSort
    arr = (int *) pIMem->pVTbl->Alloc(pIMem, arrSize * sizeof(int));
    fillIntArray(arr, arrSize);
    start = clock();

    pIEcoLab1->pVTbl->qsort(pIEcoLab1, arr, arrSize, sizeof(int), compareInt);

    end = clock();
    double insertionSortDuration = (double)(end - start) / CLOCKS_PER_SEC;
    pIMem->pVTbl->Free(pIMem, arr);

    //qsort
    arr = (int *) pIMem->pVTbl->Alloc(pIMem, arrSize * sizeof(int));
    fillIntArray(arr, arrSize);
    start = clock();

    qsort(arr, arrSize, sizeof(int), compareInt);

    end = clock();
    double qsortDuration = (double)(end - start) / CLOCKS_PER_SEC;
    pIMem->pVTbl->Free(pIMem, arr);

    fprintf(file, "%s,%s,%d,%lf\n", "treeSort", "int", arrSize, insertionSortDuration);
    fprintf(file, "%s,%s,%d,%lf\n", "qsort", "int", arrSize, qsortDuration);
}

void testDoubleSort(IEcoLab1 *pIEcoLab1, FILE *file, int arrSize, IEcoMemoryAllocator1 *pIMem) {
    clock_t start, end;
    double* arr;

    //treeSort
    arr = (double *) pIMem->pVTbl->Alloc(pIMem, arrSize * sizeof(double));
    fillDoubleArray(arr, arrSize);
    start = clock();

    pIEcoLab1->pVTbl->qsort(pIEcoLab1, arr, arrSize, sizeof(double), compareDouble);

    end = clock();
    double insertionSortDuration = (double)(end - start) / CLOCKS_PER_SEC;
    pIMem->pVTbl->Free(pIMem, arr);

    //qsort
    arr = (double *) pIMem->pVTbl->Alloc(pIMem, arrSize * sizeof(double ));
    fillDoubleArray(arr, arrSize);
    start = clock();

    qsort(arr, arrSize, sizeof(double), compareDouble);

    end = clock();
    double qsortDuration = (double)(end - start) / CLOCKS_PER_SEC;
    pIMem->pVTbl->Free(pIMem, arr);

    fprintf(file, "%s,%s,%d,%lf\n", "treeSort", "double", arrSize, insertionSortDuration);
    fprintf(file, "%s,%s,%d,%lf\n", "qsort", "double", arrSize, qsortDuration);
}

void testFloatSort(IEcoLab1 *pIEcoLab1, FILE *file, int arrSize, IEcoMemoryAllocator1 *pIMem) {
    clock_t start, end;
    float* arr;

    //treeSort
    arr = (float *) pIMem->pVTbl->Alloc(pIMem, arrSize * sizeof(float));
    fillFloatArray(arr, arrSize);
    start = clock();

    pIEcoLab1->pVTbl->qsort(pIEcoLab1, arr, arrSize, sizeof(float), compareFloat);

    end = clock();
    double insertionSortDuration = (double)(end - start) / CLOCKS_PER_SEC;
    pIMem->pVTbl->Free(pIMem, arr);

    //qsort
    arr = (float *) pIMem->pVTbl->Alloc(pIMem, arrSize * sizeof(float));
    fillFloatArray(arr, arrSize);
    start = clock();

    qsort(arr, arrSize, sizeof(float), compareFloat);

    end = clock();
    double qsortDuration = (double)(end - start) / CLOCKS_PER_SEC;
    pIMem->pVTbl->Free(pIMem, arr);

    fprintf(file, "%s,%s,%d,%lf\n", "treeSort", "float", arrSize, insertionSortDuration);
    fprintf(file, "%s,%s,%d,%lf\n", "qsort", "float", arrSize, qsortDuration);
}

void testStringSort(IEcoLab1 *pIEcoLab1, FILE *file, int arrSize, IEcoMemoryAllocator1 *pIMem) {
    clock_t start, end;
    char** arr;

    //treeSort
    arr = (char **) pIMem->pVTbl->Alloc(pIMem, arrSize * sizeof(char *));
    fillStringArray(arr, arrSize, pIMem);
    start = clock();

    pIEcoLab1->pVTbl->qsort(pIEcoLab1, arr, arrSize, sizeof(char *), compareString);

    end = clock();
    double insertionSortDuration = (double)(end - start) / CLOCKS_PER_SEC;
    for (size_t i = 0; i < arrSize; ++i) {
        pIMem->pVTbl->Free(pIMem, arr[i]);
    }
    pIMem->pVTbl->Free(pIMem, arr);

    //qsort
    arr = (char **) pIMem->pVTbl->Alloc(pIMem, arrSize * sizeof(char *));
    fillStringArray(arr, arrSize, pIMem);
    start = clock();

    qsort(arr, arrSize, sizeof(char *), compareString);

    end = clock();
    double qsortDuration = (double)(end - start) / CLOCKS_PER_SEC;
    for (size_t i = 0; i < arrSize; ++i) {
        pIMem->pVTbl->Free(pIMem, arr[i]);
    }
    pIMem->pVTbl->Free(pIMem, arr);

    fprintf(file, "%s,%s,%d,%lf\n", "treeSort", "string", arrSize, insertionSortDuration);
    fprintf(file, "%s,%s,%d,%lf\n", "qsort", "string", arrSize, qsortDuration);
}

void testSort(IEcoLab1 *pIEcoLab1, int arrSize, IEcoMemoryAllocator1 *pIMem) {
    int* arr;
    arr = (int *) pIMem->pVTbl->Alloc(pIMem, arrSize * sizeof(int));
    fillIntArray(arr, arrSize);
    pIEcoLab1->pVTbl->qsort(pIEcoLab1, arr, arrSize, sizeof(int), compareInt);
    pIMem->pVTbl->Free(pIMem, arr);
}

/*
 *
 * <сводка>
 *   Функция EcoMain
 * </сводка>
 *
 * <описание>
 *   Функция EcoMain - точка входа
 * </описание>
 *
 */
int16_t EcoMain(IEcoUnknown* pIUnk) {
    int16_t result = -1;
    /* Указатель на системный интерфейс */
    IEcoSystem1* pISys = 0;
    /* Указатель на интерфейс работы с системной интерфейсной шиной */
    IEcoInterfaceBus1* pIBus = 0;
    /* Указатель на интерфейс работы с памятью */
    IEcoMemoryAllocator1* pIMem = 0;
    char_t* name = 0;
    char_t* copyName = 0;
    /* Указатель на тестируемый интерфейс */
    IEcoLab1* pIEcoLab1 = 0;

    IEcoConnectionPointContainer* pICPC = 0;
    IEcoConnectionPoint* pICP = 0;
    IEcoLab1Events* pIEcoLab1Sink = 0;
    IEcoUnknown* pISinkUnk = 0;
    uint32_t cAdvise = 0;

    IEcoCalculatorX* pIX = 0;
    IEcoCalculatorY* pIY = 0;
    srand(time(0));

    int arrSizes[5] = {10000, 50000, 100000, 150000, 200000};


    /* Проверка и создание системного интрефейса */
    if (pISys == 0) {
        result = pIUnk->pVTbl->QueryInterface(pIUnk, &GID_IEcoSystem1, (void **)&pISys);
        if (result != 0 && pISys == 0) {
        /* Освобождение системного интерфейса в случае ошибки */
            goto Release;
        }
    }

    /* Получение интерфейса для работы с интерфейсной шиной */
    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);
    if (result != 0 || pIBus == 0) {
        /* Освобождение в случае ошибки */
        goto Release;
    }
#ifdef ECO_LIB
    /* Регистрация статического компонента для работы со списком */
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoLab1, (IEcoUnknown*)GetIEcoComponentFactoryPtr_1F5DF16EE1BF43B999A434ED38FE8F3A);
    if (result != 0 ) {
        /* Освобождение в случае ошибки */
        goto Release;
    }
#endif
    /* Получение интерфейса управления памятью */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoMemoryManager1, 0, &IID_IEcoMemoryAllocator1, (void**) &pIMem);

    /* Проверка */
    if (result != 0 || pIMem == 0) {
        /* Освобождение системного интерфейса в случае ошибки */
        goto Release;
    }

    /* Выделение блока памяти */
    name = (char_t *)pIMem->pVTbl->Alloc(pIMem, 10);

    /* Заполнение блока памяти */
    pIMem->pVTbl->Fill(pIMem, name, 'a', 9);


    /* Получение тестируемого интерфейса */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoLab1, 0, &IID_IEcoLab1, (void**) &pIEcoLab1);
    if (result != 0 || pIEcoLab1 == 0) {
        /* Освобождение интерфейсов в случае ошибки */
        goto Release;
    }

    printf("IEcoLab1 component\n");

    result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoConnectionPointContainer, (void **)&pICPC);
    if (result != 0 || pICPC == 0) {
        /* Освобождение интерфейсов в случае ошибки */
        goto Release;
    }

    printf("IEcoConnectionPointContainer\n");

    result = pICPC->pVTbl->FindConnectionPoint(pICPC, &IID_IEcoLab1Events, &pICP);
    if (result != 0 || pICP == 0) {
        /* Освобождение интерфейсов в случае ошибки */
        goto Release;
    }

    /* Освобождение интерфейса */
    pICPC->pVTbl->Release(pICPC);

    printf("ConnectionPoint\n");

    result = createCEcoLab1Sink(pIMem, (IEcoLab1Events**)&pIEcoLab1Sink);

    if (pIEcoLab1Sink != 0) {
        result = pIEcoLab1Sink->pVTbl->QueryInterface(pIEcoLab1Sink, &IID_IEcoUnknown,(void **)&pISinkUnk);
        if (result != 0 || pISinkUnk == 0) {
            /* Освобождение интерфейсов в случае ошибки */
            goto Release;
        }
        /* Подключение */
        result = pICP->pVTbl->Advise(pICP, pISinkUnk, &cAdvise);
        /* Проверка */
        if (result == 0 && cAdvise == 1) {
            /* Сюда можно добавить код */
        }
        /* Освобождение интерфейса */
        pISinkUnk->pVTbl->Release(pISinkUnk);
    }

    printf("Sink\n");

    printf("Sort\n");
    testSort(pIEcoLab1, 10, pIMem);

    /* Освлбождение блока памяти */
    pIMem->pVTbl->Free(pIMem, name);

    if (pIEcoLab1Sink != 0) {
        /* Отключение */
        result = pICP->pVTbl->Unadvise(pICP, cAdvise);
        pIEcoLab1Sink->pVTbl->Release(pIEcoLab1Sink);
        pICP->pVTbl->Release(pICP);
    }

Release:

    /* Освобождение интерфейса для работы с интерфейсной шиной */
    if (pIBus != 0) {
        pIBus->pVTbl->Release(pIBus);
    }

    /* Освобождение интерфейса работы с памятью */
    if (pIMem != 0) {
        pIMem->pVTbl->Release(pIMem);
    }

    /* Освобождение тестируемого интерфейса */
    if (pIEcoLab1 != 0) {
        pIEcoLab1->pVTbl->Release(pIEcoLab1);
    }


    /* Освобождение системного интерфейса */
    if (pISys != 0) {
        pISys->pVTbl->Release(pISys);
    }

    return result;
}

