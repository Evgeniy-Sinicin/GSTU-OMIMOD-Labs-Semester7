#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <thread>
#include <chrono>
#include <mutex>
#include <bits/stdc++.h>
#include "Process.h"

void print_matrixn(float** matrix, int size, int precision);
void print_array(float* arr, int size, int precision);
void write_solve(float* solves, int size, std::string path);
float** read_system(std::string systemPath, int* size);
float** calculate_l(float** system, int size);
float** calculate_transpose(float** matrixL, int size);
float* read_coeffs(std::string coeffsPath, int* size);
float* solve_l(float** matrixL, float* coeffsY, int coeffsCount);
float* solve_lt(float** matrixLT, float* solveY, int coeffsCount);

int main()  
{
    std::cout << "Main thread with id:" << std::this_thread::get_id() << " started..." << std::endl;

    int processes_count = 4;
    Process* processes = new Process[processes_count]();
    std::thread* threads = new std::thread[processes_count];

#pragma region Pathes
    std::vector<std::string> system_pathes;
    system_pathes.push_back("./Debug/Lab_1_Resources/System_1.txt");
    system_pathes.push_back("./Debug/Lab_1_Resources/System_2.txt");
    system_pathes.push_back("./Debug/Lab_1_Resources/System_3.txt");
    system_pathes.push_back("./Debug/Lab_1_Resources/System_4.txt");

    std::vector<std::string> coeffs_pathes;
    coeffs_pathes.push_back("./Debug/Lab_1_Resources/Coeffs_1.txt");
    coeffs_pathes.push_back("./Debug/Lab_1_Resources/Coeffs_2.txt");
    coeffs_pathes.push_back("./Debug/Lab_1_Resources/Coeffs_3.txt");
    coeffs_pathes.push_back("./Debug/Lab_1_Resources/Coeffs_4.txt");

    std::vector<std::string> decisions_pathes;
    decisions_pathes.push_back("./Debug/Lab_1_Resources/Decision_1.txt");
    decisions_pathes.push_back("./Debug/Lab_1_Resources/Decision_2.txt");
    decisions_pathes.push_back("./Debug/Lab_1_Resources/Decision_3.txt");
    decisions_pathes.push_back("./Debug/Lab_1_Resources/Decision_4.txt");
#pragma endregion
    
    std::mutex mtx;
    typedef std::chrono::high_resolution_clock clock;

#pragma region Multithread
    auto start_multithread_time = std::chrono::duration_cast<std::chrono::milliseconds>(clock::now().time_since_epoch()).count();

    for (int i = 0; i < processes_count; i++)
    {
        threads[i] = std::thread([&processes, &system_pathes, &coeffs_pathes, &decisions_pathes, &mtx, i]()
            {
                typedef std::chrono::high_resolution_clock clock;
                processes[i].set_start_time(std::chrono::duration_cast<std::chrono::milliseconds>(clock::now().time_since_epoch()).count());
                processes[i].set_thread_id(std::this_thread::get_id());
                //std::cout << "Process with thread id:" << processes[i].get_thread_id() << " started." << std::endl;

                int printPrecision = 6;
                int systemSize1;
                float** system1 = read_system(system_pathes[i], &systemSize1);

                mtx.lock();
                std::cout << "--- System ID(" << processes[i].get_thread_id() << ") ---" << std::endl;
                print_matrixn(system1, systemSize1, printPrecision);
                mtx.unlock();

                int coeffsSize1;
                float* coeffs1 = read_coeffs(coeffs_pathes[i], &coeffsSize1);

                mtx.lock();
                std::cout << "--- Coeffs ID(" << processes[i].get_thread_id() << ") ---" << std::endl;
                print_array(coeffs1, coeffsSize1, printPrecision);
                mtx.unlock();

                float** matrixL1 = calculate_l(system1, systemSize1);

                mtx.lock();
                std::cout << "--- L1 Matrix ID(" << processes[i].get_thread_id() << ") ---" << std::endl;
                print_matrixn(matrixL1, systemSize1, printPrecision);
                mtx.unlock();

                float** matrixLT1 = calculate_transpose(matrixL1, systemSize1);

                mtx.lock();
                std::cout << "--- LT1 Matrix ID(" << processes[i].get_thread_id() << ") ---" << std::endl;
                print_matrixn(matrixLT1, systemSize1, printPrecision);
                mtx.unlock();

                float* decisionL1 = solve_l(matrixL1, coeffs1, coeffsSize1);

                mtx.lock();
                std::cout << "--- L1 Decision ID(" << processes[i].get_thread_id() << ") ---" << std::endl;
                print_array(decisionL1, coeffsSize1, printPrecision);
                mtx.unlock();

                float* decisionLT1 = solve_lt(matrixLT1, decisionL1, coeffsSize1);

                mtx.lock();
                std::cout << "--- LT1 Decision ID(" << processes[i].get_thread_id() << ") ---" << std::endl;
                print_array(decisionLT1, coeffsSize1, printPrecision);
                mtx.unlock();

                write_solve(decisionLT1, coeffsSize1, decisions_pathes[i]);

                processes[i].set_finish_time(std::chrono::duration_cast<std::chrono::milliseconds>(clock::now().time_since_epoch()).count());
                //std::cout << "Process with thread id:" << processes[i].get_thread_id() << " finished." << std::endl;

                mtx.lock();
                std::cout << "Process with thread id:" << processes[i].get_thread_id() << " lived " << processes[i].get_finish_time() - processes[i].get_start_time() << " milliseconds." << std::endl;
                mtx.unlock();
            });

        //threads[i].detach();
    }

    /*for (int i = 0; i < 5; i++)
    {
        std::cout << "Main thread waits other threads..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }*/

    for (int i = 0; i < processes_count; i++)
    {
        threads[i].join();
    }

    auto end_multithread_time = std::chrono::duration_cast<std::chrono::milliseconds>(clock::now().time_since_epoch()).count();

    std::cout << "Threads complite time: " << end_multithread_time - start_multithread_time << " milliseconds." << std::endl;
#pragma endregion

    std::cout << "\n--- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---\n" << std::endl;

#pragma region Singlethread
    auto start_singlethread_time = std::chrono::duration_cast<std::chrono::milliseconds>(clock::now().time_since_epoch()).count();

    for (int i = 0; i < processes_count; i++)
    {
        int printPrecision = 6;
        int systemSize1;
        float** system1 = read_system(system_pathes[i], &systemSize1);

        std::cout << "--- System ID(" << std::this_thread::get_id() << ") ---" << std::endl;
        print_matrixn(system1, systemSize1, printPrecision);

        int coeffsSize1;
        float* coeffs1 = read_coeffs(coeffs_pathes[i], &coeffsSize1);

        std::cout << "--- Coeffs ID(" << std::this_thread::get_id() << ") ---" << std::endl;
        print_array(coeffs1, coeffsSize1, printPrecision);

        float** matrixL1 = calculate_l(system1, systemSize1);

        std::cout << "--- L1 Matrix ID(" << std::this_thread::get_id() << ") ---" << std::endl;
        print_matrixn(matrixL1, systemSize1, printPrecision);

        float** matrixLT1 = calculate_transpose(matrixL1, systemSize1);

        std::cout << "--- LT1 Matrix ID(" << std::this_thread::get_id() << ") ---" << std::endl;
        print_matrixn(matrixLT1, systemSize1, printPrecision);

        float* decisionL1 = solve_l(matrixL1, coeffs1, coeffsSize1);

        std::cout << "--- L1 Decision ID(" << std::this_thread::get_id() << ") ---" << std::endl;
        print_array(decisionL1, coeffsSize1, printPrecision);

        float* decisionLT1 = solve_lt(matrixLT1, decisionL1, coeffsSize1);

        std::cout << "--- LT1 Decision ID(" << std::this_thread::get_id() << ") ---" << std::endl;
        print_array(decisionLT1, coeffsSize1, printPrecision);

        write_solve(decisionLT1, coeffsSize1, decisions_pathes[i]);
    }

    auto end_singlethread_time = std::chrono::duration_cast<std::chrono::milliseconds>(clock::now().time_since_epoch()).count();

    std::cout << "Single thread with id:" << std::this_thread::get_id() << " finished. Complite time: " << end_singlethread_time - start_singlethread_time << " milliseconds." << std::endl;

#pragma endregion
    
    system("pause");

    return EXIT_SUCCESS;
}

void write_solve(float* solves, int size, std::string path)
{
    std::ofstream out;

    out.open(path);

    for (int i = 0; i < size; i++)
    {
        out << solves[i] << std::endl;
    }

    out.close();
}

float* read_coeffs(std::string coeffsPath, int* size)
{
    int sizeInt;
    std::string sizeStr;
    std::ifstream in(coeffsPath);

    if (!in.is_open())
    {
        return nullptr;
    }

    getline(in, sizeStr);
    sizeInt = stoi(sizeStr);

    if (size != nullptr)
    {
        *size = sizeInt;
    }

    float* coeffs = new float[sizeInt];

    for (int i = 0; i < sizeInt; ++i)
    {
        std::string coeff;
        getline(in, coeff);
        coeffs[i] = stof(coeff);
    }

    return coeffs;
}

float** read_system(std::string systemPath, int* size)
{
    std::string matrixSizeStr;
    std::ifstream in(systemPath);
    std::vector<float> elements;

    if (!in.is_open())
    {
        return nullptr;
    }

    getline(in, matrixSizeStr);
    *size = stoi(matrixSizeStr);
    std::string line;
    std::string delimeter = " ";
    float** matrix = new float* [*size];

    for (int i = 0; i < *size; i++)
    {
        getline(in, line);
        matrix[i] = new float[*size];

        for (int j = 0; j < *size; j++)
        {
            size_t delimiterPosition = line.find(delimeter);
            std::string elementStr = line.substr(0, delimiterPosition);
            matrix[i][j] = stof(elementStr);
            elements.push_back(matrix[i][j]);
            line.erase(0, delimiterPosition + delimeter.length());
        }
    }

    in.close();

    return matrix;
}

float** calculate_l(float** system, int size)
{
    float** matrixL = new float* [size];

    for (int row = 0; row < size; row++)
    {
        matrixL[row] = new float[size];

        for (int col = 0; col < size; col++)
        {
            matrixL[row][col] = 0.0f;
        }
    }

    for (int row = 0; row < size; row++)
    {
        float sum = 0.0f;

        for (int col = 0; col <= row; col++)
        {
            sum = 0.0f;

            if (col == row)
            {
                for (int k = 0; k < col; k++)
                {
                    sum += powf(matrixL[col][k], 2);
                }

                matrixL[col][col] = roundf(sqrtf(system[col][col] - sum) * 100.0f) / 100.0f;
            }
            else
            {
                for (int k = 0; k < col; k++)
                {
                    sum += matrixL[row][k] * matrixL[col][k];
                }

                matrixL[row][col] = roundf((system[row][col] - sum) / matrixL[col][col] * 100.0f) / 100.0f;
            }
        }
    }

    return matrixL;
}

float* solve_l(float** matrixL, float* coeffsY, int coeffsCount)
{
    float* solveY = new float[coeffsCount];

    for (int row = 0; row < coeffsCount; row++)
    {
        float sum = 0;

        for (int col = 0; col <= row; col++)
        {
            if (col == row)
            {
                solveY[row] = (coeffsY[row] - sum) / matrixL[row][col];
            }
            else
            {
                sum += solveY[col] * matrixL[row][col];
            }
        }
    }

    return solveY;
}

float** calculate_transpose(float** matrixL, int size)
{
    float** matrixLT = new float*[size];

    for (int row = 0; row < size; row++)
    {
        matrixLT[row] = new float[size];

        for (int col = 0; col < size; col++)
        {
            matrixLT[row][col] = matrixL[col][row];
        }
    }

    return matrixLT;
}

float* solve_lt(float** matrixLT, float* solveY, int coeffsCount)
{
    float* solveX = new float[coeffsCount];

    for (int row = coeffsCount - 1; row >= 0; row--)
    {
        float sum = 0;

        for (int col = coeffsCount - 1; col >= row; col--)
        {
            if (col == row)
            {
                solveX[row] = (solveY[row] - sum) / matrixLT[row][col];
            }
            else
            {
                sum += solveX[col] * matrixLT[row][col];
            }
        }
    }

    return solveX;
}

void print_matrixn(float** matrix, int size, int precision)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            std::cout << std::setw(precision) << matrix[i][j];
        }

        std::cout << std::endl;
    }
}

void print_array(float* arr, int size, int precision)
{
    for (int i = 0; i < size; i++)
    {
        std::cout << std::setw(precision) << arr[i] << std::endl;
    }
}