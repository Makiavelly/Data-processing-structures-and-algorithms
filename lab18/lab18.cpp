#include <iostream>
#include <windows.h>
#undef max
#include <string>

enum ACTION_MENU{ NON = -1, ADD_ELEM = 1, DELETE_ELEM = 2, STATUS = 3, SEARCH = 4 };

struct list {
    list* next;
    std::string data;
};

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

bool add_elem(list* head, std::string data, int& compare) {
    list* temp{ head };
    list* new_elem{ new list };
    if (!new_elem) {
        std::cerr << "Ошибка выделения памяти!" << std::endl;
        exit(1);
    }
    new_elem->data = data;
    new_elem->next = nullptr;
    while (temp->next != nullptr) {
        compare++;
        if (temp->next->data == data) {
            wrong_message("\n\nТакой элемент уже есть\n");
            delete new_elem;
            return false;
        }
        temp = temp->next;
    }
    temp->next = new_elem;
    return true;
}

void output_table(list* arr, int m = 5) {
    setConsoleColor(2); std::cout << std::endl;
    for (int i = 0; i < m; i++) {
        std::cout << "Индекс " << i << ": ";
        list* temp{ arr[i].next };
        while (temp != nullptr) {
            std::cout << temp->data << " ";
            temp = temp->next;
        }
        std::cout << std::endl;
    }
    setConsoleColor(7); std::cout << std::endl;
}

int calc_hash(std::string* word, int m = 5) {
    int hash_sum{ 0 };
    for (char letter : *word) hash_sum += (int)letter;
    hash_sum = hash_sum % m;
    return hash_sum;
}

void input_word(list* arr, int& compare) {
    std::string* word{ new std::string };
    if (!word) {
        std::cerr << "Ошибка выделения памяти!" << std::endl;
        exit(1);
    }
    std::cout << "Ожидание ввода: ";
    std::cin >> *word;
    if (add_elem(&arr[calc_hash(word)], *word, compare)) right_message("\n\nДобавлено!\n");
    else delete word;
}

list* search_elem(list* arr, std::string& word, int& compare, int m = 5) {
    int index = calc_hash(&word, m);
    list* temp{ &arr[index] };
    while (temp->next != nullptr) {
        compare++;
        if (temp->next->data == word) return temp;
        temp = temp->next;
    }
    return nullptr;
}

bool delete_elem(list* arr, int& compare) {
    std::string word;
    std::cout << "Введите элемент, который хотите удалить: ";
    std::cin >> word;
    list* founded_elem{ search_elem(arr, word, compare) };
    if (founded_elem == nullptr) {
        wrong_message("\nЭлемент не найден\n");
        return false;
    }
    else {
        setConsoleColor(2);
        std::cout << "\nЭлемент найден под индексом " << calc_hash(&word) << std::endl;
        setConsoleColor(7);
        list* delete_elem{ founded_elem->next };
        founded_elem->next = delete_elem->next;
        delete delete_elem;
        return true;
    }
}

int main()
{
    setlocale(LC_ALL, "ru");

    int ans;

    std::cout << "Отладка?\n1.Да\n2.Нет\nОжидание ввода: ";
    std::cin >> ans;
    checking_input(ans, 1, 2);
    if (ans == 1) {
        list hash_table[5];
        int compare{ 0 };
        for (int i = 0; i < 5; i++) hash_table[i].next = nullptr;
        while (true) {

            std::cout << "Выберите действие:\n1.Добавить элемент\n2.Удалить элемент\n3.Вывести текущее состояние\n4.Поиск элемента" << std::endl;
            std::cout << "Ожидание ответа: ";
            std::cin >> ans;
            checking_input(ans, 1, 4);

            switch (ans) {
            case ADD_ELEM:
                input_word(hash_table, compare);
                std::cout << "Кол-во сравнений: " << compare << "\n\n";
                compare = 0;
                break;
            case DELETE_ELEM:
                if (delete_elem(hash_table, compare)) right_message("Удалено!\n");
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
                if (search_elem(hash_table, word, compare) == nullptr) wrong_message("\nЭлемент не найден\n");
                else {
                    setConsoleColor(2);
                    std::cout << "\nЭлемент найден под индексом " << calc_hash(&word) << std::endl;
                    setConsoleColor(7);
                }
                std::cout << "Кол-во сравнений: " << compare << "\n\n";
                compare = 0;
                break;
            }
        }
    }
    else {
        const int a{ 9 }, b{ 17 }, c{ 23 };
        int compare_add_a{ 0 }, compare_add_b{ 0 }, compare_add_c{ 0 };
        int compare_search_a{ 0 }, compare_search_b{ 0 }, compare_search_c{ 0 };
        list hash_table_a[a];
        for (int i = 0; i < a; i++) hash_table_a[i].next = nullptr;
        list hash_table_b[b];
        for (int i = 0; i < b; i++) hash_table_b[i].next = nullptr;
        list hash_table_c[c];
        for (int i = 0; i < c; i++) hash_table_c[i].next = nullptr;

        std::string keys[20]{ "sasha", "olya", "misha", "vasya", "lena", "nadya", "tanya", "sergey", "egor", "dima", "artem", "vlad", "fedor", "liza", "kirill", "kamil", "rashid", "matvey", "nastya", "ksusha" };
        for (std::string key : keys) {
            add_elem(&hash_table_a[calc_hash(&key, a)], key, compare_add_a);
            add_elem(&hash_table_b[calc_hash(&key, b)], key, compare_add_b);
            add_elem(&hash_table_c[calc_hash(&key, c)], key, compare_add_c);
        }

        output_table(hash_table_a, a);
        output_table(hash_table_b, b);
        output_table(hash_table_c, c);

        for (std::string key : keys) {
            int compare_temp_a{ 0 }, compare_temp_b{ 0 }, compare_temp_c{ 0 };
            std::cout << "\nКлюч " << key << " можно найти за:";
            search_elem(hash_table_a, key, compare_temp_a, a);
            compare_search_a += compare_temp_a;
            std::cout << "\n" << a << " - " << compare_temp_a << " сравнений";
            search_elem(hash_table_b, key, compare_temp_b, b);
            compare_search_b += compare_temp_b;
            std::cout << "\n" << b << " - " << compare_temp_b << " сравнений";
            search_elem(hash_table_c, key, compare_temp_c, c);
            compare_search_c += compare_temp_c;
            std::cout << "\n" << c << " - " << compare_temp_c << " сравнений";
            std::cout << std::endl;
        }
        std::cout << "Суммарное количество сравнений для размещения 20 ключей в хеш таблице размером:\n" << a << " - " << compare_add_a << "\n" << b << " - " << compare_add_b << "\n" << c << " - " << compare_add_c << std::endl;
        std::cout << "Суммарное количество сравнений для поиска 20 ключей в хеш таблице размером:\n" << a << " - " << compare_search_a << "\n" << b << " - " << compare_search_b << "\n" << c << " - " << compare_search_c << std::endl;

    }
}