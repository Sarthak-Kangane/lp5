#include <sys/_types/_rune_t.h>
%%writefile cuda_program.cu

#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;

// ---------------- VECTOR ADDITION ----------------

#define VECTOR_SIZE 1000000

__global__ void vectorAdd(int *A, int *B, int *C)
{
    int i = threadIdx.x + blockIdx.x * blockDim.x;

    if(i < VECTOR_SIZE)
    {
        C[i] = A[i] + B[i];
    }
}

// ---------------- MATRIX MULTIPLICATION ----------------

#define MATRIX_SIZE 512

__global__ void matrixMul(int *A, int *B, int *C)
{
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if(row < MATRIX_SIZE && col < MATRIX_SIZE)
    {
        int sum = 0;

        for(int k = 0; k < MATRIX_SIZE; k++)
        {
            sum += A[row * MATRIX_SIZE + k] *
                   B[k * MATRIX_SIZE + col];
        }

        C[row * MATRIX_SIZE + col] = sum;
    }
}

int main()
{
    // =====================================================
    // VECTOR ADDITION
    // =====================================================

    cout << "========== VECTOR ADDITION ==========\n";

    int *A, *B, *C;
    int *d_A, *d_B, *d_C;

    A = new int[VECTOR_SIZE];
    B = new int[VECTOR_SIZE];
    C = new int[VECTOR_SIZE];

    // Initialize vectors
    for(int i = 0; i < VECTOR_SIZE; i++)
    {
        A[i] = i;
        B[i] = i;
    }

    // ---------------- CPU VECTOR ADDITION ----------------

    auto cpu_start = high_resolution_clock::now();

    for(int i = 0; i < VECTOR_SIZE; i++)
    {
        C[i] = A[i] + B[i];
    }

    auto cpu_end = high_resolution_clock::now();

    auto cpu_duration =
    duration_cast<milliseconds>(cpu_end - cpu_start);

    cout << "CPU Vector Addition Time: "
         << cpu_duration.count()
         << " ms\n";

    // ---------------- GPU VECTOR ADDITION ----------------

    cudaMalloc((void**)&d_A, VECTOR_SIZE * sizeof(int));
    cudaMalloc((void**)&d_B, VECTOR_SIZE * sizeof(int));
    cudaMalloc((void**)&d_C, VECTOR_SIZE * sizeof(int));

    cudaMemcpy(d_A, A,
               VECTOR_SIZE * sizeof(int),
               cudaMemcpyHostToDevice);

    cudaMemcpy(d_B, B,
               VECTOR_SIZE * sizeof(int),
               cudaMemcpyHostToDevice);

    cudaEvent_t start, stop;

    cudaEventCreate(&start);
    cudaEventCreate(&stop);

    cudaEventRecord(start);

    vectorAdd<<<(VECTOR_SIZE + 255)/256, 256>>>(d_A, d_B, d_C);

    cudaDeviceSynchronize();

    cudaEventRecord(stop);

    cudaEventSynchronize(stop);

    float gpu_time = 0;

    cudaEventElapsedTime(&gpu_time, start, stop);

    cudaMemcpy(C, d_C,
               VECTOR_SIZE * sizeof(int),
               cudaMemcpyDeviceToHost);

    cout << "GPU Vector Addition Time: "
         << gpu_time
         << " ms\n";

    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    delete[] A;
    delete[] B;
    delete[] C;

    // =====================================================
    // MATRIX MULTIPLICATION
    // =====================================================

    cout << "\n========== MATRIX MULTIPLICATION ==========\n";

    int size = MATRIX_SIZE * MATRIX_SIZE;

    int *M1, *M2, *M3;
    int *d_M1, *d_M2, *d_M3;

    M1 = new int[size];
    M2 = new int[size];
    M3 = new int[size];

    // Initialize matrices
    for(int i = 0; i < size; i++)
    {
        M1[i] = 1;
        M2[i] = 1;
    }

    // ---------------- CPU MATRIX MULTIPLICATION ----------------

    auto cpu_start_mat = high_resolution_clock::now();

    for(int i = 0; i < MATRIX_SIZE; i++)
    {
        for(int j = 0; j < MATRIX_SIZE; j++)
        {
            int sum = 0;

            for(int k = 0; k < MATRIX_SIZE; k++)
            {
                sum += M1[i * MATRIX_SIZE + k] *
                       M2[k * MATRIX_SIZE + j];
            }

            M3[i * MATRIX_SIZE + j] = sum;
        }
    }

    auto cpu_end_mat = high_resolution_clock::now();

    auto cpu_duration_mat =
    duration_cast<milliseconds>(cpu_end_mat - cpu_start_mat);

    cout << "CPU Matrix Multiplication Time: "
         << cpu_duration_mat.count()
         << " ms\n";

    // ---------------- GPU MATRIX MULTIPLICATION ----------------

    cudaMalloc((void**)&d_M1, size * sizeof(int));
    cudaMalloc((void**)&d_M2, size * sizeof(int));
    cudaMalloc((void**)&d_M3, size * sizeof(int));

    cudaMemcpy(d_M1, M1,
               size * sizeof(int),
               cudaMemcpyHostToDevice);

    cudaMemcpy(d_M2, M2,
               size * sizeof(int),
               cudaMemcpyHostToDevice);

    dim3 threadsPerBlock(16,16);

    dim3 blocksPerGrid(
        (MATRIX_SIZE + 15)/16,
        (MATRIX_SIZE + 15)/16
    );

    cudaEventRecord(start);

    matrixMul<<<blocksPerGrid, threadsPerBlock>>>(
        d_M1, d_M2, d_M3
    );

    cudaDeviceSynchronize();

    cudaEventRecord(stop);

    cudaEventSynchronize(stop);

    cudaEventElapsedTime(&gpu_time, start, stop);

    cudaMemcpy(M3, d_M3,
               size * sizeof(int),
               cudaMemcpyDeviceToHost);

    cout << "GPU Matrix Multiplication Time: "
         << gpu_time
         << " ms\n";

    // Cleanup

    cudaFree(d_M1);
    cudaFree(d_M2);
    cudaFree(d_M3);

    delete[] M1;
    delete[] M2;
    delete[] M3;

    return 0;
}


//Commands to run
// !nvcc cuda_program.cu -o cuda_program
// ./cuda_program