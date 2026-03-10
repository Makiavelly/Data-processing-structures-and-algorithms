#include <iostream>
#include <windows.h>
#undef max

enum ACTION_MAIN_MENU {
	NON = -1000001, ADD_CELL = 1, DELETE_CELL = 2, CURRENT_STATUS = 3, SEARCH_ELEM = 4, STACK_STATUS = 5
};

enum MODE_ADD {
	BEFORE = 1, AFTER = 2
};

struct list_elem {
	int data;
	list_elem* ptr;
};

list_elem* head;
list_elem* current_cell;
list_elem* sp;

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
	if (head->ptr == nullptr) {
		setConsoleColor(4);
		std::cout << "\nСписок пуст\n" << std::endl;
		setConsoleColor(7);
		return true;
	}
	else return false;
}

void add_cell(int data) {
	list_elem* temp = (list_elem*)malloc(sizeof(list_elem));
	temp->data = data;
	temp->ptr = current_cell->ptr;
	current_cell->ptr = temp;
}

void delete_cell() {
	list_elem *temp;
	temp = current_cell->ptr;
	current_cell->ptr = temp->ptr;
	if (sp == nullptr) temp->ptr = nullptr;
	else temp->ptr = sp;
	sp = temp;
}

void current_status() {
	int counter{ 0 };
	list_elem* temp{ head->ptr };
	setConsoleColor(2);
	std::cout << std::endl;
	while (temp != nullptr) { 
		std::cout << counter << "-й элемент: " << temp->data << std::endl; 
		temp = temp->ptr;
		counter += 1; 
	}
	std::cout << std::endl;
	setConsoleColor(7);
}

void stack_status() {
	if (sp == nullptr) {
		setConsoleColor(4);
		std::cout << "\nСписок вспомогательных элементов пуст\n" << std::endl;
		setConsoleColor(7);
	}
	else {
		int counter{ 0 };
		list_elem* temp{ sp };
		setConsoleColor(2);
		std::cout << std::endl;
		while (temp != nullptr) {
			std::cout << counter << "-й элемент: " << temp->data << std::endl;
			temp = temp->ptr; counter++;
		}
		std::cout << std::endl;
		setConsoleColor(7);
	}
}

void search_elem(int data) {
	int counter{ 0 };
	current_cell = head;
	while (current_cell->ptr != nullptr) {
		if (current_cell->ptr->data == data) {
			setConsoleColor(2);
			std::cout << "\nЭлемент найден под индексом " << counter << std::endl << std::endl;
			setConsoleColor(7);
			break;
		}
		current_cell = current_cell->ptr;
		counter += 1;
	}
	if (current_cell->ptr == nullptr) {
		setConsoleColor(4);
		std::cout << "\nЭлемент не найден\n" << std::endl;
		setConsoleColor(7);
		current_cell = nullptr;
	}
}

void main_menu() {
	int ans;

	while (true) {
		std::cout << "Выберите действие:\n1.Добавить элемент\n2.Удалить элемент\n3.Вывод текущего состояния\n4.Поиск элемента по значению\n5.Вывод состояния вспомогательного списка" << std::endl;
		std::cout << "Ожидание ввода: ";

		std::cin >> ans;

		checking_input(ans, 1, 5);

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
			if (head->ptr == nullptr) {
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
				if (mode == AFTER) current_cell = current_cell->ptr;
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
			if (!is_empty()) current_status();
			break;
		case SEARCH_ELEM:
			if (!is_empty()) {
				std::cout << "Введите данные, которые нужно найти: ";
				std::cin >> ans;
				checking_input(ans, -1000000, 2147483647);
				if (ans != NON)	search_elem(ans);
			}
			break;
		case STACK_STATUS:
			stack_status();
			break;
		}
	}
}

void init() {
	head->ptr = nullptr;
	head->data = NULL;
}

int main()
{
	setlocale(LC_ALL, "ru");

	head = (list_elem*)malloc(sizeof(list_elem));
	sp;

	init();
	main_menu();


}