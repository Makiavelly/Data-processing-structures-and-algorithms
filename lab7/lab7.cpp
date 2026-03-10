#include <iostream>
#include <windows.h>
#undef max

enum ACTION_MAIN_MENU {
	NON = -1000001, ADD_CELL = 1, DELETE_CELL = 2, CURRENT_STATUS = 3, SEARCH_ELEM = 4
};

enum MODE_ADD {
	BEFORE = 1, AFTER = 2
};

enum MODE_SEARCH {
	DIRECT = 1, REVERSE = 2
};

struct two_sides_list {
	two_sides_list *left, *right;
	int data;
};

two_sides_list* head{ (two_sides_list*)malloc(sizeof(two_sides_list)) };
two_sides_list* current_cell;
int count_elem{ 0 };

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

bool is_empty() {
	if (head->right == head) {
		setConsoleColor(4);
		std::cout << "\nСписок пуст\n" << std::endl;
		setConsoleColor(7);
		return true;
	}
	else return false;
}

void search_elem(int data) {
	int mode;
	std::cout << "В прямом или обратном порядке проводить поиск\n1.В прямом\n2.В обратном " << std::endl;
	std::cout << "Ожидание ввода: ";
	std::cin >> mode;
	checking_input(mode, 1, 2);
	if (mode == NON) current_cell = nullptr;

	int counter{ 0 };
	if (mode == DIRECT) {
		current_cell = head->right;
		while (current_cell != head) {
			if (current_cell->data == data) {
				setConsoleColor(2);
				std::cout << "\nЭлемент найден под индексом " << counter << std::endl << std::endl;
				setConsoleColor(7);
				break;
			}
			current_cell = current_cell->right; counter += 1;
		}

		if (current_cell == head) {
			setConsoleColor(4);
			std::cout << "\nЭлемент не найден" << std::endl << std::endl;
			setConsoleColor(7);
			current_cell = nullptr;
		}
	}
	else if (mode == REVERSE) {
		current_cell = head->left;
		while (current_cell != head) {
			if (current_cell->data == data) {
				setConsoleColor(2);
				std::cout << "\nЭлемент найден под индексом " << count_elem - (counter + 1) << std::endl << std::endl;
				setConsoleColor(7);
				break;
			}
			current_cell = current_cell->left; counter += 1;
		}

		if (current_cell == head) {
			setConsoleColor(4);
			std::cout << "\nЭлемент не найден" << std::endl << std::endl;
			setConsoleColor(7);
			current_cell = nullptr;
		}
	}
}

void add_cell(int data) {
	two_sides_list* temp = (two_sides_list*)malloc(sizeof(two_sides_list));
	temp->data = data;
	temp->right = current_cell->right;
	temp->left = current_cell;
	current_cell->right->left = temp;
	current_cell->right = temp;
	count_elem += 1;
}

void delete_cell() {
	current_cell->left->right = current_cell->right;
	current_cell->right->left = current_cell->left;
	free(current_cell);
	count_elem -= 1;
}

void current_status(int mode) {
	int counter{ 0 };
	std::cout << std::endl;
	if (mode == DIRECT) {
		two_sides_list* temp{ head->right };
		while (temp != head) {
			setConsoleColor(2);
			std::cout << counter << "-й элемент: " << temp->data << std::endl;
			setConsoleColor(7);
			temp = temp->right; counter += 1;
		}
	}
	else if (mode == REVERSE) {
		two_sides_list* temp{ head->left };
		while (temp != head) {
			setConsoleColor(2);
			std::cout << count_elem - (counter+1) << "-й элемент: " << temp->data << std::endl;
			setConsoleColor(7);
			temp = temp->left; counter += 1;
		}
	}
	std::cout << std::endl;
}

void main_menu() {
	int ans;

	while (true) {
		std::cout << "Выберите действие:\n1.Добавить элемент\n2.Удалить элемент\n3.Вывод текущего состояния\n4.Поиск элемента по значению" << std::endl;
		std::cout << "Ожидание ввода: ";

		std::cin >> ans;

		checking_input(ans, 1, 4);

		switch (ans) {
		case NON:
			break;
		case ADD_CELL:
			int mode;
			std::cout << "Добавить элемент до или после указанного:\n1.До\n2.После" << std::endl;
			std::cout << "Ожидание ввода: ";
			std::cin >> mode;
			checking_input(mode, 1, 2);
			if (mode == NON) break;
			if (head->right == head) {
				if (mode == AFTER)	current_cell = head;
				else if (mode == BEFORE) {
					setConsoleColor(4);
					std::cout << "\nНевозможное действие, т.к. список пуст\n" << std::endl;
					setConsoleColor(7);
					break;
				}
			}
			else {
				std::cout << "Введите значение элемента, до/после которого будет добавлено новое число: ";
				std::cin >> ans;
				checking_input(ans, -1000000, 2147483647);
				if (ans == NON) break;
				else search_elem(ans);
				if (current_cell == nullptr) break;
				if (mode == BEFORE) current_cell = current_cell->left;
			}
			int elem;
			std::cout << "Ведите значение элемента: ";
			std::cin >> elem;
			if (elem == NON) break;
			add_cell(elem);
			setConsoleColor(2);
			std::cout << "\nДобавлено\n" << std::endl;
			setConsoleColor(7);
			break;
		case DELETE_CELL:
			if (!is_empty()) {
				std::cout << "Введите значение удаляемого элемента: ";
				std::cin >> ans;
				checking_input(ans, -1000000, 2147483647);
				if (ans == NON) break;
				search_elem(ans);
				if (current_cell == nullptr) break;
				delete_cell();
				setConsoleColor(2);
				std::cout << "\nУдалено\n" << std::endl;
				setConsoleColor(7);
			}
			break;
		case CURRENT_STATUS:
			if (!is_empty()) {
				std::cout << "В прямом или обратном порядке проводить просмотр\n1.В прямом\n2.В обратном " << std::endl;
				std::cout << "Ожидание ввода: ";
				std::cin >> mode;
				checking_input(mode, 1, 2);
				if (mode == NON) break;
				current_status(mode);
			}
			break;
		case SEARCH_ELEM:
			if (!is_empty()) {
				std::cout << "Введите данные, которые нужно найти: ";
				std::cin >> ans;
				checking_input(ans, -1000000, 2147483647);
				if (ans == NON) break;
				search_elem(ans);
			}
			break;
		}
	}
}

void init() {
	head->left = head;
	head->right = head;
}

int main()
{
	setlocale(LC_ALL, "ru");
	init();

	main_menu();
}
