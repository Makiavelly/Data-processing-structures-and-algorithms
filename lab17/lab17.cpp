#include <iostream>
#include <windows.h>
#undef max
#include <string>

enum ACTION_MENU { NON = -1, ADD_ELEM = 1, STATUS = 2, SEARCH = 3 };

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

void output_table(std::string** arr, int m = 10) {
    setConsoleColor(2); std::cout << std::endl;
    for (int i = 0; i < m; i++) 
        std::cout << "Индекс " << i << ": " << (arr[i] == nullptr ? " " : *arr[i]) << std::endl;
    setConsoleColor(7); std::cout << std::endl;
}

int calc_hash(std::string* word, int m) {
    int hash_sum{ 0 };
    for (char letter : *word) hash_sum += (int)letter;
    hash_sum = hash_sum % m;
    return hash_sum;
}

bool add_elem(std::string** arr, std::string* word, int& compare, int m) {
    int index{ calc_hash(word, m) }, i{ 0 };
    std::string** temp{ &arr[index] };
    do {
        if (*temp == nullptr) {
            *temp = word;
            return true;
        }
        compare++;
        if (**temp == *word) {
            wrong_message("\n\nТакой элемент уже есть\n");
            return false;
        }
        temp = &arr[(index + (++i)) % m];
    } while (*temp != arr[index]);
    wrong_message("\n\nСвободных ячеек нет\n");
    return false;
}

void input_word(std::string** arr, int& compare, int m = 10) {
    std::string* word{ new std::string };
    if (!word) {
        std::cerr << "Ошибка выделения памяти!" << std::endl;
        exit(1);
    }
    std::cout << "Ожидание ввода: ";
    std::cin >> *word;
    if (add_elem(arr, word, compare, m)) right_message("\n\nДобавлено!\n");
    else delete word;
}

int search_elem(std::string** arr, std::string* word, int& compare, int m = 10) {
    int index = calc_hash(word, m), i{ 0 };
    std::string** temp{ &arr[index] };
    do {
        if (*temp == nullptr) return NON;
        compare++;
        if (**temp == *word) return (index + i) % m;
        temp = &arr[(index + (++i)) % m];
    } while (*temp != arr[index]);
    return NON;
}

int main()
{
    setlocale(LC_ALL, "ru");

    int ans;

    std::cout << "Отладка?\n1.Да\n2.Нет\nОжидание ввода: ";
    std::cin >> ans;
    checking_input(ans, 1, 2);
    if (ans == 1) {
        std::string* hash_table[10]{ nullptr };
        int compare{ 0 };
        while (true) {

            std::cout << "Выберите действие:\n1.Добавить элемент\n2.Вывести текущее состояние\n3.Поиск элемента" << std::endl;
            std::cout << "Ожидание ответа: ";
            std::cin >> ans;
            checking_input(ans, 1, 3);

            switch (ans) {
            case ADD_ELEM:
                input_word(hash_table, compare);
                std::cout << "Кол-во сравнений: " << compare << "\n\n";
                compare = 0;
                break;
            case STATUS:
                output_table(hash_table);
                break;
            case SEARCH:
                std::string word;
                std::cout << "Введите элемент, который хотите найти: ";
                std::cin >> word;
                int index{ search_elem(hash_table, &word, compare) };
                if (index == NON) wrong_message("\nКлюч не найден\n");
                else {
                    setConsoleColor(2);
                    std::cout << "\nКлюч найден под индексом " << index << std::endl;
                    setConsoleColor(7);
                }
                std::cout << "Кол-во сравнений: " << compare << "\n\n";
                compare = 0;
                break;
            }
        }
    }
    else {
        const int a{ 11 }, b{ 13 }, c{ 17 };
        int compare_add_a{ 0 }, compare_add_b{ 0 }, compare_add_c{ 0 };
        int compare_search_a{ 0 }, compare_search_b{ 0 }, compare_search_c{ 0 };
        std::string* hash_table_a[a]{ nullptr };
        std::string* hash_table_b[b]{ nullptr };
        std::string* hash_table_c[c]{ nullptr };

        std::string keys[10]{ "sasha", "olya", "misha", "vasya", "lena", "nadya", "tanya", "sergey", "egor", "dima" };
        for (int i = 0; i < 10; i++) {
            add_elem(hash_table_a, &keys[i], compare_add_a, a);
            add_elem(hash_table_b, &keys[i], compare_add_b, b);
            add_elem(hash_table_c, &keys[i], compare_add_c, c);
        }

        output_table(hash_table_a, a);
        output_table(hash_table_b, b);
        output_table(hash_table_c, c);

        for (int i = 0; i < 10; i++) {
            int compare_temp_a = 0, compare_temp_b = 0, compare_temp_c = 0;
            std::cout << "\nКлюч " << keys[i] << " можно найти за:";
            search_elem(hash_table_a, &keys[i], compare_temp_a, a);
            compare_search_a += compare_temp_a;
            std::cout << "\n" << a << " - " << compare_temp_a << " сравнений";
            search_elem(hash_table_b, &keys[i], compare_temp_b, b);
            compare_search_b += compare_temp_b;
            std::cout << "\n" << b << " - " << compare_temp_b << " сравнений";
            search_elem(hash_table_c, &keys[i], compare_temp_c, c);
            compare_search_c += compare_temp_c;
            std::cout << "\n" << c << " - " << compare_temp_c << " сравнений";
            std::cout << std::endl;
        }
        std::cout << "Суммарное количество сравнений для размещения 20 ключей в хеш таблице размером:\n" << a << " - " << compare_add_a << "\n" << b << " - " << compare_add_b << "\n" << c << " - " << compare_add_c << std::endl;
        std::cout << "Суммарное количество сравнений для поиска 20 ключей в хеш таблице размером:\n" << a << " - " << compare_search_a << "\n" << b << " - " << compare_search_b << "\n" << c << " - " << compare_search_c << std::endl;

    }
}