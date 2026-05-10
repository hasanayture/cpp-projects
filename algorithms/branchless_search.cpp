#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <numeric>

int binary_search_branched(const std::vector<int>& sorted_data, int target_value) {
    int left_index = 0;
    int right_index = static_cast<int>(sorted_data.size()) - 1;

    while (left_index <= right_index) {
        int middle_index = left_index + (right_index - left_index) / 2;
        if (sorted_data[middle_index] < target_value) {
            left_index = middle_index + 1;
        } else if (sorted_data[middle_index] > target_value) {
            right_index = middle_index - 1;
        } else {
            return middle_index;
        }
    }
    return -1;
}

int binary_search_branchless(const std::vector<int>& sorted_data, int target_value) {
    int remaining_size = static_cast<int>(sorted_data.size());
    const int* current_base = sorted_data.data();

    while (remaining_size > 1) {
        int half_size = remaining_size / 2;
        current_base += (current_base[half_size - 1] < target_value) * half_size;
        remaining_size -= half_size;
    }

    return (*current_base == target_value) ? static_cast<int>(current_base - sorted_data.data()) : -1;
}

int main() {
    const size_t array_size = 1 << 20;
    const int query_count = 1000000;
    const int warmup_iterations = 100000;

    std::vector<int> dataset(array_size);
    std::iota(dataset.begin(), dataset.end(), 0);

    std::mt19937 random_generator(42);
    std::uniform_int_distribution<int> distribution(0, static_cast<int>(array_size) - 1);

    std::vector<int> search_queries(query_count);
    for (int i = 0; i < query_count; ++i) {
        search_queries[i] = distribution(random_generator);
    }

    for (int i = 0; i < warmup_iterations; ++i) {
        int target = search_queries[i % query_count];
        volatile int result_a = binary_search_branched(dataset, target);
        volatile int result_b = binary_search_branchless(dataset, target);
        (void)result_a;
        (void)result_b;
    }

    auto start_branched = std::chrono::high_resolution_clock::now();
    long long branched_checksum = 0;
    for (int i = 0; i < query_count; ++i) {
        branched_checksum += binary_search_branched(dataset, search_queries[i]);
    }
    auto end_branched = std::chrono::high_resolution_clock::now();

    auto start_branchless = std::chrono::high_resolution_clock::now();
    long long branchless_checksum = 0;
    for (int i = 0; i < query_count; ++i) {
        branchless_checksum += binary_search_branchless(dataset, search_queries[i]);
    }
    auto end_branchless = std::chrono::high_resolution_clock::now();

    auto branched_duration = std::chrono::duration_cast<std::chrono::microseconds>(end_branched - start_branched).count();
    auto branchless_duration = std::chrono::duration_cast<std::chrono::microseconds>(end_branchless - start_branchless).count();

    std::cout << "Branched Search: " << branched_duration << " us (Checksum: " << branched_checksum << ")\n";
    std::cout << "Branchless Search: " << branchless_duration << " us (Checksum: " << branchless_checksum << ")\n";

    return 0;
}
