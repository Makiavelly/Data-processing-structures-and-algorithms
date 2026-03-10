#include <iostream>
#include <windows.h>
#include <time.h>
#undef max

enum ACTION_MAIN_MENU {
	ADD_CELL = 1, DELETE_CELL = 2, CURRENT_STATUS = 3, TRASH_STATUS = 4
};
enum ACTION_ADD_MENU {
	NON, NEW, OLD
};

enum ACTIONDELETE_MENU {
	FINALLY = 1, STACK = 2
};

struct mem_cell {
	mem_cell* ptr;
	int data;
};

void add_cell(mem_cell** sp, int data) {
	mem_cell* temp{ (mem_cell*)malloc(sizeof(mem_cell)) };
	if (temp == nullptr) {
		std::cerr << "Ошибка: Не удалось выделить память." << std::endl;
		exit(1);
	}

	if ((*sp) == nullptr) {
		(*sp) = temp;
		(*sp)->data = data;
		(*sp)->ptr = nullptr;
	}
	else {
		temp->ptr = (*sp);
		temp->data = data;
		(*sp) = temp;
	}
}

void delete_cell(mem_cell** sp) {
	mem_cell* temp{ (*sp)->ptr };
	free(*sp);
	(*sp) = temp;
}

void swap_cell(mem_cell** from, mem_cell** to) {
	mem_cell* temp{ (*from)->ptr };
	(*from)->ptr = (*to);
	(*to) = (*from);
	(*from) = temp;
}

void setConsoleColor(int color) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void current_status(mem_cell* sp) {
	if (sp == nullptr) {
		setConsoleColor(2);
		std::cout << "\nСтек пуст\n" << std::endl;
		setConsoleColor(7);
	}
	else {
		mem_cell* temp{ sp };
		unsigned int counter{ 1 };

		std::cout << std::endl;
		setConsoleColor(2);
		while (temp != nullptr) {
			std::cout << counter << "-й элемент = " << temp->data << std::endl;
			temp = temp->ptr;
			counter += 1;
		}
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

void input_num_elem(int& num) {
	std::cout << "\nВведите количество элементов: ";
	std::cin >> num;
	checking_input(num, 1, 2147483647);
}

void add_menu(mem_cell** sp, mem_cell** sp_trash) {
	int ans;

	std::cout << "\nВыберите вариант добавления:\n1. Новый элемент\n2. Взять из вспомогательного стека удалённых элементов" << std::endl;
	std::cout << "Ожидание ввода: ";

	std::cin >> ans;
	checking_input(ans, 1, 2);

	switch (ans) {
	case NON:
		break;
	case NEW:
		input_num_elem(ans);
		for (; ans > 0; ans--) {
			add_cell(&(*sp), std::rand());
		}
		setConsoleColor(2);
		std::cout << "\nДобавлено!\n" << std::endl;
		setConsoleColor(7);
		break;
	case OLD:
		if ((*sp_trash) == nullptr) {
			setConsoleColor(4);
			std::cout << "\nСтек удалённых элементов пуст\n" << std::endl;
			setConsoleColor(7);
		}
		else {
			input_num_elem(ans);
			int i{ 0 };
			for (; i < ans; i++) {
				if ((*sp_trash) == nullptr) {
					setConsoleColor(4);
					std::cout << "\nСтек удалённых элементов опустел. Было добавлено " << i << " элементов\n" << std::endl;
					setConsoleColor(7);
					break;
				}
				else swap_cell(&(*sp_trash), &(*sp));
			}
			if (i == ans) {
				setConsoleColor(2);
				std::cout << "\nДобавлено!\n" << std::endl;
				setConsoleColor(7);
			}
		}
		break;
	}
}

void delete_menu(mem_cell** sp, mem_cell** sp_trash) {
	int ans;

	std::cout << "\nВыберите вариант удаления:\n1. Удалить окончательно\n2. Положить в стек удалённых элементов" << std::endl;
	std::cout << "Ожидание ввода: ";

	std::cin >> ans;
	checking_input(ans, 1, 2);

	switch (ans) {
	case NON:
		break;
	case FINALLY: {
		input_num_elem(ans);
		int j{ 0 };
		for (; j < ans; j++) {
			if ((*sp) == nullptr) {
				setConsoleColor(4);
				std::cout << "\nСтек опустел. Было удалено " << j << " элементов\n" << std::endl;
				setConsoleColor(7);
				break;
			}
			else delete_cell(&(*sp));
		}
		if (ans == j) {
			setConsoleColor(2);
			std::cout << "\nУдалено\n" << std::endl;
			setConsoleColor(7);
		}
		break;
	}
	case STACK:
		input_num_elem(ans);
		int i{ 0 };
		for (; i < ans; i++) {
			if ((*sp) == nullptr) {
				setConsoleColor(4);
				std::cout << "\nСтек опустел. Было перенесено " << i << " элементов\n" << std::endl;
				setConsoleColor(7);
				break;
			}
			else swap_cell(&(*sp), &(*sp_trash));
		}
		if (i == ans) {
			setConsoleColor(2);
			std::cout << "\nУдалено\n" << std::endl;
			setConsoleColor(7);
		}
		break;
	}

}

void main_menu(mem_cell** sp, mem_cell** sp_trash) {
	int ans;

	while (true)
	{
		std::cout << "Выберите лействие:\n1. Добавить элемент\n2. Удалить элемент\n3. Вывести текущее состояние\n4. Вывести состояние вспомогательного стека" << std::endl;
		std::cout << "Ожидание ввода: ";

		std::cin >> ans;
		checking_input(ans, 1, 4);

		switch (ans) {
		case NON:
			break;
		case ADD_CELL:
			add_menu(&(*sp), &(*sp_trash));
			break;
		case DELETE_CELL:
			if ((*sp) == nullptr) {
				setConsoleColor(4);
				std::cout << "\nСтек пуст\n" << std::endl;
				setConsoleColor(7);
				break;
			}
			else delete_menu(&(*sp), &(*sp_trash));
			break;
		case CURRENT_STATUS:
			current_status((*sp));
			break;
		case TRASH_STATUS:
			current_status((*sp_trash));
		}
	}
}

int main()
{
	setlocale(LC_ALL, "ru");
	std::srand(std::time(0));

	mem_cell* sp{ nullptr }, * sp_trash{ nullptr };

	main_menu(&sp, &sp_trash);
}