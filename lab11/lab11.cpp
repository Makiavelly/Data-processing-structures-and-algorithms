#include <iostream>
#include <windows.h>
#undef max

enum ACTION_MAIN_MENU {
	NON = - 100001, ADD_CELL = 1, DELETE_TREE = 2,CURRENT_STATUS = 3, SEARCH_ELEM = 4
};

struct tree {
	tree* left, * right;
	int data;
};

void setConsoleColor(int color) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
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

void symmetric_view(tree* locale_root, int depth) {
	if (locale_root != nullptr) {
		symmetric_view(locale_root->left, depth + 1);
		for (int i = 0; i < depth; i++) std::cout << "     ";
		std::cout << locale_root->data << std::endl;
		symmetric_view(locale_root->right, depth + 1);
	}
}

bool is_empty(tree* root) {
	if (root == nullptr) {
		setConsoleColor(4);
		std::cout << "\nСписок пуст\n" << std::endl;
		setConsoleColor(7);
		return true;
	}
	else return false;
}

tree* search_elem(tree* root, int data) {
	if (root == nullptr) return nullptr;
	else if (root->data == data) {
		std::cout << root->data;
		return root;
	}
	else {
		tree* temp;
		temp = search_elem(root->left, data);
		if (temp != nullptr) {
			std::cout << "-->" << root->data;
			return temp;
		}
		else {
			temp = search_elem(root->right, data);
			if (temp != nullptr) {
				std::cout << "-->" << root->data;
				return temp;
			}
			else return nullptr;
		}
	}
}

tree* add_cell(tree* root, int data) {
	if (root == nullptr) {
		tree* new_cell = (tree*)malloc(sizeof(tree));
		if (!new_cell) {
			std::cerr << "Ошибка выделения памяти!" << std::endl;
			exit(1);
		}
		new_cell->data = data;
		new_cell->left = nullptr;
		new_cell->right = nullptr;

		setConsoleColor(2);
		std::cout << "\nДобавлено\n" << std::endl;
		setConsoleColor(7);
		return new_cell;
	}
	else {
		int value;
		tree* temp;

		std::cout << "Введите значение вершины, потомком которой надо сделать новую вершину: ";
		std::cin >> value;
		checking_input(value, -100000, 2147483647);
		if (value == NON) return root;

		std::cout << std::endl;
		temp = search_elem(root, value);
		if (temp == nullptr) {
			setConsoleColor(4);
			std::cout << "\nЭлемент не найден\n" << std::endl;
			setConsoleColor(7);
			return root;
		}
		else {
			std::cout << std::endl << std::endl;
			if (temp->left == nullptr) {
				if (temp->right == nullptr) {
					int side;
					std::cout << "Сделать левым или правым потомком:\n1.Левым\n2.Правым\nОжидание ответа: ";
					std::cin >> side;
					checking_input(side, 1, 2);
					if (side == NON) return root;

					tree* new_cell = (tree*)malloc(sizeof(tree));
					if (!new_cell) {
						std::cerr << "Ошибка выделения памяти!" << std::endl;
						exit(1);
					}
					new_cell->data = data;
					new_cell->left = nullptr;
					new_cell->right = nullptr;

					if (side == 1) temp->left = new_cell;
					else if (side == 2) temp->right = new_cell;

					setConsoleColor(2);
					std::cout << "\nДобавлено\n" << std::endl;
					setConsoleColor(7);
					return root;
				}
				else {
					tree* new_cell = (tree*)malloc(sizeof(tree));
					if (!new_cell) {
						std::cerr << "Ошибка выделения памяти!" << std::endl;
						exit(1);
					}
					new_cell->data = data;
					new_cell->left = nullptr;
					new_cell->right = nullptr;
					temp->left = new_cell;

					setConsoleColor(2);
					std::cout << "\nДобавлено\n" << std::endl;
					setConsoleColor(7);
					return root;
				}
			}
			else if (temp->right == nullptr) {
				tree* new_cell = (tree*)malloc(sizeof(tree));
				if (!new_cell) {
					std::cerr << "Ошибка выделения памяти!" << std::endl;
					exit(1);
				}
				new_cell->data = data;
				new_cell->left = nullptr;
				new_cell->right = nullptr;
				temp->right = new_cell;

				setConsoleColor(2);
				std::cout << "\nДобавлено\n" << std::endl;
				setConsoleColor(7);
				return root;
			}
			else {
				setConsoleColor(4);
				std::cout << "\nДобавление невозможно\n" << std::endl;
				setConsoleColor(7);
				return root;
			}
		}
	}
}

tree* delete_cell(tree* root) {
	if (root == nullptr) return nullptr;
	else {
		tree* temp;
		temp = delete_cell(root->left);
		if (temp == nullptr) {
			temp = delete_cell(root->right);
			if (temp == nullptr) {
				std::cout << "Удалется веришна со значением: " << root->data << std::endl;
				free(root);
				root = temp;
				return nullptr;
			}
		}
	}
}

void main_menu(tree** root) {
	int ans;

	while (true) {
		std::cout << "Выберите действие:\n1.Добавить элемент\n2.Удалить дерево\n3.Вывести текущее состояние\n4.Поиска элемента" << std::endl;
		std::cout << "Ожидание ответа: ";
		std::cin >> ans;
		checking_input(ans, 1, 4);

		switch (ans) {
		case ADD_CELL:
			int data;
			std::cout << "Введите значение новой вершины: ";
			std::cin >> data;
			checking_input(data, -100000, 2147483647);
			if (data == NON) break;

			*root = add_cell(*root, data);
			break;
		case DELETE_TREE:
			if (!is_empty(*root)) {
				*root = delete_cell(*root);
				setConsoleColor(2);
				std::cout << "\nУдалено\n" << std::endl;
				setConsoleColor(7);
			}
			break;
		case CURRENT_STATUS:
			if (!is_empty(*root)) {
				std::cout << std::endl;
				setConsoleColor(2);
				symmetric_view(*root, 0);
				std::cout << std::endl;
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
				if(data == NON) break;

				std::cout << std::endl;
				temp = search_elem(*root, data);
				if (temp == nullptr) {
					setConsoleColor(4);
					std::cout << "\nЭлемент не найден\n" << std::endl;
					setConsoleColor(7);
				}
				else {
					setConsoleColor(2);
					std::cout << "\nНайлено\n" << std::endl;
					setConsoleColor(7);
				}
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