#include <iostream>
#include <windows.h>
#undef max

enum ACTION_MAIN_MENU {
	NON = -1000001, ADD_CELL_PRIMARY = 1, ADD_CELL_SECONDARY = 2, DELETE_CELL_PRIMARY = 3, DELETE_CELL_SECONDARY = 4, CURRENT_STATUS = 5, SEARCH_ELEM = 6
};

enum MODE_ADD {
	BEFORE = 1, AFTER = 2
};

struct list_secondary {
	int data;
	list_secondary* next;
};

struct list_primary {
	int data;
	list_secondary* first_secondary_cell;
	list_primary* next;
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
bool is_empty(T* h) {
	if (h == nullptr) {
		setConsoleColor(4);
		std::cout << "\nСписок пуст\n" << std::endl;
		setConsoleColor(7);
		return true;
	}
	else return false;
}



void current_status(list_primary* temp_primary) {
	std::cout << std::endl;
	setConsoleColor(2);
	int counter_primary{ 0 };
	while (temp_primary != nullptr) {
		std::cout << counter_primary << "-й вложенный список имеет значение: " << temp_primary->data << std::endl;
		list_secondary* temp_secondary{ temp_primary->first_secondary_cell };
		int counter_secondary{ 0 };
		if (temp_secondary == nullptr) {
			std::cout << "\tПуст" << std::endl;
		}
		while (temp_secondary != nullptr) {
			std::cout << "\t" << counter_secondary << "-й элемент: " << temp_secondary->data << std::endl;
			temp_secondary = temp_secondary->next; counter_secondary += 1;
		}
		temp_primary = temp_primary->next; counter_primary += 1;
	}
	setConsoleColor(7);
	std::cout << std::endl;
}

template<typename T>
T* search(T* lp, int data) {
	if (lp->data == data) return lp;
	while (lp->next != nullptr){
		if (lp->next->data == data) return lp;
		lp = lp->next;
	}
	return nullptr;
}

void search_elem(list_primary* temp_primary, int data) {
	int counter_primary{ 0 }, counter_secondary{ 0 };
	//проверка первого элемента в первичном списке
	if (temp_primary->data == data) {
		std::cout << std::endl;
		setConsoleColor(2);
		std::cout << "Элемент найден под индексом " << counter_primary << " первичного списка" << std::endl;
		setConsoleColor(7);
		std::cout << std::endl;
		return;
		/*void* ptr = malloc(sizeof(list_primary*) + sizeof(list_secondary*));
		list_primary** pr_ptr = (list_primary**)ptr;
		(*pr_ptr) = temp_primary;
		list_secondary** sec_ptr = (list_secondary**)((char*)ptr + sizeof(list_primary*));
		(*sec_ptr) = nullptr;
		return ptr;*/
	}
	list_secondary* first_temp_secondary{ temp_primary->first_secondary_cell };
	//проверка первого элемента в первом вторичном списке
	if (first_temp_secondary->data == data) {
		std::cout << std::endl;
		setConsoleColor(2);
		std::cout << "Элемент найден в " << counter_primary << " вложенном списке под индексом " << counter_secondary << std::endl;
		setConsoleColor(7);
		std::cout << std::endl;
		return;
		/*void* ptr = malloc(sizeof(list_primary*) + sizeof(list_secondary*));
		list_primary** pr_ptr = (list_primary**)ptr;
		(*pr_ptr) = temp_primary;
		list_secondary** sec_ptr = (list_secondary**)((char*)ptr + sizeof(list_primary*));
		(*sec_ptr) = first_temp_secondary;
		return ptr;*/
	}
	//проверка остальных элементов первого вторичного списка
	while (first_temp_secondary->next != nullptr) {
		if (first_temp_secondary->next->data == data) {
			std::cout << std::endl;
			setConsoleColor(2);
			std::cout << "Элемент найден в " << counter_primary << " вложенном списке под индексом " << counter_secondary << std::endl;
			setConsoleColor(7);
			std::cout << std::endl;
			return;
			/*void* ptr = malloc(sizeof(list_primary*) + sizeof(list_secondary*));
			list_primary** pr_ptr = (list_primary**)ptr;
			(*pr_ptr) = temp_primary;
			list_secondary** sec_ptr = (list_secondary**)((char*)ptr + sizeof(list_primary*));
			(*sec_ptr) = first_temp_secondary;
			return ptr;*/
		}
		first_temp_secondary = first_temp_secondary->next; counter_secondary += 1;
	}
	counter_primary += 1;
	//проверка остальных первичных списков и их подсписков
	while (temp_primary->next != nullptr) {

		if (temp_primary->next->data == data) {
			std::cout << std::endl;
			setConsoleColor(2);
			std::cout << "Элемент найден под индексом " << counter_primary << " первичного списка" << std::endl;
			setConsoleColor(7);
			std::cout << std::endl;
			return;
			/*void* ptr = malloc(sizeof(list_primary*) + sizeof(list_secondary*));
			list_primary** pr_ptr = (list_primary**)ptr;
			(*pr_ptr) = temp_primary;
			list_secondary** sec_ptr = (list_secondary**)((char*)ptr + sizeof(list_primary*));
			(*sec_ptr) = nullptr;
			return ptr;*/
		}

		counter_secondary = 0;
		list_secondary* temp_secondary{ temp_primary->next->first_secondary_cell };
		//проверка первого списка вторичного списка
		if (temp_secondary->data == data) {
			std::cout << std::endl;
			setConsoleColor(2);
			std::cout << "Элемент найден в " << counter_primary << " вложенном списке под индексом " << counter_secondary << std::endl;
			setConsoleColor(7);
			std::cout << std::endl;
			return;
			/*void* ptr = malloc(sizeof(list_primary*) + sizeof(list_secondary*));
			list_primary** pr_ptr = (list_primary**)ptr;
			(*pr_ptr) = temp_primary->next;
			list_secondary** sec_ptr = (list_secondary**)((char*)ptr + sizeof(list_primary*));
			(*sec_ptr) = temp_secondary;
			return ptr;*/
		}
		else counter_secondary += 1;
		//проверка остальных списков вторичного списка
		while (temp_secondary->next != nullptr) {
			if (temp_secondary->next->data == data) {
				std::cout << std::endl;
				setConsoleColor(2);
				std::cout << "Элемент найден в " << counter_primary << " вложенном списке под индексом " << counter_secondary << std::endl;
				setConsoleColor(7);
				std::cout << std::endl;
				return;
				/*void* ptr = malloc(sizeof(list_primary*) + sizeof(list_secondary*));
				list_primary** pr_ptr = (list_primary**)ptr;
				(*pr_ptr) = temp_primary->next;
				list_secondary** sec_ptr = (list_secondary**)((char*)ptr + sizeof(list_primary*));
				(*sec_ptr) = temp_secondary;
				return ptr;*/
			}
			temp_secondary = temp_secondary->next; counter_secondary += 1;
		}
		temp_primary = temp_primary->next; counter_primary += 1;
	}

	std::cout << std::endl;
	setConsoleColor(4);
	std::cout << "Элемент не найден" << std::endl;
	setConsoleColor(7);
	std::cout << std::endl;

	/*void* ptr = malloc(sizeof(list_primary*) + sizeof(list_secondary*));
	list_primary** pr_ptr = (list_primary**)ptr;
	(*pr_ptr) = nullptr;
	list_secondary** sec_ptr = (list_secondary**)((char*)ptr + sizeof(list_primary*));
	(*sec_ptr) = nullptr;
	return ptr;*/
}

list_primary* add_cell(list_primary* cell, int data, bool head_change) {
	list_primary* temp = (list_primary*)malloc(sizeof(list_primary));
	if (!temp) {
		std::cerr << "Ошибка выделения памяти!" << std::endl;
		exit(1);
	}

	temp->data = data;
	temp->first_secondary_cell = nullptr;

	if (head_change) {
		temp->next = cell;
		return temp;
	}
	else {
		temp->next = cell->next;
		cell->next = temp;
		return nullptr;
	}
}

list_secondary* add_cell(list_secondary* cell, int data, bool head_change) {
	list_secondary* temp = (list_secondary*)malloc(sizeof(list_secondary));
	if (!temp) {
		std::cerr << "Ошибка выделения памяти!" << std::endl;
		exit(1);
	}

	temp->data = data;

	if (head_change) {
		temp->next = cell;
		return temp;
	}
	else {
		temp->next = cell->next;
		cell->next = temp;
		return nullptr;
	}
}

int add_action_primary(list_primary** head) {
	int target_value, mode, value;
	list_primary* previous_elem;
	bool head_change{ false }, first_elem_is_found{ false };

	if (*head == nullptr) {
		std::cout << "Введите добавляемое знчение: ";
		std::cin >> value;
		checking_input(value, -1000000, 2147483647);
		if (value == NON) return NON;
		head_change = true;

		(*head) = add_cell(*head, value, head_change);
	}
	else {
		std::cout << "Введите знчение элемента, до/после которого будет добавлен новый элемент: ";
		std::cin >> target_value;
		checking_input(target_value, -1000000, 2147483647);
		if (target_value == NON) return NON;
		else {
			if ((*head)->data == target_value) {
				previous_elem = *head;
				first_elem_is_found = true;
			}
			else {
				previous_elem = search<list_primary>((*head), target_value);
				if (previous_elem == nullptr) {
					std::cout << std::endl;
					setConsoleColor(4);
					std::cout << "Элемент не найден" << std::endl;
					setConsoleColor(7);
					std::cout << std::endl;
					return NON;
				}
			}
		}

		std::cout << "Введите добавляемое знчение: ";
		std::cin >> value;
		checking_input(value, -1000000, 2147483647);
		if (value == NON) return NON;

		std::cout << "Добавить элемент до или после указанного:\n1.До\n2.После" << std::endl;
		std::cout << "Ожидание ввода: ";
		std::cin >> mode;
		checking_input(mode, 1, 2);
		if (mode == NON) return NON;

		if (first_elem_is_found && mode == BEFORE) head_change = true;
		else if (!first_elem_is_found && mode == AFTER) previous_elem = previous_elem->next;

		if (head_change) *head = add_cell(previous_elem, value, head_change);
		else  add_cell(previous_elem, value, head_change);
	}

	return 0;
}

int add_action_secondary(list_primary** head) {
	int target_value, mode, value;
	list_primary* primary_elem;
	list_secondary* previous_elem;
	bool head_change{ false }, first_elem_is_found{ false };

	std::cout << "Введите знчение элемента основного списка, в подсписок которого будет добавлен новый элемент: ";
	std::cin >> target_value;
	checking_input(target_value, -1000000, 2147483647);
	if (target_value == NON) return NON;
	else {
		if ((*head)->data == target_value) {
			primary_elem = *head;
			first_elem_is_found = true;
		}
		else {
			primary_elem = search<list_primary>((*head), target_value);
			if (primary_elem == nullptr) {
				std::cout << std::endl;
				setConsoleColor(4);
				std::cout << "Элемент не найден" << std::endl;
				setConsoleColor(7);
				std::cout << std::endl;
				return NON;
			}
			else primary_elem = primary_elem->next;
		}
	}

	if (primary_elem->first_secondary_cell == nullptr) {
		std::cout << "Введите добавляемое знчение: ";
		std::cin >> value;
		checking_input(value, -1000000, 2147483647);
		if (value == NON) return NON;
		head_change = true;

		primary_elem->first_secondary_cell = add_cell(primary_elem->first_secondary_cell, value, head_change);
	}
	else {
		std::cout << "Введите знчение элемента, до/после которого будет добавлен новый элемент: ";
		std::cin >> target_value;
		checking_input(target_value, -1000000, 2147483647);
		if (target_value == NON) return NON;
		else {
			if (primary_elem->first_secondary_cell->data == target_value) {
				first_elem_is_found = true;
				previous_elem = primary_elem->first_secondary_cell;
			}
			else {
				first_elem_is_found = false;
				previous_elem = search<list_secondary>(primary_elem->first_secondary_cell, target_value);
				if (previous_elem == nullptr) {
					std::cout << std::endl;
					setConsoleColor(4);
					std::cout << "Элемент не найден" << std::endl;
					setConsoleColor(7);
					std::cout << std::endl;
					return NON;
				}
			}
		}

		std::cout << "Введите добавляемое знчение: ";
		std::cin >> value;
		checking_input(value, -1000000, 2147483647);
		if (value == NON) return NON;

		std::cout << "Добавить элемент до или после указанного:\n1.До\n2.После" << std::endl;
		std::cout << "Ожидание ввода: ";
		std::cin >> mode;
		checking_input(mode, 1, 2);
		if (mode == NON) return NON;

		if (first_elem_is_found && mode == BEFORE) head_change = true;
		else if (!first_elem_is_found && mode == AFTER) previous_elem = previous_elem->next;

		if (head_change) primary_elem->first_secondary_cell = add_cell(previous_elem, value, head_change);
		else  add_cell(previous_elem, value, head_change);
	}

	return 0;
}

list_secondary* delete_cell(list_secondary* cell, bool head_change) {
	list_secondary* temp;

	if (head_change) {
		temp = cell->next;
		free(cell);
		cell = nullptr;
		return temp;
	}
	else {
		temp = cell->next;
		cell->next = temp->next;
		free(temp);
		temp = nullptr;
		return nullptr;
	}
}

list_primary* delete_cell(list_primary* cell, bool head_change) {
	list_primary* temp_p;
	list_secondary* temp_s;
	if (head_change) {
		temp_p = cell->next;
		temp_s = cell->first_secondary_cell;
		list_secondary* temp_previous;
		while (temp_s != nullptr) {
			temp_previous = temp_s;
			temp_s = temp_s->next;
			free(temp_previous);
		}
		free(cell);
		cell = nullptr;
		return temp_p;
	}
	else {
		temp_p = cell->next;
		cell->next = temp_p->next;
		temp_s = temp_p->first_secondary_cell;
		list_secondary* temp_previous;
		while (temp_s != nullptr) {
			temp_previous = temp_s;
			temp_s = temp_s->next;
			free(temp_previous);
		}
		free(temp_p);
		temp_p = nullptr;
		return nullptr;
	}

}

int delete_action_primary(list_primary** head) {
	bool head_change{ false };
	int value;
	list_primary* previous_elem;
	std::cout << "Введите значение удаляемого элемента: ";
	std::cin >> value;
	checking_input(value, -1000000, 2147483647);
	if (value == NON) return NON;

	if ((*head)->data == value) {
		head_change = true;
		previous_elem = (*head);
	}
	else {
		previous_elem = search<list_primary>(*head, value);
		if (previous_elem == nullptr) {
			std::cout << std::endl;
			setConsoleColor(4);
			std::cout << "Элемент не найден" << std::endl;
			setConsoleColor(7);
			std::cout << std::endl;
			return NON;
		}
	}

	if (head_change) *head = delete_cell(previous_elem, head_change);
	else delete_cell(previous_elem, head_change);

	return 0;
}

int delete_action_secondary(list_primary** head) {
	bool head_change{ false };
	int value_p, value_s;
	list_primary* primary_elem;
	list_secondary* previous_elem;

	std::cout << "Введите значение элемента основного списка, из подсписка которого будет удалён нужный элемент: ";
	std::cin >> value_p;
	checking_input(value_p, -1000000, 2147483647);
	if (value_p == NON) return NON;

	if ((*head)->data == value_p) {
		primary_elem = *head;
	}
	else {
		primary_elem = search<list_primary>(*head, value_p);
		if (primary_elem == nullptr) {
			std::cout << std::endl;
			setConsoleColor(4);
			std::cout << "Элемент не найден" << std::endl;
			setConsoleColor(7);
			std::cout << std::endl;
			return NON;
		}
		else primary_elem = primary_elem->next;
	}
	if (primary_elem->first_secondary_cell == nullptr) {
		std::cout << std::endl;
		setConsoleColor(4);
		std::cout << "Первичный списоок пуст" << std::endl;
		setConsoleColor(7);
		std::cout << std::endl;
		return NON;
	}

	std::cout << "Введите значение удаляемого элемента: ";
	std::cin >> value_s;
	checking_input(value_s, -1000000, 2147483647);
	if (value_s == NON) return NON;

	if (primary_elem->first_secondary_cell->data == value_s) {
		head_change = true;
		previous_elem = primary_elem->first_secondary_cell;
	}
	else {
		previous_elem = search<list_secondary>(primary_elem->first_secondary_cell, value_s);
		if (previous_elem == nullptr) {
			std::cout << std::endl;
			setConsoleColor(4);
			std::cout << "Элемент не найден" << std::endl;
			setConsoleColor(7);
			std::cout << std::endl;
			return NON;
		}
	}

	if (head_change) primary_elem->first_secondary_cell = delete_cell(previous_elem, head_change);
	else delete_cell(previous_elem, head_change);

	return 0;
}

void main_menu(list_primary** head) {
	int ans;

	while (true) {
		std::cout << "Выберите действие:\n1.Добавить элемент в первичный список\n2.Добавить элемент во вторичный список\n3.Удалить элемент из первичного списка\n4.Удалить элемент из вторичного списка\n\
5.Вывод текущего состояния\n6.Поиск элемента по значению" << std::endl;
		std::cout << "Ожидание ввода: ";
		std::cin >> ans;
		checking_input(ans, 1, 6);

		switch (ans) {
		case NON:
			break;
		case ADD_CELL_PRIMARY:
			if (add_action_primary(&(*head)) != NON) {
				setConsoleColor(2);
				std::cout << "\nДобавлено\n" << std::endl;
				setConsoleColor(7);
			}
			break;
		case ADD_CELL_SECONDARY:
			if (!is_empty(*head)) {
				if (add_action_secondary(&(*head)) != NON) {
					setConsoleColor(2);
					std::cout << "\nДобавлено\n" << std::endl;
					setConsoleColor(7);
				}
			}
			break;
		case DELETE_CELL_PRIMARY:
			if (!is_empty(*head)) {
				if (delete_action_primary(&(*head)) != NON) {
					setConsoleColor(2);
					std::cout << "\nУдалено\n" << std::endl;
					setConsoleColor(7);
				}
			}
			break;
		case DELETE_CELL_SECONDARY:
			if (!is_empty(*head)) {
				if (delete_action_secondary(&(*head)) != NON) {
					setConsoleColor(2);
					std::cout << "\nУдалено\n" << std::endl;
					setConsoleColor(7);
				}
			}
			break;
		case CURRENT_STATUS:
			if (!is_empty<list_primary>(*head)) current_status(*head);
			break;
		case SEARCH_ELEM:
			if (!is_empty<list_primary>(*head)) {
				int value;
				std::cout << "Введите значение элемента: ";
				std::cin >> value;
				checking_input(value, -1000000, 2147483647);
				if (value == NON) break;
				search_elem(*head, value);
			}
			break;
		}
	}
}



int main()
{
	setlocale(LC_ALL, "ru");

	list_primary* head{ nullptr };

	main_menu(&head);
}