#include <iostream>
#include <windows.h>
#include <string>
#undef max

enum ACTION_MAIN_MENU {
	NON = -1000001, ADD_CELL = 1, DELETE_CELL = 2, CURRENT_STATUS = 3, SEARCH_ELEM = 4
};

enum EXECUTION_OPTION {
	INTEGER, STRING
};

const int LIST_SIZE{ 10 };
std::string stringi[LIST_SIZE]{ "vevefv", "qpckge", "ackt","rebd","trpobmrr","bbgkcv","gbkld", "rrtv", "xmllme", "pkgjblfm" };
constexpr EXECUTION_OPTION mode{ INTEGER };

template<typename T>
struct ordered_list {
    int count_elem;
	T data_array[LIST_SIZE];
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

template<typename T>
bool is_empty(ordered_list<T>& ol) {
	if (ol.count_elem == (int)0) {
		setConsoleColor(4);
		std::cout << "\nСписок пуст\n" << std::endl;
		setConsoleColor(7);
		return true;
	}
	else return false;
}

template<typename T>
bool is_full(ordered_list<T>& ol) {
	if (ol.count_elem == LIST_SIZE) {
		setConsoleColor(4);
		std::cout << "\nСписок полон\n" << std::endl;
		setConsoleColor(7);
		return true;
	}
	else return false;
}

template<typename T>
void add_cell(ordered_list<T>& ol, T data) {
	if (ol.count_elem == 0) ol.data_array[0] = data;
	else {
		int i{ ol.count_elem - 1 };
		for (; i > -1; i--) {
			if (data > ol.data_array[i]) { ol.data_array[i + 1] = data; break; }
			else ol.data_array[i + 1] = ol.data_array[i];
		}
		if (i == -1) ol.data_array[0] = data;
	}
	ol.count_elem += 1;
}

template<typename T>
void delete_cell(ordered_list<T>& ol, int index){
	for (int i = index; i < ol.count_elem; i++) {
		ol.data_array[i] = ol.data_array[i + 1];
	}
	ol.count_elem -= 1;
}

template<typename T>
void current_status(ordered_list<T>& ol) {
	std::cout << std::endl;
	setConsoleColor(2);
	for (int i = 0; i < ol.count_elem; i++) {
		std::cout << i << "-й элемент = " << ol.data_array[i] << std::endl;
	}
	setConsoleColor(7);
	std::cout << std::endl;
}

template<typename T>
int search_elem(ordered_list<T>& ol, T data) {
	int i{ 0 };
	for (; i < ol.count_elem; i++) {
		if (ol.data_array[i] == data) {
			setConsoleColor(2);
			std::cout << "\nЭлемент найден под индексом " << i << std::endl << std::endl;
			setConsoleColor(7);
			return i;
		}
	}
	if (i == ol.count_elem) {
		setConsoleColor(4);
		std::cout << "\nЭлемент не найден\n" << std::endl;
		setConsoleColor(7);
		return NON;
	}
}

template<typename T>
void main_menu(ordered_list<T>& ol) {
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
			if (!is_full(ol)) {
				if constexpr (mode == INTEGER) { 
					int elem;
					std::cout << "Введите добавлемое число: ";
					std::cin >> elem;
					checking_input(elem, -1000000, 2147483647);
					if (elem == NON) break;
					else add_cell<T>(ol, elem); 
				}
				else if constexpr (mode == STRING) { 
					//std::string elem = stringi[std::rand() % 10];
					std::string elem;
					std::cout << "Введите добавяемую строку: ";
					std::cin >> elem;
					add_cell<T>(ol, elem);
				}
				
				setConsoleColor(2);
				std::cout << "\nДобавлено\n" << std::endl;
				setConsoleColor(7);
			}
			break;
		case DELETE_CELL:
			if (!is_empty<T>(ol)) {
				std::cout << "Введите значение удаляемого элемента: ";
				std::cin >> ans;
				checking_input(ans, -1000000, 2147483647);
				if (ans == NON) break;
				else { 
					ans = search_elem(ol, ans);
					if (ans == NON) break;
					delete_cell<T>(ol, ans); 
					setConsoleColor(2);
					std::cout << "\nУдалено\n" << std::endl;
					setConsoleColor(7);
				}
			}
			break;
		case CURRENT_STATUS:
			if (!is_empty<T>(ol)) current_status<T>(ol);
			break;
		case SEARCH_ELEM:
			if (!is_empty<T>(ol)) {
				T ans;
				std::cout << "Введите данные, которые нужно найти: ";
				std::cin >> ans;
				if constexpr (mode == INTEGER) {
					checking_input(ans, 0, 2147483647);
					if (ans == NON) break;
				}
				search_elem<T>(ol, ans);
			}
			break;
		}
	}

}

int main()
{
	setlocale(LC_ALL, "ru");

	if constexpr (mode == INTEGER) { ordered_list<int> ol{ 0 }; main_menu<int>(ol); }
	else if constexpr (mode == STRING) { ordered_list<std::string> ol{ 0 }; main_menu<std::string>(ol); }
}