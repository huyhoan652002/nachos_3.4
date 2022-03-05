#define SIZE 9999
#include <syscall.h>
// bubble sort function
void bubble_sort(int *array, int size)
{
    int i, j, tmp;
    for (i = 0; i < size; i++) {
        for (j = 0; j < (size-1); j++) {
            if (array[j] > array[j + 1]) {
                tmp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = tmp;
            }
        }
    }
}

int main()
{
    int i, j, tmp;
    int array[SIZE];

    // initialize the array, in reverse sorted order
    for (i = 0; i < SIZE; i++) {
        array[i] = (SIZE-1) - i;
    }

    // sort!
    bubble_sort(array, SIZE);

    // check if the array is sorted
    for (i = 0; i < SIZE; i++) {
        if (array[i] != i) {
            Exit(1);
        }
    }

    // print the array
    for (i=0; i<SIZE; i++) {
        PrintNum(array[i]);
        PrintChar(' ');
        if (((i+1) % 15) == 0) {
            PrintChar('\n');
        }
    }
    PrintChar('\n');

    Exit(0);
}