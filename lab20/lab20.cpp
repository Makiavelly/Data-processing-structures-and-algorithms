#include <iostream>
#include <windows.h>
#include <fstream>
#include <vector>
#include <random>
#undef max
#define NON -1

const char* a_file("a.txt");
const char* b_file("b.txt");
const char* d_file("d.txt");
const char* c_file("c.txt");
const char* source_file("source.txt");
const int count_elen_in_series{ 200000 };
int count;
bool fEnd{ false };

void setConsoleColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void wrong_message(std::string mess) {
    setConsoleColor(4);
    std::cout << mess;
    setConsoleColor(7);
}

void right_message(std::string mess) {
    setConsoleColor(2);
    std::cout << mess;
    setConsoleColor(7);
}

bool checking_input(int& ans, int lower_limit, int upper_limit) {
    if (std::cin.peek() == '\n' && lower_limit <= ans && ans <= upper_limit) {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return true;
    }
    else {
        ans = NON;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        wrong_message("Ошибка ввода!");
        return false;
    }
}

void fast_method(int* arr, int left, int right) {
    int i{ left }, j{ right };
    int sred{ arr[(left + right) / 2] }, temp;
    do {
        while (sred > arr[i]) i++;
        
        while (arr[j] > sred) j--;

        if (i <= j) {
            temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
            i++; j--;
        }
    } while (j >= i);

    if (j > left) fast_method(arr, left, j);
    if (right > i) fast_method(arr, i, right);
}


void pre_sorting() {
    int* arr;
    int i{ 0 }, locale_count{ count };

    std::ofstream A(a_file);
    std::ofstream B(b_file);
    std::ifstream inp(source_file);

    while (locale_count - count_elen_in_series > 0) {
        arr = new int[count_elen_in_series];

        for (int j = 0; j < count_elen_in_series; j++) inp >> arr[j];
        fast_method(arr, 0, count_elen_in_series - 1);

        if (i % 2 == 0) {
            for (int j = 0; j < count_elen_in_series; j++) A << arr[j] << " ";
            A << std::endl;
        }
        else {
            for (int j = 0; j < count_elen_in_series; j++) B << arr[j] << " ";
            B << std::endl;
        }
        locale_count -= count_elen_in_series;
        delete[] arr;
        i++;
    }
    if (locale_count != 0) {
        arr = new int[locale_count];
        for (int j = 0; j < locale_count; j++) inp >> arr[j];
        fast_method(arr, 0, locale_count - 1);

        if (i % 2 == 0) {
            for (int j = 0; j < locale_count; j++) A << arr[j] << " ";
            A << std::endl;
        }
        else {
            for (int j = 0; j < locale_count; j++) B << arr[j] << " ";
            B << std::endl;
        }
        delete[] arr;
    }
    right_message("Предварительная сортировка завершена\n\n");
    A.close();
    B.close();
    inp.close();
}

void sorting(int deep) {
    if (fEnd) {
        right_message("Сортировкa завершена\n");
        if (deep % 2 == 0) std::cout << "Результат в " << a_file << std::endl;
        else std::cout << "Результат в " << c_file << std::endl;
    }
    else {
        if (deep % 2 == 0) {
            std::ifstream A(a_file);
            std::ifstream B(b_file);
            std::ofstream C(c_file);
            std::ofstream D(d_file);

            int a_elem, b_elem, j{ 0 };
            A >> a_elem;  B >> b_elem;

            while (!A.eof() || !B.eof()) {
                for (int i = 0; i < count_elen_in_series * deep; i++) {
                    if (A.eof()) {
                        if (B.eof()) break;
                        else {
                            while (B >> b_elem) {
                                if (B.eof()) break;
                                else {
                                    if (j % 2 == 0) C << b_elem << " ";
                                    else D << b_elem << " ";
                                }
                            }
                            break;
                            
                        }
                    }
                    else if (B.eof()) {
                        if (A.eof()) break;
                        else {
                            while (A >> a_elem) {
                                if (A.eof()) break;
                                else {
                                    if (j % 2 == 0) C << a_elem << " ";
                                    else D << a_elem << " ";
                                }
                            }
                            break;
                        }
                    }
                    else {
                        if (a_elem > b_elem) {
                            if (j % 2 == 0) C << b_elem << " ";
                            else D << b_elem << " ";
                            B >> b_elem;
                        }
                        else {
                            if (j % 2 == 0) C << a_elem << " ";
                            else D << a_elem << " ";
                            A >> a_elem;
                        }
                    }
                }
                if (j % 2 == 0) C << std::endl;
                else D << std::endl;
                j++;
            }
            if (j == 1) fEnd = true;
            A.close();
            B.close();
            C.close();
            D.close();

            sorting(deep + 1);
        }
        else {
            std::ofstream A(a_file);
            std::ofstream B(b_file);
            std::ifstream C(c_file);
            std::ifstream D(d_file);

            int c_elem, d_elem, j{ 0 };
            C >> c_elem;  D >> d_elem;

            while (!C.eof() || !D.eof()) {
                for (int i = 0; i < count_elen_in_series * deep; i++) {
                    if (C.eof()) {
                        if (D.eof()) break;
                        else {
                            while (D >> d_elem) {
                                if (D.eof()) break;
                                else {
                                    if (j % 2 == 0) A << d_elem << " ";
                                    else B << d_elem << " ";
                                }
                            }
                            break;

                        }
                    }
                    else if (D.eof()) {
                        if (C.eof()) break;
                        else {
                            while (C >> c_elem) {
                                if (C.eof()) break;
                                else {
                                    if (j % 2 == 0) A << c_elem << " ";
                                    else B << c_elem << " ";
                                }
                            }
                            break;
                        }
                    }
                    else {
                        if (c_elem > d_elem) {
                            if (j % 2 == 0) A << d_elem << " ";
                            else B << d_elem << " ";
                            D >> d_elem;
                        }
                        else {
                            if (j % 2 == 0) A << c_elem << " ";
                            else B << c_elem << " ";
                            C >> c_elem;
                        }
                    }
                }
                if (j % 2 == 0) A << std::endl;
                else B << std::endl;
                j++;
            }
            A.close();
            B.close();
            C.close();
            D.close();

            sorting(deep + 1);
        }
    }
}

int main()
{
    setlocale(LC_ALL, "ru");
    std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<int> distribution(1, 1000000);

	int ans;
	std::cout << "Автоматически заполнить файл, который нужно будет отсортировать:\n1. Да\n2. Нет\nОжидание ответа: ";
	std::cin >> ans;

    if (ans == 1) {
        std::cout << "Введите количество создаваемых элементов: ";
        std::cin >> count;

        std::ofstream out(source_file);
        for (int i = 0; i < count; i++) {
            if (i % 500 == 0) out << std::endl;
            out << distribution(generator) << " ";
        }
        right_message("Заполнение завершилось\n\n");
        out.close();
    }

    pre_sorting();
    if (count < count_elen_in_series) fEnd = true;
    sorting(2);
}
