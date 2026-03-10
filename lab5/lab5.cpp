#include <iostream>
#include <windows.h>
#undef max

enum ACTION_MAIN_MENU {
	NON = -1000001, ADD_CELL = 1, DELETE_CELL = 2, CURRENT_STATUS = 3, SEARCH_ELEM = 4
};

enum MODE_ADD {
	BEFORE = 1, AFTER = 2
};

const int LIST_SIZE{ 10 };
int count_elem{ 0 };

struct list_elem {
	int data;
	int index_ptr;
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

bool is_empty() {
	if (count_elem == 1) {
		setConsoleColor(4);
		std::cout << "\nСписок пуст\n" << std::endl;
		setConsoleColor(7);
		return true;
	}
	else return false;
}

bool is_full() {
	if (count_elem == LIST_SIZE) {
		setConsoleColor(4);
		std::cout << "\nСписок полон\n" << std::endl;
		setConsoleColor(7);
		return true;
	}
	else return false;
}

void add_cell(list_elem (&l)[LIST_SIZE], int data, int mode, int index) {
	int free_cell;
	for (int i = 1; i < LIST_SIZE; i++) if (l[i].index_ptr == -1) free_cell = i;
	l[free_cell].data = data;

	if (mode == NON) {
		l[free_cell].index_ptr = 0;
		l[0].index_ptr = free_cell;
		count_elem += 1;
		return;
	}
	
	if (mode == BEFORE) index -= 1;

	int current_index{ 0 };
	for (int i = 0; i < index; i++) current_index = l[current_index].index_ptr;
	l[free_cell].index_ptr = l[current_index].index_ptr;
	l[current_index].index_ptr = free_cell;
	count_elem += 1;
}

void delete_cell(list_elem(&l)[LIST_SIZE], int index) {
	int current_index{ 0 }, predecessor_index{ 0 };
	for (int i = 0; i <= index; i++) {
		predecessor_index = current_index;
		current_index = l[current_index].index_ptr;
	}
	l[predecessor_index].index_ptr = l[current_index].index_ptr;
	l[current_index].index_ptr = -1;
	count_elem -= 1;
}

void current_status(list_elem(&l)[LIST_SIZE]) {
	int i{ 0 }, counter{ 0 };
	setConsoleColor(2);
	std::cout << std::endl;
	while (i = l[i].index_ptr) { std::cout << counter << "-й элемент: " << l[i].data << std::endl; counter += 1; }
	std::cout << std::endl;
	setConsoleColor(7);
}

int search_elem(list_elem(& l)[LIST_SIZE], int data) {
	int i{ 0 }, counter{ 0 };
	while (i = l[i].index_ptr) {
		if (l[i].data == data) {
			setConsoleColor(2);
			std::cout << "\nЭлемент найден под индексом " << counter << std::endl << std::endl;
			setConsoleColor(7);
			return counter;
		}
		counter += 1;
	}
	setConsoleColor(4);
	std::cout << "\nЭлемент не найден\n" << std::endl;
	setConsoleColor(7);
	return NON;
}

void main_menu(list_elem (&l)[LIST_SIZE]) {
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
			if (!is_full()) {
				int elem;
				std::cout << "Введите добавляемый элемент: ";
				std::cin >> elem;
				checking_input(elem, -1000000, 2147483647);
				if (elem == NON) break;

				if (count_elem == 1) {
					ans = 0;
					add_cell(l, elem, NON, ans);
				}
				else {
					std::cout << "Введите значение элемента до/или после которого нужно добавить элемент: ";
					std::cin >> ans;
					checking_input(ans, -1000000, 2147483647);
					if (ans == NON) break;
					ans = search_elem(l, ans);
					if (ans == NON) break;
					else ans += 1;
					int mode;
					std::cout << "Добавить элемент до или после указанного:\n1.До\n2.После" << std::endl;
					std::cout << "Ожидание ввода: ";
					std::cin >> mode;
					checking_input(mode, 1, 2);
					if (mode == NON) break;
					add_cell(l, elem, mode, ans);
				}
				setConsoleColor(2);
				std::cout << "\nДобавлено\n" << std::endl;
				setConsoleColor(7);
			}
			break;
		case DELETE_CELL:
			if (!is_empty()) {
				std::cout << "Введите значение удаляемого элемента: ";
				std::cin >> ans;
				checking_input(ans, -1000000, 2147483647);
				if (ans == NON) break;
				else {
					ans = search_elem(l, ans);
					if (ans == NON) break;
					delete_cell(l, ans);
					setConsoleColor(2);
					std::cout << "\nУдалено\n" << std::endl;
					setConsoleColor(7);
				}
			}
			break;
		case CURRENT_STATUS:
			if (!is_empty()) current_status(l);
			break;
		case SEARCH_ELEM:
			if (!is_empty()) { 
				std::cout << "Введите данные, которые нужно найти: ";
				std::cin >> ans;
				checking_input(ans, 0, 2147483647);
				if (ans != NON)	search_elem(l, ans);
			}
			break;
		}
	}
}

void init(list_elem(&l)[LIST_SIZE]) {
	l[0].index_ptr = 0;
	for (int i = 1; i < LIST_SIZE; i++) l[i].index_ptr = -1;
	count_elem = 1;
}

int main()
{
	setlocale(LC_ALL, "ru");

	list_elem l[LIST_SIZE];
	init(l);
	main_menu(l);


}