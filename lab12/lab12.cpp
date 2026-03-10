#include <iostream>
#include <windows.h>
#undef max

enum ACTION_MAIN_MENU {
	NON = -100001, ADD_ELEM_REC = 1, ADD_ELEM_NOT_REC = 2, DELETE_ELEM = 3, CURRENT_STATUS = 4, OUTPUT_SOTRED_TREE = 5, SEARCH_ELEM = 6, CREATE_RANDOM_TREE = 7
};

struct tree {
	tree* left, * right;
	int data, counter;
};

void setConsoleColor(int color) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

bool is_empty(tree * root) {
	if (root == nullptr) {
		setConsoleColor(4);
		std::cout << "\nСписок пуст\n" << std::endl;
		setConsoleColor(7);
		return true;
	}
	else return false;
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
		setConsoleColor(4);
		std::cout << "\nОшибка ввода!\n" << std::endl;
		setConsoleColor(7);
		return false;
	}
}

void back_symmetric_view(tree* locale_root, int depth) {
	if (locale_root != nullptr) {
		back_symmetric_view(locale_root->right, depth + 1);
		for (int i = 0; i < depth; i++) std::cout << "     ";
		std::cout << locale_root->data << std::endl;
		back_symmetric_view(locale_root->left, depth + 1);
	}
}

tree* search_elem(tree* root, int data) {
	tree* temp{ root };

	while (temp != nullptr && temp->data != data) {
		if (data < temp->data) temp = temp->left;
		else if (data > temp->data) temp = temp->right;
	}
	if (temp == nullptr) {
		setConsoleColor(4);
		std::cout << "\nЭлемент не найден\n" << std::endl;
		setConsoleColor(7);
		return nullptr;
	}
	else {
		setConsoleColor(2);
		std::cout << "\nЭлемент найден. Счётчик появления: " << temp->counter << std:: endl << std::endl;
		setConsoleColor(7);
		return temp;
	}
}

tree* add_elem_recursion(tree* locale_root, int data) {
	if (locale_root == nullptr) {
		tree* new_elem = (tree*)malloc(sizeof(tree));
		if (!new_elem) {
			std::cerr << "Ошибка выделения памяти!" << std::endl;
			exit(1);
		}
		new_elem->data = data;
		new_elem->counter = 1;
		new_elem->left = nullptr; new_elem->right = nullptr;
		return new_elem;
	}
	else {
		if (data < locale_root->data) locale_root->left = add_elem_recursion(locale_root->left, data);
		else if (data > locale_root->data) locale_root->right = add_elem_recursion(locale_root->right, data);
		else locale_root->counter += 1;
		return locale_root;
	}
}

tree* add_elem_not_recursion(tree* root, int data) {
	if (root == nullptr) {
		tree* new_elem = (tree*)malloc(sizeof(tree));
		if (!new_elem) {
			std::cerr << "Ошибка выделения памяти!" << std::endl;
			exit(1);
		}
		new_elem->left = nullptr; new_elem->right = nullptr;
		new_elem->data = data; new_elem->counter = 1;
		return new_elem;
	}
	else {
		tree* current{ root }, * parent{ nullptr };
		while (current != nullptr) {
			parent = current;
			if (data < current->data) current = current->left;
			else if (data > current->data) current = current->right;
			else {
				current->counter += 1;
				current = nullptr;
			}
		}
		if (data < parent->data) {
			tree* new_elem = (tree*)malloc(sizeof(tree));
			if (!new_elem) {
				std::cerr << "Ошибка выделения памяти!" << std::endl;
				exit(1);
			}
			new_elem->left = nullptr; new_elem->right = nullptr;
			new_elem->data = data; new_elem->counter = 1;
			parent->left = new_elem;
		}
		else if (data > parent->data) {
			tree* new_elem = (tree*)malloc(sizeof(tree));
			if (!new_elem) {
				std::cerr << "Ошибка выделения памяти!" << std::endl;
				exit(1);
			}
			new_elem->left = nullptr; new_elem->right = nullptr;
			new_elem->data = data; new_elem->counter = 1;
			parent->right = new_elem;
		}
		return root;
	}
}

tree* create_tree(tree* root, int num) {
	for (int i = 0; i < num; i++) {
		root = add_elem_not_recursion(root, std::rand() % 100 + 1);
	}
	return root;
}

void output_sorted_tree(tree* locale_root) {
	if (locale_root != nullptr) {
		output_sorted_tree(locale_root->left);
		std::cout << locale_root->data << "(" << locale_root->counter << ") ";
		output_sorted_tree(locale_root->right);
	}
}

tree* changer(tree* locale_root, tree* vertex_delete) {
	if (locale_root->right != nullptr) {
		locale_root->right = changer(locale_root->right, vertex_delete);
		return locale_root;
	}
	else {
		vertex_delete->data = locale_root->data;
		vertex_delete->counter = locale_root->counter;
		tree* temp{ locale_root->left };
		free(locale_root); locale_root = nullptr;
		return temp;
	}
}

tree* delete_elem(tree* locale_root, int data){
	if (locale_root == nullptr) {
		setConsoleColor(4);
		std::cout << "\nЭлемент не найден\n" << std::endl;
		setConsoleColor(7);
		return nullptr;
	}
	else {
		if (data < locale_root->data) locale_root->left = delete_elem(locale_root->left, data);
		else if (data > locale_root->data) locale_root->right = delete_elem(locale_root->right, data);
		else {
			if (locale_root->counter == 1) {
				tree* temp{ locale_root };
				if (locale_root->left == nullptr) {
					locale_root = locale_root->right;
					free(temp); temp = nullptr;
				}
				else if (locale_root->right == nullptr) {
					locale_root = locale_root->left;
					free(temp); temp = nullptr;
				}
				else {
					locale_root->left = changer(locale_root->left, locale_root);
				}
			}
			else locale_root->counter -= 1;
		}
		return locale_root;
	}
}

void main_menu(tree** root) {
	int ans;

	while (true) {
		std::cout << "Выберите действие:\n1.Добавить элемент (рекурсивно)\n2.Добавить элемент (нерекурсивно)\n3.Удалить элемент\n4.Вывести текущее состояние\n5.Вывод всех вершин в порядке возрастания их ключей\n6.Поиск элемента\n7.Создание дерева с заданным числом вершин со случайными ключами" << std::endl;
		std::cout << "Ожидание ответа: ";
		std::cin >> ans;
		checking_input(ans, 1, 7);

		switch (ans) {
		case ADD_ELEM_REC:
		{
			int data;
			std::cout << "Введите значение новой вершины: ";
			std::cin >> data;
			checking_input(data, -100000, 2147483647);
			if (data == NON) break;

			*root = add_elem_recursion(*root, data);
			setConsoleColor(2);
			std::cout << "\nДобавлено\n" << std::endl;
			setConsoleColor(7);
		}
			break;
		case ADD_ELEM_NOT_REC:
			int data;
			std::cout << "Введите значение новой вершины: ";
			std::cin >> data;
			checking_input(data, -100000, 2147483647);
			if (data == NON) break;

			*root = add_elem_not_recursion(*root, data);
			setConsoleColor(2);
			std::cout << "\nДобавлено\n" << std::endl;
			setConsoleColor(7);
			break;
		case DELETE_ELEM:
			if (!is_empty(*root)) {
				std::cout << "Введите значение удаляемой вершины: ";
				std::cin >> data;
				checking_input(data, -100000, 2147483647);
				if (data == NON) break;

				if (delete_elem(*root, data) != nullptr) {
					setConsoleColor(2);
					std::cout << "\nУдалено\n" << std::endl;
					setConsoleColor(7);
				}
			}
			break;
		case CURRENT_STATUS:
			if (!is_empty(*root)) {
				std::cout << std::endl;
				setConsoleColor(2);
				back_symmetric_view(*root, 0);
				std::cout << std::endl;
				setConsoleColor(7);
			}
			break;
		case OUTPUT_SOTRED_TREE:
			if (!is_empty(*root)) {
				std::cout << std::endl;
				setConsoleColor(2);
				output_sorted_tree(*root);
				std::cout << std::endl << std::endl;
				setConsoleColor(7);
			}
			break;
		case SEARCH_ELEM:
			if (!is_empty(*root)) {
				tree* temp;
				int data;
				std::cout << "Введите значение, которое нужно найти: ";
				std::cin >> data;
				checking_input(data, -100000, 2147483647);
				if (data == NON) break;

				search_elem(*root, data);
			}
			break;
		case CREATE_RANDOM_TREE:
			if (*root == nullptr) {
				int num;
				std::cout << "Введите количество вершин дерева: ";
				std::cin >> num;
				checking_input(num, 1, 2147483647);
				if (num == NON) break;
				*root = create_tree(*root, num);
				setConsoleColor(2);
				std::cout << "\nДерево взрощено\n" << std::endl;
				setConsoleColor(7);
			}
			else {
				setConsoleColor(4);
				std::cout << "\nДерево должно быть пустым для вызова этой операции\n" << std::endl;
				setConsoleColor(7);
			}
			break;
		}

	}
}

int main()
{
	setlocale(LC_ALL, "ru");
	srand(std::time(0));

	tree* root{ nullptr };

	main_menu(&root);
}