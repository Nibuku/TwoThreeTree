#include <twothreetree/twothreetree.h>
#include <vector>
#include <iostream>
#include <chrono>
#include <numeric>
#include <set>
#include <random>

using namespace std;

int main() {
    std::vector<int> counts = { 100, 500, 1000, 5000, 10000, 25000, 50000, 100000 };
    int min_value = -10000; 
    int max_value = 1000000; 
    int num_runs = 100; 



    std::mt19937 engine; 
    engine.seed(std::time(nullptr));

    for (const auto& count : counts) {
        std::vector<double> insertion_times(num_runs); 
        std::vector<double> search_times(num_runs); 
        std::vector<double> removal_times(num_runs); 

        
        for (int i = 0; i < num_runs; ++i) {
            set<int> tree;
            std::vector<int> values(count);

            
            for (int j = 0; j < count; ++j) {
                values[j] = min_value +  engine() % (max_value - min_value + 1);
            }

            
            auto start_insertion = std::chrono::steady_clock::now();
            for (const auto& value : values) {
                tree.insert(value);
            }
            auto end_insertion = std::chrono::steady_clock::now();
            insertion_times[i] = std::chrono::duration<double, std::milli>(end_insertion - start_insertion).count();

            
            auto start_search = std::chrono::steady_clock::now();
            for (const auto& value : values) {
                tree.find(value);
            }
            auto end_search = std::chrono::steady_clock::now();
            search_times[i] = std::chrono::duration<double, std::milli>(end_search - start_search).count();

            
            auto start_removal = std::chrono::steady_clock::now();
            for (const auto& value : values) {
                tree.erase(value);
            }
            auto end_removal = std::chrono::steady_clock::now();
            removal_times[i] = std::chrono::duration<double, std::milli>(end_removal - start_removal).count();
        }

        double average_insertion_time = std::accumulate(insertion_times.begin(), insertion_times.end(), 0.0) / num_runs;
        double average_search_time = std::accumulate(search_times.begin(), search_times.end(), 0.0) / num_runs;
        double average_removal_time = std::accumulate(removal_times.begin(), removal_times.end(), 0.0) / num_runs;

        std::cout << "For " << count << " values:" << std::endl;
        std::cout << "Average time taken to insert: " << average_insertion_time << " ms" << std::endl;
        std::cout << "Average time taken to search: " << average_search_time << " ms" << std::endl;
        std::cout << "Average time taken to remove: " << average_removal_time << " ms" << std::endl;
    }

    return 0;
}



