/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   CEcoLab1
 * </сводка>
 *
 * <описание>
 *   Данный исходный код описывает реализацию интерфейсов CEcoLab1
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2018 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */

#include "IEcoSystem1.h"
#include "IEcoInterfaceBus1.h"
#include "IEcoInterfaceBus1MemExt.h"
#include "CEcoLab1.h"
#include "IdEcoCalculatorD.h"
#include "IdEcoCalculatorE.h"
#include "IdEcoCalculatorA.h"
#include "IdEcoCalculatorB.h"

#include "CEcoLab1EnumConnectionPoints.h"
#include "IEcoConnectionPointContainer.h"
#include "IEcoLab1Events.h"


int IsEqualGUID(const UGUID* guid1, const UGUID* guid2) {
    return memcmp(guid1, guid2, sizeof(UGUID)) == 0;
}
/*
 *
 * <сводка>
 *   Функция QueryInterface
 * </сводка>
 *
 * <описание>
 *   Функция QueryInterface для интерфейса IEcoLab1
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1_QueryInterface(/* in */ struct IEcoLab1* me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoLab1* pCMe = (CEcoLab1*)me;

    /* Проверка указателей */
    if (me == 0 || ppv == 0) {
        return -1;
    }

    /* Проверка и получение запрошенного интерфейса */
    if ( IsEqualUGUID(riid, &IID_IEcoLab1) ) {
        *ppv = &pCMe->m_pVTblIEcoLab1;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
    else if (IsEqualGUID(riid, &IID_IEcoCalculatorY)) {
        *ppv = &pCMe->m_pVTblIY;
        ++pCMe->m_cRef;
    }
    else if (IsEqualGUID(riid, &IID_IEcoCalculatorX)) {
        if (pCMe->m_pInnerUnknownB == 0) {
            *ppv = &pCMe->m_pVTblIX;
        }
        else {
            pCMe->m_pInnerUnknownB->pVTbl->QueryInterface(pCMe->m_pInnerUnknownB, riid, ppv);
        }
        ++pCMe->m_cRef;
    }
    else if (IsEqualUGUID(riid, &IID_IEcoConnectionPointContainer) ) {
        *ppv = &pCMe->m_pVTblICPC;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoUnknown) ) {
        *ppv = &pCMe->m_pVTblIEcoLab1;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
    else {
        *ppv = 0;
        return -1;
    }
    return 0;
}

int16_t ECOCALLMETHOD CEcoLab1_Fire_OnMyCallback_ProcSort(/* in */ struct IEcoLab1* me, /* in */ char_t* Name, char *arr, size_t arrSize, size_t elemSize) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    int16_t result = 0;
    uint32_t count = 0;
    uint32_t index = 0;
    IEcoEnumConnections* pEnum = 0;
    IEcoLab1Events* pIEvents = 0;
    EcoConnectionData cd;

    if (me == 0 ) {
        return -1;
    }

    if (pCMe->m_pISinkCP != 0) {
        result = ((IEcoConnectionPoint*)pCMe->m_pISinkCP)->pVTbl->EnumConnections((IEcoConnectionPoint*)pCMe->m_pISinkCP, &pEnum);
        if ( (result == 0) && (pEnum != 0) ) {
            while (pEnum->pVTbl->Next(pEnum, 1, &cd, 0) == 0) {
                result = cd.pUnk->pVTbl->QueryInterface(cd.pUnk, &IID_IEcoLab1Events, (void**)&pIEvents);
                if ( (result == 0) && (pIEvents != 0) ) {
                    result = pIEvents->pVTbl->OnMyCallback_ProcSort(me, Name, arr, arrSize, elemSize);
                    pIEvents->pVTbl->Release(pIEvents);
                }
                cd.pUnk->pVTbl->Release(cd.pUnk);
            }
            pEnum->pVTbl->Release(pEnum);
        }
    }
    return result;
}

int16_t ECOCALLMETHOD CEcoLab1_Fire_OnMyCallback_EndSort(/* in */ struct IEcoLab1* me, /* in */ char_t* Name, char *arr, size_t arrSize, size_t elemSize, FILE *resultFile) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    int16_t result = 0;
    uint32_t count = 0;
    uint32_t index = 0;
    IEcoEnumConnections* pEnum = 0;
    IEcoLab1Events* pIEvents = 0;
    EcoConnectionData cd;

    if (me == 0 ) {
        return -1;
    }

    if (pCMe->m_pISinkCP != 0) {
        result = ((IEcoConnectionPoint*)pCMe->m_pISinkCP)->pVTbl->EnumConnections((IEcoConnectionPoint*)pCMe->m_pISinkCP, &pEnum);
        if ( (result == 0) && (pEnum != 0) ) {
            while (pEnum->pVTbl->Next(pEnum, 1, &cd, 0) == 0) {
                result = cd.pUnk->pVTbl->QueryInterface(cd.pUnk, &IID_IEcoLab1Events, (void**)&pIEvents);
                if ( (result == 0) && (pIEvents != 0) ) {
                    result = pIEvents->pVTbl->OnMyCallback_EndSort(pIEvents, Name, arr, arrSize, elemSize);
                    pIEvents->pVTbl->Release(pIEvents);
                }
                cd.pUnk->pVTbl->Release(cd.pUnk);
            }
            pEnum->pVTbl->Release(pEnum);
        }
    }
    return result;
}


int16_t ECOCALLMETHOD CEcoLab1_Fire_OnMyCallback_StartSort(/* in */ struct IEcoLab1* me, /* in */ char_t* Name, char *arr, size_t arrSize, size_t elemSize, FILE *resultFile) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    int16_t result = 0;
    uint32_t count = 0;
    uint32_t index = 0;
    IEcoEnumConnections* pEnum = 0;
    IEcoLab1Events* pIEvents = 0;
    EcoConnectionData cd;

    if (me == 0 ) {
        return -1;
    }

    if (pCMe->m_pISinkCP != 0) {
        result = ((IEcoConnectionPoint*)pCMe->m_pISinkCP)->pVTbl->EnumConnections((IEcoConnectionPoint*)pCMe->m_pISinkCP, &pEnum);
        if ( (result == 0) && (pEnum != 0) ) {
            while (pEnum->pVTbl->Next(pEnum, 1, &cd, 0) == 0) {
                result = cd.pUnk->pVTbl->QueryInterface(cd.pUnk, &IID_IEcoLab1Events, (void**)&pIEvents);
                if ( (result == 0) && (pIEvents != 0) ) {
                    result = pIEvents->pVTbl->OnMyCallback_StartSort(pIEvents, Name, arr, arrSize, elemSize, resultFile);
                    pIEvents->pVTbl->Release(pIEvents);
                }
                cd.pUnk->pVTbl->Release(cd.pUnk);
            }
            pEnum->pVTbl->Release(pEnum);
        }
    }
    return result;
}

/*
 *
 * <сводка>
 *   Функция AddRef
 * </сводка>
 *
 * <описание>
 *   Функция AddRef для интерфейса IEcoLab1
 * </описание>
 *
 */
uint32_t ECOCALLMETHOD CEcoLab1_AddRef(/* in */ struct IEcoLab1* me) {
    CEcoLab1* pCMe = (CEcoLab1*)me;

    /* Проверка указателя */
    if (me == 0 ) {
        return -1;
    }

    return ++pCMe->m_cRef;
}

/*
 *
 * <сводка>
 *   Функция Release
 * </сводка>
 *
 * <описание>
 *   Функция Release для интерфейса IEcoLab1
 * </описание>
 *
 */
uint32_t ECOCALLMETHOD CEcoLab1_Release(/* in */ struct IEcoLab1* me) {
    CEcoLab1* pCMe = (CEcoLab1*)me;

    /* Проверка указателя */
    if (me == 0 ) {
        return -1;
    }

    /* Уменьшение счетчика ссылок на компонент */
    --pCMe->m_cRef;

    /* В случае обнуления счетчика, освобождение данных экземпляра */
    if ( pCMe->m_cRef == 0 ) {
        deleteCEcoLab1((IEcoLab1*)pCMe);
        return 0;
    }
    return pCMe->m_cRef;
}

/*X*/
int16_t ECOCALLMETHOD IEcoCalculatorX_QueryInterface (/* in */ IEcoCalculatorX* me, /* in */ const UGUID* riid, /* out */ void **ppv) {
    CEcoLab1* pCMe = (CEcoLab1 *)((uint64_t)me - sizeof(struct IEcoLab1*) - sizeof(struct IEcoCalculatorY*));

    if (me == 0 || ppv == 0) {
        return -1;
    }

    IEcoUnknown* cast = (IEcoUnknown *)&pCMe->m_pVTblIEcoLab1;
    return cast->pVTbl->QueryInterface(cast, riid, ppv);
}

uint32_t ECOCALLMETHOD IEcoCalculatorX_AddRef (/* in */ IEcoCalculatorX* me) {
    CEcoLab1* pCMe = (CEcoLab1 *)((uint64_t)me - sizeof(struct IEcoLab1*) - sizeof(struct IEcoCalculatorY*));

    if (me == 0) {
        return -1;
    }

    IEcoUnknown* cast = (IEcoUnknown *)&pCMe->m_pVTblIEcoLab1;
    return cast->pVTbl->AddRef(cast);
}

uint32_t ECOCALLMETHOD IEcoCalculatorX_Release (/* in */ IEcoCalculatorX* me) {
    CEcoLab1* pCMe = (CEcoLab1 *)((uint64_t)me - sizeof(struct IEcoLab1*) - sizeof(struct IEcoCalculatorY*));

    if (me == 0) {
        return -1;
    }

    IEcoUnknown* cast = (IEcoUnknown *)&pCMe->m_pVTblIEcoLab1;
    return cast->pVTbl->Release(cast);
}

int32_t ECOCALLMETHOD IEcoCalculatorX_Addition (/* in */ IEcoCalculatorX* me, /* in */  int16_t a, /* in */ int16_t b) {
    CEcoLab1* pCMe = (CEcoLab1 *)((uint64_t)me - sizeof(struct IEcoLab1*) - sizeof(struct IEcoCalculatorY*));

    if (me == 0) {
        return -1;
    }

    IEcoUnknown* cast = (IEcoUnknown *)&pCMe->m_pVTblIEcoLab1;
    return pCMe->m_pIX->pVTbl->Addition(pCMe->m_pIX, a, b);
}

int16_t ECOCALLMETHOD IEcoCalculatorX_Subtraction (/* in */ IEcoCalculatorX* me, /* in */ int16_t a, /* in */ int16_t b) {
    CEcoLab1* pCMe = (CEcoLab1 *)((uint64_t)me - sizeof(struct IEcoLab1*) - sizeof(struct IEcoCalculatorY*));

    if (me == 0) {
        return -1;
    }

    return pCMe->m_pIX->pVTbl->Subtraction(pCMe->m_pIX, a, b);
}

/*Y*/
int16_t ECOCALLMETHOD IEcoCalculatorY_QueryInterface (/* in */IEcoCalculatorY* me, /* in */ const UGUID* riid, /* out */ void **ppv) {
    CEcoLab1* pCMe = (CEcoLab1 *)((uint64_t)me - sizeof(struct IEcoLab1*));

    if (me == 0 || ppv == 0) {
        return -1;
    }

    IEcoUnknown* cast = (IEcoUnknown *)&pCMe->m_pVTblIEcoLab1;
    return cast->pVTbl->QueryInterface(cast, riid, ppv);
}


uint32_t ECOCALLMETHOD IEcoCalculatorY_AddRef (/* in */ IEcoCalculatorY* me) {
    CEcoLab1* pCMe = (CEcoLab1 *)((uint64_t)me - sizeof(struct IEcoLab1*));

    if (me == 0) {
        return -1;
    }
    IEcoUnknown* cast = (IEcoUnknown *)&pCMe->m_pVTblIEcoLab1;
    return cast->pVTbl->AddRef(cast);
}

uint32_t ECOCALLMETHOD IEcoCalculatorY_Release (/* in */ IEcoCalculatorY* me) {
    CEcoLab1* pCMe = (CEcoLab1 *)((uint64_t)me - sizeof(struct IEcoLab1*));

    if (me == 0) {
        return -1;
    }

    IEcoUnknown* cast = (IEcoUnknown *)&pCMe->m_pVTblIEcoLab1;
    return cast->pVTbl->Release(cast);
}

int32_t ECOCALLMETHOD IEcoCalculatorY_Multiplication (/* in */ IEcoCalculatorY* me, /* in */ int16_t a, /* in */ int16_t b) {
    CEcoLab1* pCMe = (CEcoLab1 *)((uint64_t)me - sizeof(struct IEcoLab1*));

    if (me == 0) {
        return -1;
    }

    return pCMe->m_pIY->pVTbl->Multiplication(pCMe->m_pIY, a, b);
}

int16_t ECOCALLMETHOD IEcoCalculatorY_Division (/* in */ IEcoCalculatorY* me, /* in */ int16_t a, /* in */ int16_t b) {
    CEcoLab1* pCMe = (CEcoLab1 *)((uint64_t)me - sizeof(struct IEcoLab1*));

    if (me == 0) {
        return -1;
    }

    return pCMe->m_pIY->pVTbl->Division(pCMe->m_pIY, a, b);
}


// Копирует заданное число байт
void copy_bytes(char *src, char *dst, size_t count) {
    char *end = src + count;
    while (src < end) {
        *(dst++) = *(src++);
    }
}

// Структура для узла бинарного дерева
struct Node {
    char *data;
    struct Node *left, *right;
};

// Функция для создания нового узла дерева
struct Node* newNode(struct CEcoLab1* pCMe, char *item, size_t elem_size) {
    struct Node* temp = pCMe->m_pIMem->pVTbl->Alloc(pCMe->m_pIMem, sizeof(struct Node));
    temp->data = pCMe->m_pIMem->pVTbl->Alloc(pCMe->m_pIMem, elem_size);
    copy_bytes(item, temp->data, elem_size);
    temp->left = temp->right = NULL;
    return temp;
}

// Функция для вставки нового узла в бинарное дерево
struct Node* insert(struct CEcoLab1* pCMe, struct Node* node, char *key, size_t elem_size, int (*comp)(const void *, const void *)) {
    if (node == NULL) return newNode(pCMe, key, elem_size);

    if (comp(key, node->data) < 0)
        node->left = insert(pCMe, node->left, key, elem_size, comp);
    else
        node->right = insert(pCMe, node->right, key, elem_size, comp);

    return node;
}

// Функция для обхода дерева в порядке возрастания
void inOrder(struct Node* root, char **arr, size_t elem_size, CEcoLab1* pCMe, size_t elem_count, struct IEcoLab1* me, char *array) {
    CEcoLab1_Fire_OnMyCallback_ProcSort(me, pCMe->m_Name, array, elem_count, elem_size);
    if (root != NULL) {
        inOrder(root->left, arr, elem_size, pCMe, elem_count, me, array);
        copy_bytes(root->data, *arr, elem_size);
        *arr += elem_size;
        inOrder(root->right, arr, elem_size, pCMe, elem_count, me, array);
    }
}

void freeTree(struct Node* node, CEcoLab1* pCMe) {
    if (node == NULL) return;

    freeTree(node->left, pCMe);
    freeTree(node->right, pCMe);

    pCMe->m_pIMem->pVTbl->Free(pCMe->m_pIMem, node);
}

// Функция для сортировки массива с использованием Tree Sort
void treeSort(struct CEcoLab1* pCMe, char *arr, size_t n, size_t elem_size, int (*comp)(const void *, const void *), struct IEcoLab1 *me) {
    struct Node *root = NULL;

    // Вставляем все элементы массива в бинарное дерево
    for (int i = 0; i < n; i++) {
        root = insert(pCMe, root, (char*)arr + i * elem_size, elem_size, comp);
    }

    // Обходим дерево в порядке возрастания и записываем отсортированные элементы обратно в массив
    inOrder(root, &arr, elem_size, pCMe, n, me, arr);
    
    freeTree(root, pCMe);
}

/*
 *
 * <сводка>
 *   Функция TreeSort реализует сортировку двоичным деревом
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1_qsort(
                                        struct IEcoLab1* me,
                                        void* pData,
                                        size_t elem_count,
                                        size_t elem_size,
                                        int (__cdecl *compare)(const void *, const void*)) {
    CEcoLab1* pCMe = (CEcoLab1*)me;

    /* Проверка указателей */
    if (me == 0 || pData == 0 || compare == 0) {
        return -1;
    }
    
    FILE * resultFile;
    resultFile = fopen("output.csv", "w");
    
    /* callback*/
    CEcoLab1_Fire_OnMyCallback_StartSort(me, pCMe->m_Name, pData, elem_count, elem_size, resultFile);
        
    // Tree sort
    treeSort(pCMe, pData, elem_count, elem_size, compare, me);
    
    CEcoLab1_Fire_OnMyCallback_EndSort(me, pCMe->m_Name, pData, elem_count, elem_size, resultFile);
    
    fclose(resultFile);
    
    return 0;
}

int16_t ECOCALLMETHOD CEcoLab1_IEcoConnectionPointContainer_QueryInterface(/* in */ struct IEcoConnectionPointContainer* me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoLab1* pCMe = (CEcoLab1*)((uint64_t)me - sizeof(struct IEcoLab1*) - sizeof(struct IEcoCalculatorY*) -sizeof(struct IEcoCalculatorX *));
    if (me == 0 || ppv == 0) {
        return -1;
    }

    /* Проверка и получение запрошенного интерфейса */
    if ( IsEqualUGUID(riid, &IID_IEcoLab1) ) {
        *ppv = &pCMe->m_pVTblIEcoLab1;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoConnectionPointContainer) ) {
        *ppv = &pCMe->m_pVTblICPC;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoUnknown) ) {
        *ppv = &pCMe->m_pVTblIEcoLab1;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
    else {
        *ppv = 0;
        return -1;
    }

    return 0;
}

/*
 *
 * <сводка>
 *   Функция AddRef
 * </сводка>
 *
 * <описание>
 *   Функция AddRef для интерфейса IEcoConnectionPointContainer
 * </описание>
 *
 */
uint32_t ECOCALLMETHOD CEcoLab1_IEcoConnectionPointContainer_AddRef(/* in */ struct IEcoConnectionPointContainer* me) {
    CEcoLab1* pCMe = (CEcoLab1*)((uint64_t)me - sizeof(struct IEcoLab1*) - sizeof(struct IEcoCalculatorY*) -sizeof(struct IEcoCalculatorX *));
    if (me == 0 ) {
        return -1;
    }

    return ++pCMe->m_cRef;
}

/*
 *
 * <сводка>
 *   Функция Release
 * </сводка>
 *
 * <описание>
 *   Функция Release для интерфейса IEcoConnectionPointContainer
 * </описание>
 *
 */
uint32_t ECOCALLMETHOD CEcoLab1_IEcoConnectionPointContainer_Release(/* in */ struct IEcoConnectionPointContainer* me) {
    CEcoLab1* pCMe = (CEcoLab1*)((uint64_t)me - sizeof(struct IEcoLab1*) - sizeof(struct IEcoCalculatorY*) -sizeof(struct IEcoCalculatorX *));

    if (me == 0 ) {
        return -1;
    }

    /* Уменьшение счетчика ссылок на компонент */
    --pCMe->m_cRef;

    /* В случае обнуления счетчика, освобождение данных экземпляра */
    if ( pCMe->m_cRef == 0 ) {
        deleteCEcoLab1((IEcoLab1*)pCMe);
        return 0;
    }
    return pCMe->m_cRef;
}

/*
 *
 * <сводка>
 *   Функция EnumConnectionPoints
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1_IEcoConnectionPointContainer_EnumConnectionPoints(/* in */ struct IEcoConnectionPointContainer* me, /* out */ struct IEcoEnumConnectionPoints **ppEnum) {
    CEcoLab1* pCMe = (CEcoLab1*)((uint64_t)me - sizeof(struct IEcoLab1*) - sizeof(struct IEcoCalculatorY*) -sizeof(struct IEcoCalculatorX *));
    int16_t result = 0;

    if (me == 0 || ppEnum == 0 ) {
        return -1;
    }

    result = createCEcoLab1EnumConnectionPoints((IEcoUnknown*)pCMe->m_pISys,
                                                (struct IEcoConnectionPoint *) &pCMe->m_pISinkCP->m_pVTblICP, ppEnum);

    return result;
}

/*
 *
 * <сводка>
 *   Функция FindConnectionPoint
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1_IEcoConnectionPointContainer_FindConnectionPoint(/* in */ struct IEcoConnectionPointContainer* me, /* in */ const UGUID* riid, /* out */ struct IEcoConnectionPoint **ppCP) {
    CEcoLab1* pCMe = (CEcoLab1*)((uint64_t)me - sizeof(struct IEcoLab1*) - sizeof(struct IEcoCalculatorY*) -sizeof(struct IEcoCalculatorX *));
    int16_t result = 0;

    if (me == 0 || ppCP == 0) {
        return -1;
    }

    if (!IsEqualUGUID(riid, &IID_IEcoLab1Events)) {
        *ppCP = 0;
        /* CONNECT_E_NOCONNECTION */
        return -1;
    }

    if (pCMe->m_pISinkCP == 0) {
        /* E_FAIL */
        return -1;
    }

    pCMe->m_pISinkCP->m_pVTblICP->AddRef((struct IEcoConnectionPoint *) &pCMe->m_pISinkCP->m_pVTblICP);
    *ppCP = (struct IEcoConnectionPoint *) &pCMe->m_pISinkCP->m_pVTblICP;

    return 0;
}

/*
 *
 * <сводка>
 *   Функция Init
 * </сводка>
 *
 * <описание>
 *   Функция инициализации экземпляра
 * </описание>
 *
 */
int16_t ECOCALLMETHOD initCEcoLab1(/*in*/ struct IEcoLab1* me, /* in */ struct IEcoUnknown *pIUnkSystem) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    IEcoInterfaceBus1* pIBus = 0;
    IEcoUnknown* pOuterUnknown = (IEcoUnknown*)me;
    int16_t result = -1;

    /* Проверка указателей */
    if (me == 0 ) {
        return result;
    }

    /* Сохранение указателя на системный интерфейс */
    pCMe->m_pISys = (IEcoSystem1*)pIUnkSystem;

    /* Получение интерфейса для работы с интерфейсной шиной */
    result = pCMe->m_pISys->pVTbl->QueryInterface(pCMe->m_pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);

    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorE, 0, &IID_IEcoCalculatorY, (void**) &pCMe->m_pIY);

    if (result != 0 || pCMe->m_pIY == 0) {
        result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorD, 0, &IID_IEcoCalculatorY,(void **) &pCMe->m_pIY);
    }

    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorB, pOuterUnknown, &IID_IEcoUnknown,(void**) &pCMe->m_pInnerUnknownB);

    if (result != 0 || pCMe->m_pInnerUnknownB == 0) {
        result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorA, 0, &IID_IEcoCalculatorX, (void **) &pCMe->m_pIX);
    }

    result = createCEcoLab1ConnectionPoint((IEcoUnknown*)pCMe->m_pISys,
                                           (IEcoConnectionPointContainer *) &pCMe->m_pVTblICPC, &IID_IEcoLab1Events, (IEcoConnectionPoint**)&(pCMe->m_pISinkCP));
    if (result == 0 && pCMe->m_pISinkCP != 0) {
        result = 0;
    }

    /* Освобождение */
    pIBus->pVTbl->Release(pIBus);
    
    return result;
}

/* Create Virtual Table IEcoLab1 */
IEcoLab1VTbl g_x277FC00C35624096AFCFC125B94EEC90VTbl = {
        CEcoLab1_QueryInterface,
        CEcoLab1_AddRef,
        CEcoLab1_Release,
        CEcoLab1_qsort
};

IEcoCalculatorYVTbl g_xBD6414C29096423EA90C04D77AFD1CADVTblLab2 = {
        IEcoCalculatorY_QueryInterface,
        IEcoCalculatorY_AddRef,
        IEcoCalculatorY_Release,
        IEcoCalculatorY_Multiplication,
        IEcoCalculatorY_Division
};

IEcoCalculatorXVTbl g_x9322111622484742AE0682819447843DVTblLab2 = {
        IEcoCalculatorX_QueryInterface,
        IEcoCalculatorX_AddRef,
        IEcoCalculatorX_Release,
        IEcoCalculatorX_Addition,
        IEcoCalculatorX_Subtraction
};

IEcoConnectionPointContainerVTbl g_x0000000500000000C000000000000046VTblCPC = {
        CEcoLab1_IEcoConnectionPointContainer_QueryInterface,
        CEcoLab1_IEcoConnectionPointContainer_AddRef,
        CEcoLab1_IEcoConnectionPointContainer_Release,
        CEcoLab1_IEcoConnectionPointContainer_EnumConnectionPoints,
        CEcoLab1_IEcoConnectionPointContainer_FindConnectionPoint
};

/*
 *
 * <сводка>
 *   Функция Create
 * </сводка>
 *
 * <описание>
 *   Функция создания экземпляра
 * </описание>
 *
 */
int16_t ECOCALLMETHOD createCEcoLab1(/* in */ IEcoUnknown* pIUnkSystem, /* in */ IEcoUnknown* pIUnkOuter, /* out */ IEcoLab1** ppIEcoLab1) {
    int16_t result = -1;
    IEcoSystem1* pISys = 0;
    IEcoInterfaceBus1* pIBus = 0;
    IEcoInterfaceBus1MemExt* pIMemExt = 0;
    IEcoMemoryAllocator1* pIMem = 0;
    CEcoLab1* pCMe = 0;
    UGUID* rcid = (UGUID*)&CID_EcoMemoryManager1;

    /* Проверка указателей */
    if (ppIEcoLab1 == 0 || pIUnkSystem == 0) {
        return result;
    }

    /* Получение системного интерфейса приложения */
    result = pIUnkSystem->pVTbl->QueryInterface(pIUnkSystem, &GID_IEcoSystem1, (void **)&pISys);

    /* Проверка */
    if (result != 0 && pISys == 0) {
        return result;
    }

    /* Получение интерфейса для работы с интерфейсной шиной */
    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);

    /* Получение идентификатора компонента для работы с памятью */
    result = pIBus->pVTbl->QueryInterface(pIBus, &IID_IEcoInterfaceBus1MemExt, (void**)&pIMemExt);
    if (result == 0 && pIMemExt != 0) {
        rcid = (UGUID*)pIMemExt->pVTbl->get_Manager(pIMemExt);
        pIMemExt->pVTbl->Release(pIMemExt);
    }

    /* Получение интерфейса распределителя памяти */
    pIBus->pVTbl->QueryComponent(pIBus, rcid, 0, &IID_IEcoMemoryAllocator1, (void**) &pIMem);

    /* Проверка */
    if (result != 0 && pIMem == 0) {
        /* Освобождение системного интерфейса в случае ошибки */
        pISys->pVTbl->Release(pISys);
        return result;
    }

    /* Выделение памяти для данных экземпляра */
    pCMe = (CEcoLab1*)pIMem->pVTbl->Alloc(pIMem, sizeof(CEcoLab1));

    /* Сохранение указателя на системный интерфейс */
    pCMe->m_pISys = pISys;

    /* Сохранение указателя на интерфейс для работы с памятью */
    pCMe->m_pIMem = pIMem;

    /* Установка счетчика ссылок на компонент */
    pCMe->m_cRef = 1;

    /* Создание таблицы функций интерфейса IEcoLab1 */
    pCMe->m_pVTblIEcoLab1 = &g_x277FC00C35624096AFCFC125B94EEC90VTbl;

    pCMe->m_pVTblIY = &g_xBD6414C29096423EA90C04D77AFD1CADVTblLab2;
    pCMe->m_pVTblIX = &g_x9322111622484742AE0682819447843DVTblLab2;

    pCMe->m_pVTblICPC = &g_x0000000500000000C000000000000046VTblCPC;

    /* Инициализация данных */
    pCMe->m_Name = 0;

    /* Возврат указателя на интерфейс */
    *ppIEcoLab1 = (IEcoLab1*)pCMe;

    /* Освобождение */
    pIBus->pVTbl->Release(pIBus);

    return 0;
}

/*
 *
 * <сводка>
 *   Функция Delete
 * </сводка>
 *
 * <описание>
 *   Функция освобождения экземпляра
 * </описание>
 *
 */
void ECOCALLMETHOD deleteCEcoLab1(/* in */ IEcoLab1* pIEcoLab1) {
    CEcoLab1* pCMe = (CEcoLab1*)pIEcoLab1;
    IEcoMemoryAllocator1* pIMem = 0;

    if (pIEcoLab1 != 0 ) {
        pIMem = pCMe->m_pIMem;
        /* Освобождение */
        if ( pCMe->m_Name != 0 ) {
            pIMem->pVTbl->Free(pIMem, pCMe->m_Name);
        }
        if ( pCMe->m_pISys != 0 ) {
            pCMe->m_pISys->pVTbl->Release(pCMe->m_pISys);
        }
        pIMem->pVTbl->Free(pIMem, pCMe);
        pIMem->pVTbl->Release(pIMem);
    }
}

