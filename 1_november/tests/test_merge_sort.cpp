#include <gtest/gtest.h>
#include "sort.h"

TEST(MergeSortTest, Basic) {
    int arr[] = {5, 2, 8, 1, 3};
    int size = 5;
    lae::mergeSort(arr, size);
    
    for (int i = 1; i < size; i++) {
        EXPECT_LE(arr[i-1], arr[i]);
    }
}

TEST(MergeSortTest, SingleElement) {
    int arr[] = {42};
    int size = 1;
    lae::mergeSort(arr, size);
    EXPECT_EQ(arr[0], 42);
}
