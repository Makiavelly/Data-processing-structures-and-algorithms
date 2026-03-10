#include <iostream>
#include <windows.h>
#undef max
#include <string>
#define NON -1

void setConsoleColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void output_table(std::string** arr) {
    setConsoleColor(2); std::cout << std::endl;
    for (int i = 0; i < 10; i++) 
        std::cout << *(arr[i]) << " ";
    setConsoleColor(7); std::cout << std::endl;
}

void input_words(std::string** arr) {
    for (int i = 0; i != 10; i++) {
        std::cout << "Введено " << i << " слов: ";

        std::string* word{ new std::string };
        if (!word) {
            std::cerr << "Ошибка выделения памяти!" << std::endl;
            exit(1);
        }
        int hash_sum;

        while (true) {
            hash_sum = 0;
            std::cin >> *word;

            for (char letter : *word) hash_sum += (int)letter;
            hash_sum = hash_sum % 10;
            if (arr[hash_sum] == nullptr) {
                arr[hash_sum] = word;
                break;
            }
            else if (*arr[hash_sum] == *word) {
                setConsoleColor(2);
                std::cout << "\nЭтот ключ уже хранится в таблице!\n";
                std::cout << "Пожалуйста, напишите другое слово: ";
                setConsoleColor(7);
            }
            else {
                setConsoleColor(4);
                std::cout << "\nКонфликт! Пожалуйста, напишите другое слово: ";
                setConsoleColor(7);
            }
        }
    }

    setConsoleColor(2);
    std::cout << "\nХэш-таблица заполнена!\n\n";
    setConsoleColor(7);
}

int search_elem(std::string** arr, std::string* word) {
    int hash_sum{ 0 };
    for (char letter : *word) hash_sum += (int)letter;
    hash_sum = hash_sum % 10;
    if (*arr[hash_sum] == *word) return hash_sum;
    else return NON;
}

int main()
{
    //ADC FOR LOW VAR GET END OPEN left OUT POP
    setlocale(LC_ALL, "ru");

    std::string* hash_table[10]{ nullptr };


    std::cout << "Введите 10 служебных слов языка Паскаль" << std::endl;
    input_words(hash_table);

    output_table(hash_table);

    std::string word;
    int index;
    while (true) {
        std::cout << "Введите элемент, который хотите найти: ";
        std::cin >> word;
        index = search_elem(hash_table, &word);
        if (index == NON) {
            setConsoleColor(4);
            std::cout << "\nЭлемент не найден\n\n";
            setConsoleColor(7);
        }
        else {
            setConsoleColor(2);
            std::cout << "\nЭлемент найден под индексом " << index << '\n';
            setConsoleColor(7);
        }
    }
}