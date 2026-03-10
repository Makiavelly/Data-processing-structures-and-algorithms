#include <iostream>
#include <windows.h>
#undef max


const int m = 2;

enum ACTION_MENU {
    NON = -100001, ADD_VERT = 1, DELETE_VERT = 2, SEARCH = 3, STATUS = 4, CREATE = 5
};

struct page;
struct elem {
    int data;
    page* child_page;
};

struct page {
    int num_elem;
    page* left_page;
    elem** arr_elem;
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

elem** add_elem(page* current, bool& upgrade, elem** new_elem) {
    if (current == nullptr) { upgrade = true; return new_elem; }
    else {
        elem** temp;
        if (current->arr_elem[0]->data > (*new_elem)->data) {
            temp = add_elem(current->left_page, upgrade, new_elem);
            if (temp != new_elem) {
                *new_elem = *temp;
                delete temp; temp = nullptr;
            }
        }
        else {
            for (int i = current->num_elem - 1; i > -1; i--) {
                if ((*new_elem)->data > current->arr_elem[i]->data) {
                    temp = add_elem(current->arr_elem[i]->child_page, upgrade, new_elem);
                    if (temp != new_elem) {
                        *new_elem = *temp;
                        delete temp; temp = nullptr;
                    }
                    break;
                }
                else if ((*new_elem)->data == current->arr_elem[i]->data) {
                    delete *new_elem;
                    (*new_elem) = nullptr;
                    return new_elem;
                }
                else continue;
            }
        }

        if (upgrade) {
            if (current->num_elem == 2 * m) {
                page* new_page{ new page };
                if (!new_page) {
                    std::cerr << "Ошибка выделения памяти!" << std::endl;
                    exit(1);
                }
                new_page->arr_elem = new elem*[2 * m + 1];
                for (int i = 0; i < 2 * m; i++) new_page->arr_elem[i] = nullptr;
                new_page->left_page = nullptr;
                new_page->num_elem = 2;

                for (int i = 2 * m - 1; i > -1; i--) {
                    if ((*new_elem)->data > current->arr_elem[i]->data) {
                        current->arr_elem[i + 1] = *new_elem;
                        break;
                    }
                    else {
                        current->arr_elem[i + 1] = current->arr_elem[i];
                        current->arr_elem[i] = nullptr;
                    }
                    
                }
                if (current->arr_elem[0] == nullptr) current->arr_elem[0] = *new_elem;

                for (int i = m + 1; i < 2 * m + 1; i++) {
                    new_page->arr_elem[i - m - 1] = current->arr_elem[i];
                    current->arr_elem[i] = nullptr;
                }
                new_page->left_page = current->arr_elem[m]->child_page;
                current->arr_elem[m]->child_page = new_page;
                elem** wrap_temp{ new elem* };
                *wrap_temp = current->arr_elem[m];
                current->arr_elem[m] = nullptr;
                current->num_elem -= 2;
                return wrap_temp;
            }
            else {
                for (int i = current->num_elem - 1; i > -1; i--) {
                    if ((*new_elem)->data > current->arr_elem[i]->data) {
                        current->arr_elem[i + 1] = *new_elem;
                        break;
                    }
                    else {
                        current->arr_elem[i + 1] = current->arr_elem[i];
                        current->arr_elem[i] = nullptr;
                    }
                    
                }
                if (current->arr_elem[0] == nullptr) current->arr_elem[0] = *new_elem;
                current->num_elem++;
                upgrade = false;
                return new_elem;
            }
        }
        else return new_elem;
    }
}

bool wrapper_add_func(page** root, int data) {
    bool upgrade{ false };
    elem* new_elem{ new elem }, **pressed_elem;
    if (!new_elem) {
        std::cerr << "Ошибка выделения памяти!" << std::endl;
        exit(1);
    }
    new_elem->child_page = nullptr;
    new_elem->data = data;

    pressed_elem = add_elem(*root, upgrade, &new_elem);
    if (*pressed_elem == nullptr) return false;
    else {
        if (upgrade) {
            page* new_page{ new page };
            if (!new_page) {
                std::cerr << "Ошибка выделения памяти!" << std::endl;
                exit(1);
            }
            new_page->arr_elem = new elem * [2 * m + 1];
            for (int i = 0; i < 2 * m; i++) new_page->arr_elem[i] = nullptr;
            new_page->left_page = *root;
            *root = new_page;
            new_page->arr_elem[0] = *pressed_elem;
            new_page->num_elem = 1;
        }
        return true;
    }
}

void upgrading(page* current, page* parent, int ind_parent, bool& upgrade) {
    page* temp;
    if (ind_parent == -1) {
        if (parent->arr_elem[0] != nullptr && parent->arr_elem[0]->child_page->num_elem > m) {
            ind_parent++;
            elem* temp{ parent->arr_elem[ind_parent] };
            page* parent_child{ parent->arr_elem[ind_parent]->child_page };
            page* child{ parent_child->arr_elem[0]->child_page };

            parent->arr_elem[ind_parent] = parent_child->arr_elem[0];
            parent->arr_elem[ind_parent]->child_page = temp->child_page;

            parent_child->num_elem--;
            for (int i = 0; i < parent_child->num_elem; i++)
                parent_child->arr_elem[i] = parent_child->arr_elem[i + 1];
            parent_child->arr_elem[parent_child->num_elem] = nullptr;

            current->arr_elem[current->num_elem] = temp;
            temp->child_page = parent_child->left_page;
            parent_child->left_page = child;
            current->num_elem++;
            upgrade = false;
        }
        else {
            ind_parent++;
            page* parent_child{ parent->arr_elem[ind_parent]->child_page };

            current->arr_elem[current->num_elem] = parent->arr_elem[ind_parent];
            current->arr_elem[current->num_elem]->child_page = parent_child->left_page;
            current->num_elem++;
            for (int i = 0; i < current->num_elem; i++)
                current->arr_elem[current->num_elem + i] = parent_child->arr_elem[i];
            current->num_elem += parent_child->num_elem;
            delete[] parent_child->arr_elem; delete parent_child; parent_child = nullptr;

            parent->num_elem--;
            for (int i = ind_parent; i < parent->num_elem; i++)
                parent->arr_elem[i] = parent->arr_elem[i + 1];
            parent->arr_elem[parent->num_elem] = nullptr;
            if (parent->num_elem < m) upgrade = true;
            else upgrade = false;
        }
    }
    else if (ind_parent == 0) {
        if (parent->left_page != nullptr && parent->left_page->num_elem > m) {
            elem* temp{ parent->arr_elem[ind_parent] };
            page* parent_child{ parent->left_page };
            page* child{ parent_child->arr_elem[parent_child->num_elem - 1]->child_page };

            parent_child->arr_elem[parent_child->num_elem - 1]->child_page = temp->child_page;
            parent->arr_elem[ind_parent] = parent_child->arr_elem[parent_child->num_elem - 1];
            parent_child->arr_elem[parent_child->num_elem - 1] = nullptr;
            parent_child->num_elem--;

            for (int i = current->num_elem; i > 0; i--)
                current->arr_elem[i] = current->arr_elem[i - 1];
            current->arr_elem[0] = temp;
            temp->child_page = current->left_page;
            current->left_page = child;
            current->num_elem++;
            upgrade = false;
        }
        else if (parent->arr_elem[1] != nullptr && parent->arr_elem[1]->child_page->num_elem > m) {
            elem* temp{ parent->arr_elem[ind_parent] };
            page* parent_child{ parent->arr_elem[ind_parent + 1]->child_page };
            page* child{ parent_child->arr_elem[0]->child_page };

            parent->arr_elem[ind_parent + 1]->child_page = parent_child->left_page;
            parent_child->left_page = parent_child->arr_elem[0]->child_page;
            parent_child->arr_elem[0]->child_page = parent_child;
            parent->arr_elem[0]->child_page->arr_elem[m - 1] = parent->arr_elem[ind_parent + 1];
            parent->arr_elem[ind_parent + 1] = parent_child->arr_elem[0];

            parent_child->num_elem--;
            for (int i = 0; i < parent_child->num_elem; i++) 
                parent_child->arr_elem[i] = parent_child->arr_elem[i + 1];
            parent_child->arr_elem[parent_child->num_elem] = nullptr;
               
            current->num_elem++;
            upgrade = false;
        }
        else {
            page* parent_child{ parent->left_page };

            parent_child->arr_elem[parent_child->num_elem] = parent->arr_elem[ind_parent];
            parent_child->arr_elem[parent_child->num_elem]->child_page = current->left_page;
            parent_child->num_elem++;
            for (int i = 0; i < current->num_elem; i++)
                parent_child->arr_elem[parent_child->num_elem + i] = current->arr_elem[i];
            parent_child->num_elem += current->num_elem;
            delete[] current->arr_elem; delete current; current = nullptr;

            parent->num_elem--;
            for (int i = ind_parent; i < parent->num_elem; i++)
                parent->arr_elem[i] = parent->arr_elem[i + 1];
            parent->arr_elem[parent->num_elem] = nullptr;
            if (parent->num_elem < m) upgrade = true;
            else upgrade = false;
        }
    }
    else if (ind_parent == parent->num_elem - 1) {
        if (parent->arr_elem[ind_parent - 1] != nullptr && parent->arr_elem[ind_parent - 1]->child_page->num_elem > m) {
            elem* temp{ parent->arr_elem[ind_parent] };
            page* parent_child{ parent->arr_elem[ind_parent - 1]->child_page};
            page* child{ parent_child->arr_elem[parent_child->num_elem - 1]->child_page };

            parent_child->arr_elem[parent_child->num_elem - 1]->child_page = temp->child_page;
            parent->arr_elem[ind_parent] = parent_child->arr_elem[parent_child->num_elem - 1];
            parent_child->arr_elem[parent_child->num_elem - 1] = nullptr;
            parent_child->num_elem--;

            for (int i = current->num_elem; i > 0; i--)
                current->arr_elem[i] = current->arr_elem[i - 1];
            current->arr_elem[0] = temp;
            temp->child_page = current->left_page;
            current->left_page = child;
            current->num_elem++;
            upgrade = false;
        }
        else {
            page* parent_child{ parent->arr_elem[ind_parent - 1]->child_page };

            parent_child->arr_elem[parent_child->num_elem] = parent->arr_elem[ind_parent];
            parent_child->arr_elem[parent_child->num_elem]->child_page = current->left_page;
            parent_child->num_elem++;
            for (int i = 0; i < current->num_elem; i++)
                parent_child->arr_elem[parent_child->num_elem + i] = current->arr_elem[i];
            parent_child->num_elem += current->num_elem;
            delete[] current->arr_elem; delete current; current = nullptr;

            parent->num_elem--;
            parent->arr_elem[parent->num_elem] = nullptr;
            if (parent->num_elem < m) upgrade = true;
            else upgrade = false;
        }
    }
    else {
        if (parent->arr_elem[ind_parent - 1]->child_page->num_elem > m) {
            elem* temp{ parent->arr_elem[ind_parent] };
            page* parent_child{ parent->arr_elem[ind_parent - 1]->child_page };
            page* child{ parent_child->arr_elem[parent_child->num_elem - 1]->child_page };

            parent_child->arr_elem[parent_child->num_elem - 1]->child_page = temp->child_page;
            parent->arr_elem[ind_parent] = parent_child->arr_elem[parent_child->num_elem - 1];
            parent_child->arr_elem[parent_child->num_elem - 1] = nullptr;
            parent_child->num_elem--;

            for (int i = current->num_elem; i > 0; i--)
                current->arr_elem[i] = current->arr_elem[i - 1];
            current->arr_elem[0] = temp;
            temp->child_page = current->left_page;
            current->left_page = child;
            current->num_elem++;
            upgrade = false;
        }
        else if (parent->arr_elem[ind_parent + 1]->child_page->num_elem > m) {
            elem* temp{ parent->arr_elem[ind_parent] };
            page* parent_child{ parent->arr_elem[ind_parent + 1]->child_page };
            page* child{ parent_child->arr_elem[0]->child_page };

            parent->arr_elem[ind_parent + 1]->child_page = parent_child->left_page;
            parent_child->left_page = parent_child->arr_elem[0]->child_page;
            parent_child->arr_elem[0]->child_page = parent_child;
            parent->arr_elem[ind_parent]->child_page->arr_elem[m - 1] = parent->arr_elem[ind_parent + 1];
            parent->arr_elem[ind_parent + 1] = parent_child->arr_elem[0];

            parent_child->num_elem--;
            for (int i = 0; i < parent_child->num_elem; i++)
                parent_child->arr_elem[i] = parent_child->arr_elem[i + 1];
            parent_child->arr_elem[parent_child->num_elem] = nullptr;

            current->num_elem++;
            upgrade = false;
        }
        else {
            page* parent_child{ parent->arr_elem[ind_parent - 1]->child_page };

            parent_child->arr_elem[parent_child->num_elem] = parent->arr_elem[ind_parent];
            parent_child->arr_elem[parent_child->num_elem]->child_page = current->left_page;
            parent_child->num_elem++;
            for (int i = 0; i < current->num_elem; i++) 
                parent_child->arr_elem[parent_child->num_elem + i] = current->arr_elem[i];
            parent_child->num_elem += current->num_elem;
            delete[] current->arr_elem; delete current; current = nullptr;

            parent->num_elem--;
            for (int i = ind_parent; i < parent->num_elem; i++)
                parent->arr_elem[i] = parent->arr_elem[i + 1];
            parent->arr_elem[parent->num_elem] = nullptr;
            if (parent->num_elem < m) upgrade = true;
            else upgrade = false;
        }
    }
}

void changer(page* current, bool& upgrade, elem* target) {
    elem* temp{ current->arr_elem[current->num_elem - 1] };
    if (temp->child_page == nullptr) {
        target->data = temp->data;
        delete temp; temp = nullptr;
        current->arr_elem[current->num_elem - 1] = nullptr;
        current->num_elem--;
        if (current->num_elem < m) upgrade = true;
    }
    else {
        changer(temp->child_page, upgrade, target);
        if (upgrade) upgrading(temp->child_page, current, current->num_elem - 1, upgrade);
    }
}

void delete_elem(page* current, bool& upgrade, int data, bool& was_found) {
    if (current == nullptr) was_found = false;
    else {
        if (current->arr_elem[0]->data > data) {
            delete_elem(current->left_page, upgrade, data, was_found);
            if (upgrade) upgrading(current->left_page, current, -1, upgrade);
        }
        else {
            for (int i = current->num_elem - 1; i > -1; i--) {
                if (data > current->arr_elem[i]->data) {
                    delete_elem(current->arr_elem[i]->child_page, upgrade, data, was_found);
                    if (upgrade) upgrading(current->arr_elem[i]->child_page, current, i, upgrade);
                    break;
                }
                else if (data == current->arr_elem[i]->data) {
                    was_found = true;
                    if (current->left_page == nullptr) {
                        delete current->arr_elem[i];
                        current->arr_elem[i] = nullptr;
                        current->num_elem--;
                        for (int j = i; j < current->num_elem; j++) 
                            current->arr_elem[j] = current->arr_elem[j + 1];
                        current->arr_elem[current->num_elem] = nullptr;
                        if (current->num_elem < m) upgrade = true;
                    }
                    else {
                        if (i == 0) {
                            changer(current->left_page, upgrade, current->arr_elem[0]);
                            if (upgrade) upgrading(current->left_page, current, -1, upgrade);
                        }
                        else {
                            changer(current->arr_elem[i - 1]->child_page, upgrade, current->arr_elem[i]);
                            if (upgrade) upgrading(current->arr_elem[i - 1]->child_page, current, i - 1, upgrade);
                        }
                    }
                    break;
                }
                else continue;
            }
        }
    }
}

void search_elem(page* root, int data, bool& was_found) {
    if (root == nullptr) was_found = false;
    else {
        if (root->arr_elem[0]->data > data) {
            search_elem(root->left_page, data, was_found);
            if (was_found) std::cout << "-->" << root->arr_elem[0]->data;
        }
        else {
            for (int i = root->num_elem - 1; i > -1; i--) {
                if (data > root->arr_elem[i]->data) {
                    search_elem(root->arr_elem[i]->child_page, data, was_found);
                    if (was_found) std::cout << "-->" << root->arr_elem[i]->data;
                    break;
                }
                else if (data == root->arr_elem[i]->data) {
                    was_found = true;
                    std::cout << data;
                    break;
                }
                else continue;
            }
        }
    }
}

void current_status(page* root, int depth) {
    if (root->left_page == nullptr) {
        for (int i = 0; i < root->num_elem; i++) {
            for (int j = 0; j < depth; j++) std::cout << "     ";
            std::cout << root->arr_elem[i]->data << std::endl;
        }
    }
    else {
        current_status(root->left_page, depth + 1);
        for (int i = 0; i < root->num_elem; i++) {
            for (int j = 0; j < depth; j++) std::cout << "     ";
            std::cout << root->arr_elem[i]->data << " ";
            std::cout << std::endl;
            current_status(root->arr_elem[i]->child_page, depth + 1);
           
        }
    }
}

int main()
{
    setlocale(LC_ALL, "ru");
    //time_t time{ std::time(0) };
    std::srand(1743244263);
    //std::cout << time << std::endl;
    //1743244263
    page* root{ nullptr };

    int ans;
    while (true) {

        std::cout << "Выберите действие:\n1.Добавить элемент\n2.Удалить элемент\n3.Поиск элемента\n4.Вывести текущее состояние\n5.Создать дерево" << std::endl;
        std::cout << "Ожидание ответа: ";
        std::cin >> ans;
        checking_input(ans, 1, 5);

        switch (ans) {
        case ADD_VERT:
        {
            
            int data;
            std::cout << "Введите ключ добавляемого элемента: ";
            std::cin >> data;
            checking_input(data, -100000, 2147483647);
            if (data == NON) break;
            else {
                if (wrapper_add_func(&root, data)) right_message("\nДобавлено!\n\n");
                else wrong_message("\n\nТакой элемент уже есть\n\n");
            }
        }
            break;
        case DELETE_VERT:
            if (root == nullptr) wrong_message("\nДерево пусто\n\n");
            else {
                int data;
                bool upgrade{ false };
                std::cout << "Введите ключ удаляемого элемента: ";
                std::cin >> data;
                checking_input(data, -100000, 2147483647);
                if (data == NON) break;
                else {
                    bool was_found{ false };
                    delete_elem(root, upgrade, data, was_found);
                    if (root->num_elem == 0) {
                        page* temp{ root };
                        root = root->left_page;
                        delete temp; temp = nullptr;
                    }
                    if (was_found) right_message("\nУдалено\n\n");
                    else wrong_message("\nЭлемента с таким ключом нет\n\n");
                }
            }
            break;
        case STATUS:
            if (root == nullptr) wrong_message("\nДерево пусто\n\n");
            else {
                setConsoleColor(2); std::cout << std::endl;
                current_status(root, 0);
                setConsoleColor(7); std::cout << std::endl;
            }
            break;
        case SEARCH:
            int data;
            std::cout << "Введите ключ элемента: ";
            std::cin >> data;
            checking_input(data, -100000, 2147483647);
            if (data == NON) break;
            else {
                bool was_found{ false };
                search_elem(root, data, was_found);
                if (was_found) right_message("\nНайден!\n\n");
                else wrong_message("\nНе найден\n\n");
            }
            break;
        case CREATE: 
            int count;
            std::cout << "Введите количество вершин: ";
            std::cin >> count;
            checking_input(count, 1, 2147483647);
            if (count == NON) break;

            int i = 0;
            while (i < count)
                if (wrapper_add_func(&root, std::rand() % 301)) i++;

            right_message("\nДерево взрощено!\n\n"); 
            break;
        }
    }
}