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

void fillArrayWithRandomInts(int *arr, int arrSize) {
    for (size_t i = 0; i < arrSize; i ++) {
        arr[i] = rand() % 10;
    }
}

void *createFloatArray(IEcoMemoryAllocator1 *pIMem, size_t size) {
    size_t i;
    float *arr = (float *) pIMem->pVTbl->Alloc(pIMem, size * sizeof(float));
    for (i = 0; i < size; i++) {
        arr[i] = ((float) (rand() %20003 - 10000)) / ((float)(rand() % 1000 + 1)) ;
    }
    return arr;
}

void fillArrayWithRandomDoubles(int *arr, int arrSize) {
    for (int i = 0; i < arrSize; ++i) {
        arr[i] = (double)rand() + ((double)rand() / RAND_MAX);
    }
}

void *createStringArray(IEcoMemoryAllocator1 *pIMem, size_t size) {
    size_t i, j, str_size;
    char **arr = (char **) pIMem->pVTbl->Alloc(pIMem, size * sizeof(char *));
    for (i = 0; i < size; i++) {
        arr[i] = (char *) pIMem->pVTbl->Alloc(pIMem, 20 * sizeof(char));
        str_size = rand() % 14 + 4;
        for (j = 0; j < str_size; ++j) {
            arr[i][j] = (char) (rand() % ('z' - 'a' + 1) + 'a');
        }
        arr[i][str_size] = 0;
    }
    return arr;
}


// Функции удаления массивов

void deleteArray(IEcoMemoryAllocator1 *pIMem, void *arr_ptr, size_t size) {
    pIMem->pVTbl->Free(pIMem, arr_ptr);
}

void deleteStringArray(IEcoMemoryAllocator1 *pIMem, void *arr_ptr, size_t size) {
    size_t i;
    char **arr = (char **) arr_ptr;
    for (i = 0; i < size; ++i) {
        pIMem->pVTbl->Free(pIMem, arr[i]);
    }
    pIMem->pVTbl->Free(pIMem, arr_ptr);
}

// Функции копирования

void *createCopyArray(IEcoMemoryAllocator1 *pIMem, void *src, uint32_t byte_count) {
    void *copy_array = pIMem->pVTbl->Alloc(pIMem, byte_count);
    pIMem->pVTbl->Copy(pIMem, copy_array, src, byte_count);
    return copy_array;
}

void *createCopyStringArray(IEcoMemoryAllocator1 *pIMem, void *src, uint32_t byte_count) {
    char **src_arr = (char **) src;
    char **copy_array = pIMem->pVTbl->Alloc(pIMem, byte_count);
    size_t i, size = byte_count / sizeof(char *);
    for (i = 0; i < size; i++) {
        copy_array[i] = (char *) pIMem->pVTbl->Alloc(pIMem, 20 * sizeof(char));
        pIMem->pVTbl->Copy(pIMem, copy_array[i], src_arr[i], 20);
    }
    return copy_array;
}

// функции вывода массивов на экран

void printIntArray(void *array, size_t size) {
    int *arr = (int *) array;
    size_t i;
    for (i = 0; i < size; ++i) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void printFloatArray(void *array, size_t size) {
    float *arr = (float *) array;
    size_t i;
    for (i = 0; i < size; ++i) {
        printf("%f ", arr[i]);
    }
    printf("\n");
}

void printDoubleArray(void *array, size_t size) {
    double *arr = (double *) array;
    size_t i;
    for (i = 0; i < size; ++i) {
        printf("%lf ", arr[i]);
    }
    printf("\n");
}

void printStringArray(void *array, size_t size) {
    char **arr = (char **) array;
    size_t i;
    for (i = 0; i < size; ++i) {
        printf("'%s' ", arr[i]);
    }
    printf("\n");
}

// Структура для хранения сортировок над разными типами данных

typedef struct sorting {
    void *(*createArray)(IEcoMemoryAllocator1 *pIMem, size_t size);
    int (__cdecl *comp)(const void *a_ptr, const void *b_ptr);
    void (*deleteArray)(IEcoMemoryAllocator1 *pIMem, void *arr_ptr, size_t size);
    void *(*createCopy)(IEcoMemoryAllocator1 *pIMem, void *src, uint32_t byte_count);
    void (*printArray)(void *array_ptr, size_t size);
    size_t elem_size;
    const char *type_name;
} Sorting;

// Сравнение времени работы сортировок

typedef struct sort_result {
    double my_sort;
    double q_sort;
    double my_sort2;
} SortResult;


SortResult testSorting(IEcoMemoryAllocator1 *pIMem, Sorting *sorting, size_t size, IEcoLab1 *lab1rec, IEcoLab1 *lab1iter) {
    uint32_t byte_count = size * sorting->elem_size;
    void *array = sorting->createArray(pIMem, size);
    void *copy_array = sorting->createCopy(pIMem, array, byte_count);
    void *copy2_array = sorting->createCopy(pIMem, array, byte_count);
    clock_t before, after;
    double lab_sort_rec, standard_sort, lab_sort_iter;
    SortResult result;

    before = clock();
    lab1rec->pVTbl->qsort(lab1rec, array, size, sorting->elem_size, sorting->comp);
    after = clock();
    lab_sort_rec = (double)(after - before) / CLOCKS_PER_SEC;
    sorting->deleteArray(pIMem, array, size);

    before = clock();
    qsort(copy_array, size, sorting->elem_size, sorting->comp);
    after = clock();
    standard_sort = (double)(after - before) / CLOCKS_PER_SEC;
    sorting->deleteArray(pIMem, copy_array, size);

    before = clock();
    lab1iter->pVTbl->qsort(lab1iter, copy2_array, size, sorting->elem_size, sorting->comp);
    after = clock();
    lab_sort_iter = (double)(after - before) / CLOCKS_PER_SEC;
    sorting->deleteArray(pIMem, copy2_array, size);


    result.my_sort = lab_sort_rec;
    result.my_sort2 = lab_sort_iter;
    result.q_sort = standard_sort;

    return result;
}

// Вывод сортируемых массивов на экран до и после сортировок

void showSorting(IEcoMemoryAllocator1 *pIMem, Sorting *sorting, IEcoLab1 *lab1Rec, IEcoLab1 *lab1Iter, size_t size) {
    void *array_ptr = sorting->createArray(pIMem, size);
    void *copy_array = sorting->createCopy(pIMem, array_ptr, size * sorting->elem_size);
    void *copy2_array = sorting->createCopy(pIMem, array_ptr, size * sorting->elem_size);

    printf("testing sort for type: %s\n", sorting->type_name);
    printf("array and copies before sort:\n");
    sorting->printArray(array_ptr, size);
    sorting->printArray(copy_array, size);
    sorting->printArray(copy2_array, size);

    printf("size = %u\n", size);
    printf(lab1Rec->pVTbl->qsort(lab1Rec, array_ptr, size, sorting->elem_size, sorting->comp));
    lab1Rec->pVTbl->qsort(lab1Rec, array_ptr, size, sorting->elem_size, sorting->comp);
    lab1Iter->pVTbl->qsort(lab1Iter, copy2_array, size, sorting->elem_size, sorting->comp);
    qsort(copy_array, size, sorting->elem_size, sorting->comp);

    printf("array after merge sort(recursive), copy after qsort and another copy after merge sort(iterative):\n");
    sorting->printArray(array_ptr, size);
    sorting->printArray(copy_array, size);
    sorting->printArray(copy2_array, size);
    printf("\n");

    sorting->deleteArray(pIMem, array_ptr, size);
    sorting->deleteArray(pIMem, copy_array, size);
    sorting->deleteArray(pIMem, copy2_array, size);
}

void testAndWriteToFile(FILE *file, IEcoMemoryAllocator1 *pIMem, IEcoLab1 *lab1rec, IEcoLab1 *lab1iter, Sorting *sorting, size_t size) {
    SortResult result = testSorting(pIMem, sorting, size, lab1rec, lab1iter);
    printf("time test:\ttype=%s\tsize=%d done.\n", sorting->type_name, size);
    fprintf(file, "%s,%s,%d,%lf\n", "merge_sort_rec", sorting->type_name, size, result.my_sort);
    fprintf(file, "%s,%s,%d,%lf\n", "merge_sort_iter", sorting->type_name, size, result.my_sort2);
    fprintf(file, "%s,%s,%d,%lf\n", "qsort", sorting->type_name, size, result.q_sort);
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

void testIntSort(IEcoLab1 *pIEcoLab1, FILE *file, int arrSize, IEcoMemoryAllocator1 *pIMem) {
    clock_t start, end;
    int* arr;

    //test
    int arrSizeForTest = arrSize;
    arr = (int *) pIMem->pVTbl->Alloc(pIMem, arrSizeForTest * sizeof(int));
    fillArrayWithRandomInts(arr, arrSizeForTest);
    pIEcoLab1->pVTbl->qsort(pIEcoLab1, arr, arrSizeForTest, sizeof(int), compareInt);
    for (size_t i = 0; i < arrSizeForTest - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            printf("insertionSort doesn't work for int.\n");
            return;
        }
    }
    pIMem->pVTbl->Free(pIMem, arr);

    //insertionSort
    arr = (int *) pIMem->pVTbl->Alloc(pIMem, arrSize * sizeof(int));
    fillArrayWithRandomInts(arr, arrSize);
    start = clock();

    pIEcoLab1->pVTbl->qsort(pIEcoLab1, arr, arrSize, sizeof(int), compareInt);

    end = clock();
    double insertionSortDuration = (double)(end - start) / CLOCKS_PER_SEC;
    pIMem->pVTbl->Free(pIMem, arr);

    //qsort
    arr = (int *) pIMem->pVTbl->Alloc(pIMem, arrSize * sizeof(int));
    fillArrayWithRandomInts(arr, arrSize);
    start = clock();

    qsort(arr, arrSize, sizeof(int), compareInt);

    end = clock();
    double qsortDuration = (double)(end - start) / CLOCKS_PER_SEC;
    pIMem->pVTbl->Free(pIMem, arr);

    fprintf(file, "%s,%s,%d,%lf\n", "insertSort", "int", arrSize, insertionSortDuration);
    fprintf(file, "%s,%s,%d,%lf\n", "qsort", "int", arrSize, qsortDuration);
}

void testDoubleSort(IEcoLab1 *pIEcoLab1, FILE *file, int arrSize, IEcoMemoryAllocator1 *pIMem) {
    clock_t start, end;
    double* arr;

    //test
    int arrSizeForTest = 10;
    arr = (double *) pIMem->pVTbl->Alloc(pIMem, arrSizeForTest * sizeof(double));
    fillArrayWithRandomDoubles(arr, arrSizeForTest);
    pIEcoLab1->pVTbl->qsort(pIEcoLab1, arr, arrSizeForTest, sizeof(double), compareDouble);
    for (size_t i = 0; i < arrSizeForTest - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            printf("insertionSort doesn't work for double.\n");
            return;
        }
    }
    pIMem->pVTbl->Free(pIMem, arr);

    //insertionSort
    arr = (double *) pIMem->pVTbl->Alloc(pIMem, arrSize * sizeof(double));
    fillArrayWithRandomDoubles(arr, arrSize);
    start = clock();

    pIEcoLab1->pVTbl->qsort(pIEcoLab1, arr, arrSize, sizeof(double), compareDouble);

    end = clock();
    double insertionSortDuration = (double)(end - start) / CLOCKS_PER_SEC;
    pIMem->pVTbl->Free(pIMem, arr);

    //qsort
    arr = (double *) pIMem->pVTbl->Alloc(pIMem, arrSize * sizeof(double ));
    fillArrayWithRandomDoubles(arr, arrSize);
    start = clock();

    qsort(arr, arrSize, sizeof(double), compareDouble);

    end = clock();
    double qsortDuration = (double)(end - start) / CLOCKS_PER_SEC;
    pIMem->pVTbl->Free(pIMem, arr);

    fprintf(file, "%s,%s,%d,%lf\n", "insertSort", "double", arrSize, insertionSortDuration);
    fprintf(file, "%s,%s,%d,%lf\n", "qsort", "double", arrSize, qsortDuration);
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

    int arrSizes[5] = {1000, 5000, 10000, 15000, 20000};


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

    FILE * resultFile;
    srand(time(0));
    resultFile = fopen("output.csv", "w");
    fprintf(resultFile, "sort,type,size,time\n");
    for (size_t i = 0; i < 5; i++) {
        testIntSort(pIEcoLab1, resultFile, arrSizes[i], pIMem);
        testDoubleSort(pIEcoLab1, resultFile, arrSizes[i], pIMem);
//        testFloatSort(pIEcoLab1, resultFile, arrSizes[i], pIMem);
//        testStringSort(pIEcoLab1, resultFile, arrSizes[i], pIMem);
    }

    /* Освлбождение блока памяти */
    pIMem->pVTbl->Free(pIMem, name);

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

