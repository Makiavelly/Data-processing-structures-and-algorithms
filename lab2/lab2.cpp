#include <iostream>
#include <windows.h>
#include <time.h>
#undef max

const unsigned int QUEUE_SIZE{ 4 };


enum ACTION_MAIN_MENU {
	NON = 0, ADD_CELL = 1, DELETE_CELL = 2, CURRENT_STATUS = 3
};

struct queue {
	int data_array[QUEUE_SIZE];
	unsigned int head;
	unsigned int tail;
	unsigned int count_elem;
};

void setConsoleColor(int color) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

bool is_full(queue& q) {
	if (q.count_elem == QUEUE_SIZE) {
		setConsoleColor(4);
		std::cout << "\nОчередь заполнена\n" << std::endl;
		setConsoleColor(7);
		return true;
	}
	else return false;
}

bool is_empty(queue& q) {
	if (q.count_elem == (unsigned int)0) {
		setConsoleColor(4);
		std::cout << "\nОчередь пуста\n" << std::endl;
		setConsoleColor(7);
		return true;
	}
	else return false;
}

void add_cell(queue& q, int data) {
	if (!is_full(q)) {
		q.data_array[q.tail] = data;
		q.count_elem += 1;
		q.tail += 1;
		if (q.tail > (QUEUE_SIZE - 1)) q.tail = 0;

		setConsoleColor(2);
		std::cout << "\nДобавлено!\n" << std::endl;
		setConsoleColor(7);
	}
}

void delete_cell(queue& q) {
	if (!is_empty(q)) {
		q.head += 1;
		q.count_elem -= 1;
		if (q.head > (QUEUE_SIZE - 1)) q.head = 0;

		setConsoleColor(2);
		std::cout << "\nУдалено\n" << std::endl;
		setConsoleColor(7);
	}
}

void current_status(queue& q) {
	if (!is_empty(q)) {
		std::cout << std::endl;
		setConsoleColor(2);
		unsigned int i{ q.head };
		do {
			if (i > (QUEUE_SIZE - 1)) {
				i = 0;
				if (i == q.tail) break;
			}
			std::cout << q.data_array[i] << std::endl;
			i += 1;
		} while (i != q.tail);
		setConsoleColor(7);
		std::cout << std::endl;
	}
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

void main_menu(queue& q) {
	int ans;

	while (true) {
		std::cout << "Выберите действие:\n1.Добавить элемент\n2.Удалить элемент\n3.Вывод текущего состояния" << std::endl;
		std::cout << "Ожидание ввода: ";

		std::cin >> ans;

		checking_input(ans, 1, 3);

		switch (ans) {
		case NON:
			break;
		case ADD_CELL:
			add_cell(q, std::rand());
			break;
		case DELETE_CELL:
			delete_cell(q);
			break;
		case CURRENT_STATUS:
			current_status(q);
			break;
		}
	}
}

void init_queue(queue& q) {
	q.head = 0;
	q.tail = 0;
	q.count_elem = 0;
}

int main()
{
	setlocale(LC_ALL, "ru");
	std::srand(std::time(0));

	queue q;

	init_queue(q);
	main_menu(q);
}
