#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>

using namespace std;

// =======================
// Sequential Bubble Sort
// =======================
void bubble(vector<int>& arr, int n)
{
    for(int i = 0; i < n - 1; i++)
    {
        for(int j = 0; j < n - i - 1; j++)
        {
            if(arr[j] > arr[j + 1])
            {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// =====================================
// Parallel Bubble Sort (Odd-Even Sort)
// =====================================
void bubble_parallel(vector<int>& arr, int n)
{
    for(int i = 0; i < n; i++)
    {
        // Even phase
        #pragma omp parallel for
        for(int j = 0; j < n - 1; j += 2)
        {
            if(arr[j] > arr[j + 1])
            {
                swap(arr[j], arr[j + 1]);
            }
        }

        // Odd phase
        #pragma omp parallel for
        for(int j = 1; j < n - 1; j += 2)
        {
            if(arr[j] > arr[j + 1])
            {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// =======================
// Merge Function
// =======================
void merge_common(vector<int>& arr, int start, int mid, int end)
{
    int n1 = mid - start + 1;
    int n2 = end - mid;

    vector<int> left(n1);
    vector<int> right(n2);

    for(int i = 0; i < n1; i++)
    {
        left[i] = arr[start + i];
    }

    for(int i = 0; i < n2; i++)
    {
        right[i] = arr[mid + 1 + i];
    }

    int i = 0, j = 0, k = start;

    while(i < n1 && j < n2)
    {
        if(left[i] <= right[j])
        {
            arr[k++] = left[i++];
        }
        else
        {
            arr[k++] = right[j++];
        }
    }

    while(i < n1)
    {
        arr[k++] = left[i++];
    }

    while(j < n2)
    {
        arr[k++] = right[j++];
    }
}

// =======================
// Sequential Merge Sort
// =======================
void merge_seq(vector<int>& arr, int start, int end)
{
    if(start >= end)
    {
        return;
    }

    int mid = start + (end - start) / 2;

    merge_seq(arr, start, mid);
    merge_seq(arr, mid + 1, end);

    merge_common(arr, start, mid, end);
}

// =======================
// Parallel Merge Sort
// =======================
void merge_parallel(vector<int>& arr, int start, int end, int depth)
{
    if(start >= end)
    {
        return;
    }

    int mid = start + (end - start) / 2;

    // Limit thread creation depth
    if(depth <= 0)
    {
        merge_seq(arr, start, mid);
        merge_seq(arr, mid + 1, end);
    }
    else
    {
        #pragma omp parallel sections
        {
            #pragma omp section
            {
                merge_parallel(arr, start, mid, depth - 1);
            }

            #pragma omp section
            {
                merge_parallel(arr, mid + 1, end, depth - 1);
            }
        }
    }

    merge_common(arr, start, mid, end);
}

// =======================
// Print Array
// =======================
void printArray(vector<int>& arr)
{
    for(int x : arr)
    {
        cout << x << " ";
    }
    cout << endl;
}

// =======================
// Main Function
// =======================
int main()
{
    int n;

    cout << "Enter number of elements: ";
    cin >> n;

    vector<int> arr1(n), arr2(n), arr3(n), arr4(n);

    cout << "Enter elements:\n";

    for(int i = 0; i < n; i++)
    {
        cin >> arr1[i];

        arr2[i] = arr1[i];
        arr3[i] = arr1[i];
        arr4[i] = arr1[i];
    }

    chrono::high_resolution_clock::time_point start, end;
    chrono::duration<double> diff;

    // =======================
    // Sequential Bubble Sort
    // =======================
    cout << "\nSequential Bubble Sort Time:\n";

    start = chrono::high_resolution_clock::now();

    bubble(arr1, n);

    end = chrono::high_resolution_clock::now();

    diff = end - start;

    cout << diff.count() << " seconds\n";

    // =======================
    // Parallel Bubble Sort
    // =======================
    cout << "\nParallel Bubble Sort Time:\n";

    start = chrono::high_resolution_clock::now();

    bubble_parallel(arr2, n);

    end = chrono::high_resolution_clock::now();

    diff = end - start;

    cout << diff.count() << " seconds\n";

    // =======================
    // Sequential Merge Sort
    // =======================
    cout << "\nSequential Merge Sort Time:\n";

    start = chrono::high_resolution_clock::now();

    merge_seq(arr3, 0, n - 1);

    end = chrono::high_resolution_clock::now();

    diff = end - start;

    cout << diff.count() << " seconds\n";

    // =======================
    // Parallel Merge Sort
    // =======================
    cout << "\nParallel Merge Sort Time:\n";

    start = chrono::high_resolution_clock::now();

    merge_parallel(arr4, 0, n - 1, 4);

    end = chrono::high_resolution_clock::now();

    diff = end - start;

    cout << diff.count() << " seconds\n";

    // =======================
    // Sorted Output
    // =======================
    cout << "\nSorted Array:\n";
    printArray(arr4);

    return 0;
}

//g++ -fopenmp your_program.cpp -o program.exe

// .\program.exe
