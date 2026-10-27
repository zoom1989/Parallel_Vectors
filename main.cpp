#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <iomanip>

// Функция для сложения части векторов
void sum_vectors(const std::vector<int>& a, const std::vector<int>& b, std::vector<int>& result, size_t start, size_t end) {
    for (size_t i = start; i < end; ++i) {
        result[i] = a[i] + b[i];
    }
}

// Функция для запуска теста с конкретным числом потоков
double run_test(size_t size, int num_threads) {
    // Генерируем векторы
    std::vector<int> a(size, 1);
    std::vector<int> b(size, 2);
    std::vector<int> result(size, 0);

    auto start_time = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> threads;
    size_t chunk_size = size / num_threads;

    for (int i = 0; i < num_threads; ++i) {
        size_t start = i * chunk_size;
        // Последний поток забирает остаток элементов
        size_t end = (i == num_threads - 1) ? size : start + chunk_size;
        threads.emplace_back(sum_vectors, std::cref(a), std::cref(b), std::ref(result), start, end);
    }

    for (auto& t : threads) {
        t.join();
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end_time - start_time;
    return diff.count();
}

int main() {
    // 1. Вывод количества ядер
    unsigned int cores = std::thread::hardware_concurrency();
    std::cout << "Количество аппаратных ядер - " << cores << "\n\n";

    // 2. Заголовок таблицы
    std::vector<size_t> sizes = { 1000, 10000, 100000, 1000000 };
    std::vector<int> threads_count = { 1, 2, 4, 8, 16 };

    std::cout << std::setw(12) << ""; // Отступ для названий потоков
    for (size_t size : sizes) {
        std::cout << std::setw(15) << size;
    }
    std::cout << "\n";

    // 3. Запуск тестов и вывод результатов
    for (int t : threads_count) {
        std::cout << std::setw(2) << t << " potokov ";

        for (size_t size : sizes) {
            double time = run_test(size, t);
            std::cout << std::setw(14) << std::fixed << std::setprecision(6) << time << "s ";
        }
        std::cout << "\n";
    }

    return 0;
}