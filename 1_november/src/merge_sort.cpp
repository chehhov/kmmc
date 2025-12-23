#include "merge_sort.h"
#include "sort.h"
#include <iostream>
namespace lae {

void merge(int arr[], int left, int mid, int right) {
    const int size_left = mid - left + 1;
    const int size_right = right - mid;
    
    int* leftArr = new int[size_left];
    int* rightArr = new int[size_right];
    
    for (int i = 0; i < size_left; i++) {
        leftArr[i] = arr[left + i];
    }
    for (int j = 0; j < size_right; j++) {
        rightArr[j] = arr[mid + 1 + j];
    }
    int i = 0, j = 0, k = left;
    
    while (i < size_left && j < size_right) {
        if (leftArr[i] <= rightArr[j]) {
            arr[k] = leftArr[i];
            i++;
        } else {
            arr[k] = rightArr[j];
            j++;
        }
        k++;
    }
    
    while (i < size_left) {
        arr[k] = leftArr[i];
        i++;
        k++;
    }
    
    while (j < size_right) {
        arr[k] = rightArr[j];
        j++;
        k++;
    }
    
    delete[] leftArr;
    delete[] rightArr;
}

void mergeSort(int arr[], int left, int right) {
    if (left >= right) {
	 return;
    }
    const int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

void mergeSort(int arr[], int size) {
    if (size > 1) {
        mergeSort(arr, 0, size - 1);
    }
}

}

