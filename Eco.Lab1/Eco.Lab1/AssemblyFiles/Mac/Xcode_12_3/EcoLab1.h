//
//  EcoLab1.h
//  Eco.Lab1UnitTest
//
//  Created by Elina Karapetian on 06.05.2024.
//

#ifndef ECOLAB1_H
#define ECOLAB1_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Eco OS
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

// Function prototypes
void fillIntArray(int *arr, int arrSize);
void fillFloatArray(float arr[], int size);
void fillDoubleArray(int *arr, int arrSize);
void fillStringArray(char **arr, int arrSize, IEcoMemoryAllocator1 *pIMem);

int compareInt(const void *a_ptr, const void *b_ptr);
int compareFloat(const void *a, const void *b);
int compareDouble(const void *a, const void *b);
int compareString(const void *a_ptr, const void *b_ptr);

void testIntSort(IEcoLab1 *pIEcoLab1, FILE *file, int arrSize, IEcoMemoryAllocator1 *pIMem);
void testDoubleSort(IEcoLab1 *pIEcoLab1, FILE *file, int arrSize, IEcoMemoryAllocator1 *pIMem);
void testFloatSort(IEcoLab1 *pIEcoLab1, FILE *file, int arrSize, IEcoMemoryAllocator1 *pIMem);
void testStringSort(IEcoLab1 *pIEcoLab1, FILE *file, int arrSize, IEcoMemoryAllocator1 *pIMem);
void testSort(IEcoLab1 *pIEcoLab1, int arrSize, IEcoMemoryAllocator1 *pIMem);

int16_t EcoMain(IEcoUnknown* pIUnk);

#endif //ECOLAB1_H
