#include <iostream>
#include <windows.h>
#undef max

enum ACTION_MAIN_MENU {
	NON = -100001, ADD_ELEM = 1, DELETE_ELEM = 2, CURRENT_STATUS = 3, SEARCH_ELEM = 4
};

struct trees_elem {
	int data;
	trees_elem *next_parent, *next_child;
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

void current_status(trees_elem* temp_parent) {
	setConsoleColor(2);
	std::cout << std::endl;
	trees_elem* temp_child;
	int counter_parent{ 0 }, counter_child{ 0 };

	while (temp_parent != nullptr) {
		std::cout << counter_parent << "-й родитель со значением: " << temp_parent->data << std::endl;
		temp_child = temp_parent->next_child;
		while (temp_child != nullptr) {
			std::cout << '\t' << counter_child << "-й потомок со значением: " << temp_child->data << std::endl;
			temp_child = temp_child->next_child; counter_child++;
		}
		temp_parent = temp_parent->next_parent; counter_parent++; counter_child = 0;
	}
	setConsoleColor(7);
	std::cout << std::endl;
}

void* search_elem(trees_elem* root, int data) {
	void* ptr = malloc(2 * sizeof(trees_elem*));
	if (root->data == data) {
		setConsoleColor(2);
		std::cout << "\nЭлемент найден - является корнем дерева\n" << std::endl;
		setConsoleColor(7);
		return ptr;
	}

	trees_elem* temp_child, *temp_parent{ root };
	if (!ptr) {
		std::cerr << "Ошибка выделения памяти!" << std::endl;
		exit(1);
	}
	int counter_parent{ 0 }, counter_child{ 0 };
	
	while (temp_parent != nullptr) {
		temp_child = temp_parent;
		while (temp_child->next_child != nullptr) {
			if (temp_child->next_child->data == data) {
				setConsoleColor(2);
				std::cout << "\nЭлемент найден - является " << counter_child << " потомком " << counter_parent << " вершины\n" << std::endl;
				setConsoleColor(7);
				trees_elem** child_part{ (trees_elem**)ptr };
				*child_part = temp_child;
				trees_elem** parent_part{ (trees_elem**)((char*)ptr + sizeof(trees_elem*)) };
				*parent_part = temp_parent;
				return ptr;
			}
			temp_child = temp_child->next_child; counter_child++;
		}
		temp_parent = temp_parent->next_parent; counter_parent++; counter_child = 0;
	}
	setConsoleColor(4);
	std::cout << "\nЭлемент не найден\n" << std::endl;
	setConsoleColor(7);
	free(ptr);
	return nullptr;
}

void add_cell(trees_elem* temp_parent, trees_elem* found_elem, int data) {
	if (found_elem->next_parent == nullptr) {
		trees_elem* new_child{ (trees_elem*)malloc(sizeof(trees_elem)) };
		if (!new_child) {
			std::cerr << "Ошибка выделения памяти!" << std::endl;
			exit(1);
		}
		new_child->data = data;
		new_child->next_child = nullptr;
		new_child->next_parent = nullptr;

		while (temp_parent->next_parent != nullptr) temp_parent = temp_parent->next_parent;
		trees_elem* new_elem{ (trees_elem*)malloc(sizeof(trees_elem)) };
		if (!new_elem) {
			std::cerr << "Ошибка выделения памяти!" << std::endl;
			exit(1);
		}
		
		new_elem->next_parent = nullptr;
		new_elem->next_child = new_child;
		new_elem->data = found_elem->data;
		temp_parent->next_parent = new_elem;
		found_elem->next_parent = new_elem;
	}
	else {
		trees_elem* new_elem{ (trees_elem*)malloc(sizeof(trees_elem)) };
		if (!new_elem) {
			std::cerr << "Ошибка выделения памяти!" << std::endl;
			exit(1);
		}
		new_elem->data = data;
		new_elem->next_child = found_elem->next_parent->next_child;
		new_elem->next_parent = nullptr;
		found_elem->next_parent->next_child = new_elem;
	}

}

void delete_cell(trees_elem* previous_elem, trees_elem* parent_elem) {
	trees_elem* temp_delete{ previous_elem->next_child };
	previous_elem->next_child = temp_delete->next_child;
	if (temp_delete->next_parent != nullptr) {
		trees_elem* temp_replace{ temp_delete->next_parent };

		while (temp_replace->next_child != nullptr) {
			trees_elem* for_replace_one{ temp_replace->next_child }, * for_replace_two{ parent_elem->next_child };
			temp_replace->next_child = for_replace_one->next_child;
			parent_elem->next_child = for_replace_one;
			for_replace_one->next_child = for_replace_two;
		}

		trees_elem* previous_parent_elem{ parent_elem };
		while (previous_parent_elem->next_parent != temp_replace) previous_parent_elem = previous_parent_elem->next_parent;
		previous_parent_elem->next_parent = temp_replace->next_parent;
		free(temp_replace); temp_replace = nullptr;
	}

	free(temp_delete); temp_delete = nullptr;
}

void main_menu(trees_elem** head){
	int ans;
	
	while (true) {
		std::cout << "Выберите действие:\n1.Добавить элемент\n2.Удалить элемент\n3.Вывести текущее состояние\n4.Поиск элемента" << std::endl;
		std::cout << "Ожидание ответа: ";
		std::cin >> ans;
		checking_input(ans, 1, 4);

		switch(ans) {
		case ADD_ELEM:
			int target_value, value;
			std::cout << "Введите значение вершины, потомоком которой будет новая вершина: ";
			std::cin >> target_value;
			checking_input(target_value, -100000, 2147483647);
			if (target_value == NON) break;

			if ((*head)->data == target_value) {
				setConsoleColor(2);
				std::cout << "\nЭлемент найден - является корнем дерева\n" << std::endl;
				setConsoleColor(7);

				trees_elem* new_elem{ (trees_elem*)malloc(sizeof(trees_elem)) };
				if (!new_elem) {
					std::cerr << "Ошибка выделения памяти!" << std::endl;
					exit(1);
				}

				std::cout << "Введите значение новой вершины: ";
				std::cin >> value;
				checking_input(value, -100000, 2147483647);
				if (value == NON) break;

				new_elem->data = value;
				new_elem->next_parent = nullptr;
				new_elem->next_child = (*head)->next_child;
				(*head)->next_child = new_elem;
				setConsoleColor(2);
				std::cout << "\nДобавлено\n" << std::endl;
				setConsoleColor(7);
			}
			else {
				void* ptr{ search_elem(*head, target_value) };
				trees_elem** child_part{ (trees_elem**)ptr };
				if (child_part == nullptr) break;
				*child_part = (*child_part)->next_child;

				std::cout << "Введите значение новой вершины: ";
				std::cin >> value;
				checking_input(value, -100000, 2147483647);
				if (value == NON) break;

				add_cell(*head, *child_part, value);

				free(ptr);
				setConsoleColor(2);
				std::cout << "\nДобавлено\n" << std::endl;
				setConsoleColor(7);
			}
			break;
		case DELETE_ELEM:
		{
			int target_value;
			std::cout << "Введите значение удаляемой вершины: ";
			std::cin >> target_value;
			checking_input(target_value, -100000, 2147483647);
			if (target_value == NON) break;
			if ((*head)->data == target_value) {
				setConsoleColor(4);
				std::cout << "\nУдаление корня запрещено\n" << std::endl;
				setConsoleColor(7);
				break;
			}

			void* ptr{ search_elem(*head, target_value) };
			if (ptr == nullptr) break;
			trees_elem** child_part{ (trees_elem**)ptr };
			trees_elem** parent_part{ (trees_elem**)((char*)ptr + sizeof(trees_elem*)) };
			
			delete_cell(*child_part, *parent_part);

			free(ptr);
			setConsoleColor(2);
			std::cout << "\nУдалено\n" << std::endl;
			setConsoleColor(7);
		}
			break;
		case CURRENT_STATUS:
			current_status(*head);
			break;
		case SEARCH_ELEM:
		{
			int target_value;
			std::cout << "Введите значение вершины: ";
			std::cin >> target_value;
			checking_input(target_value, -100000, 2147483647);
			if (target_value == NON) break;
			void* ptr{ search_elem(*head, target_value) };
			free(ptr);
		}
			break;
		}
	}
}

trees_elem* init() {
	int data;
	while (true) {
		std::cout << "Введите значение корня: ";
		std::cin >> data;
		checking_input(data, -100000, 2147483647);
		if (data != NON) break;
	}

	trees_elem* temp_parent{ (trees_elem*)malloc(sizeof(trees_elem)) };
	if (!temp_parent) {
		std::cerr << "Ошибка выделения памяти!" << std::endl;
		exit(1);
	}
	temp_parent->data = data;
	temp_parent->next_parent = nullptr;
	temp_parent->next_child = nullptr;
	return temp_parent;
}

int main()
{
	setlocale(LC_ALL, "ru");

	trees_elem* head{ init() };

	main_menu(&head);
}