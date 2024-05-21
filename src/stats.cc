#include <twothreetree/twothreetree.h>
#include <vector>
#include <iostream>
#include <chrono>
#include <numeric> 


using namespace std;

int main() {
    std::srand(std::time(0)); // Инициализация генератора случайных чисел
    int count = 10; // Количество случайных значений для вставки
    int min_value = -10000; // Минимальное случайное значение
    int max_value = 10000; // Максимальное случайное значение
    int num_runs = 100; // Количество запусков для вычисления среднего времени

    std::vector<double> insertion_times(num_runs); // Вектор для хранения времен вставки
    std::vector<double> search_times(num_runs); // Вектор для хранения времен удаления

    // Повторяем num_runs раз
    for (int i = 0; i < num_runs; ++i) {
        Two3Tree<int> tree;
        std::vector<int> values(count);

        // Генерируем случайные значения
        for (int j = 0; j < count; ++j) {
            values[j] = min_value + std::rand() % (max_value - min_value + 1);
        }

        // Засекаем время вставки
        auto start_insertion = std::chrono::steady_clock::now();
        for (const auto& value : values) {
            tree.insert(value);
        }
        auto end_insertion = std::chrono::steady_clock::now();
        insertion_times[i] = std::chrono::duration<double, std::milli>(end_insertion - start_insertion).count();

        // Засекаем время поиска
        auto start_search = std::chrono::steady_clock::now();
        tree.search_random_values(values);
        auto end_search = std::chrono::steady_clock::now();
        search_times[i] = std::chrono::duration<double, std::milli>(end_search - start_search).count();
    }

    double average_insertion_time = std::accumulate(insertion_times.begin(), insertion_times.end(), 0.0) / num_runs;
    double average_search_time = std::accumulate(search_times.begin(), search_times.end(), 0.0) / num_runs;

    std::cout << "Average time taken to insert " << count << " values for " << num_runs << " runs: "
        << average_insertion_time << " ms" << std::endl;

    std::cout << "Average time taken to search " << count << " values for " << num_runs << " runs: "
        << average_search_time << " ms" << std::endl;

    return 0;
}



/*Bin tree
Avg time for insert 1 000: 0.09000 ms
Avg time for insert 10 000: 1.01000 ms
Avg time for insert 100 000: 12.35000 ms

Avg time for search 1 000: 0.08000 ms
Avg time for search 10 000: 0.90000 ms
Avg time for search 100 000: 6.06000 ms

Avg time for erase 1 000: 0.08000 ms
Avg time for erase 10 000: 0.7000 ms
Avg time for erase 100 000: 4.37000 ms

Vector
Avg time for insert 1 000: 0.03000 ms
Avg time for insert 10 000: 0.33000 ms
Avg time for insert 100 000: 3.97000 ms

Avg time for find 1 000: 0.21000 ms
Avg time for find 10 000: 2.03000 ms
Avg time for find 100 000: 18.44000 ms

Avg time for erase 1 000: 0.45000 ms
Avg time for erase 10 000: 4.40000 ms
Avg time for erase 100 000: 35.50000 ms
*/