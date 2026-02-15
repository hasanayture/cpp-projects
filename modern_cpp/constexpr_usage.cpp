#include <iostream>
#include <vector>
#include <array>
#include <ranges>
#include <algorithm>
#include <iterator>
#include <iomanip>
#include <type_traits>

constexpr int compile_time_size = 5;
constexpr int square(int x) { return x * x; }

// Generate lookup table at compile-time
constexpr std::array<int, compile_time_size> generate_squares() {
    std::array<int, compile_time_size> arr{};
    for (int i = 0; i < compile_time_size; ++i)
        arr[i] = square(i);
    return arr;
}
constexpr auto squares_table = generate_squares();

// Force compile-time evaluation with checks
consteval int safe_value(int x) {
    if (x < 0) throw "Negative not allowed";
    return x;
}

constexpr int label_width = 32;

template<typename T>
void print_value(std::string_view label, const T& value) {
    std::cout << std::left << std::setw(label_width)
              << label << " : " << value << "\n";
}

template<std::ranges::input_range R>
void print_range(R&& r, std::string_view label) {
    std::cout << std::left << std::setw(label_width)
              << label << " : ";
    for (auto&& e : r)
        std::cout << e << " ";
    std::cout << "\n";
}

template<typename T>
void print_type(T value) {
    if constexpr (std::is_integral_v<T>)
        print_value("Integral", value);
    else
        print_value("Other type", "");
}

int main() {
    constexpr int val = square(4);
    constexpr int safe = safe_value(10);

    print_value("Compile-time square(4)", val);
    print_value("Consteval safe_value(10)", safe);
    print_range(squares_table, "Compile-time squares table");

    std::vector<int> numbers{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto even_squares = numbers
                        | std::views::filter([](int x){ return x % 2 == 0; })
                        | std::views::transform([](int x){ return x * x; });
    print_range(even_squares, "Even squares");

    auto first_five = std::views::iota(1)
                      | std::views::take(5);
    print_range(first_five, "First five natural numbers");

    std::vector<int> unsorted{5, 1, 8, 3, 11, 25, 43, 54};
    std::ranges::sort(unsorted);
    print_range(unsorted, "Sorted numbers");

    print_type(42);
    print_type(3.14);

    return 0;
}
