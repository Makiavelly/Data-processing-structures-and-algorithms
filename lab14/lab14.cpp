#include <iostream>
#include <windows.h>
#undef max
#include <string>
#include <iomanip>


enum ACTION_MAIN_MENU {
	NON = -100001, ADD_EDGE = 1, ADD_VERT = 2, DELETE_EDGE = 3, DELETE_VERT = 4, DEPTH_TRAVERSAL = 5, WIDTH_TRAVERSAL = 6, CURRENT_STATUS = 7, CONVERT = 8
};

enum MODE {
	LIST, MATRIX
};

struct list {
	list* prim{ nullptr };
	list* second{ nullptr };
	std::string name;
};

template <typename T>
struct suppotive_list {
	T elem;
	suppotive_list<T>* next;
};

int count_vert{ 0 };
bool mode{ LIST };

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

int get_names_index(std::string* names, const std::string& elem) {
	for (int i = 0; i < count_vert; i++) {
		if (names[i] == elem) return i;
	}
	return NON;
}

list* search_elem(list* temp, const std::string& name) {
	for (int i = 0; i < count_vert; i++) {
		if (temp->prim->name == name) return temp;
		temp = temp->prim;
	}
	return nullptr;
}

void add_edge(list* first_elem, list* second_elem) {
	list* temp{ first_elem };

	list* new_fs_elem{ new list };
	if (!new_fs_elem) {
		std::cerr << "Ошибка выделения памяти!" << std::endl;
		exit(1);
	}
	new_fs_elem->name = second_elem->name;
	new_fs_elem->prim = second_elem;
	new_fs_elem->second = nullptr;

	while (temp->second != nullptr) {
		if (temp->second->name == second_elem->name) return;
		temp = temp->second;
	}
	temp->second = new_fs_elem;

	list* new_sd_elem{ new list };
	if (!new_sd_elem) {
		std::cerr << "Ошибка выделения памяти!" << std::endl;
		exit(1);
	}
	new_sd_elem->name = first_elem->name;
	new_sd_elem->prim = first_elem;
	new_sd_elem->second = nullptr;
	
	temp = second_elem;
	while (temp->second != nullptr) temp = temp->second;
	temp->second = new_sd_elem;
}

void add_edge(int* matrix, int index_fs_elem, int index_sd_elem) {
	matrix[index_fs_elem * count_vert + index_sd_elem] = 1;
	matrix[index_sd_elem * count_vert + index_fs_elem] = 1;
}

bool delete_edge(list* first_elem, list* second_elem) {
	list *temp_fs{ first_elem }, *temp_sd{ second_elem }, *deleted_elem;
	while (temp_fs->second != nullptr && temp_fs->second->prim != second_elem) temp_fs = temp_fs->second;
	if (temp_fs->second == nullptr) return false;
	else {
		deleted_elem = temp_fs->second;
		temp_fs->second = deleted_elem->second;
		delete deleted_elem; deleted_elem = nullptr;
	}

	while (temp_sd->second->prim != first_elem) temp_sd = temp_sd->second;
	deleted_elem = temp_sd->second;
	temp_sd->second = deleted_elem->second;
	delete deleted_elem; deleted_elem = nullptr;

	return true;
}

void delete_edge(int* matrix, int index_fs_elem, int index_sd_elem) {
	matrix[index_fs_elem * count_vert + index_sd_elem] = 0;
	matrix[index_sd_elem * count_vert + index_fs_elem] = 0;
}

void add_vert(list* head, const std::string& name) {
	list* new_elem{ new list };
	if (!new_elem) {
		std::cerr << "Ошибка выделения памяти!" << std::endl;
		exit(1);
	}
	new_elem->name = name;
	new_elem->prim = nullptr;
	new_elem->second = nullptr;

	list* temp{ head };
	while (temp->prim != nullptr) temp = temp->prim;
	temp->prim = new_elem;
	count_vert++;
}

void add_vert(int** matrix) {
	count_vert++;
	int* new_matrix{ new int[count_vert * count_vert] {0} };
	if (!new_matrix) {
		std::cerr << "Ошибка выделения памяти!" << std::endl;
		exit(1);
	}

	for (int i = 0; i < count_vert - 1; i++)
		for (int j = 0; j < count_vert - 1; j++) 
			new_matrix[i * count_vert + j] = (*matrix)[i * (count_vert - 1) + j];
	if(*matrix != nullptr) delete[] (*matrix);
	*matrix = new_matrix;
}

void delete_vert(list* preceding_elem) {
	list *deleted_elem{ preceding_elem->prim }, *temp{ preceding_elem->prim->second }, *for_delete_edge;
	while (temp != nullptr) {
		for_delete_edge = temp->second;
		delete_edge(deleted_elem, temp->prim);
		temp = for_delete_edge;
	}
	preceding_elem->prim = deleted_elem->prim;
	delete deleted_elem; deleted_elem = nullptr;
	count_vert--;
}

void delete_vert(int** matrix, int index_elem) {
	count_vert--;
	if (count_vert == 0) {
		delete[] (*matrix);
		(*matrix) = nullptr;
	}
	else {
		int* new_matrix{ new int[(count_vert * count_vert)] };

		for (int i = 0; i < count_vert; i++)
			for (int j = 0; j < count_vert; j++)
				new_matrix[i * count_vert + j] = (*matrix)[(i >= index_elem ? i + 1 : i) * (count_vert + 1) + (j >= index_elem ? j + 1 : j)];

		delete[](*matrix);
		(*matrix) = new_matrix;
	}

}

template<typename T>
suppotive_list<T>* add_elem_stack(suppotive_list<T>* sp, T elem) {
	suppotive_list<T>* new_elem{ new suppotive_list<T> };
	if (!new_elem) {
		std::cerr << "Ошибка выделения памяти!" << std::endl;
		exit(1);
	}
	new_elem->elem = elem;
	new_elem->next = sp;
	sp = new_elem;
	return sp;
}

template<typename T>
bool search_elem_stack(suppotive_list<T>* sp, T elem) {
	while (sp != nullptr) {
		if (sp->elem == elem) return true;
		else sp = sp->next;
	}
	return false;
}

template<typename T>
suppotive_list<T>* delete_elem_stack(suppotive_list<T>* sp) {
	suppotive_list<T>* temp{ sp };
	sp = sp->next;
	delete temp;
	return sp;
}

void depth_traversal(list* head) {
	suppotive_list<list*>* proc_vert{ nullptr }, *sp{ nullptr };
	list* temp_prim{ head->prim }, *temp_second{ head->prim->second };

	setConsoleColor(2); std::cout << std::endl;

	sp = add_elem_stack<list*>(sp, temp_prim);
	while (sp != nullptr) {
		temp_prim = sp->elem;
		sp = delete_elem_stack<list*>(sp);
		if (!search_elem_stack<list*>(proc_vert, temp_prim)) {
			std::cout << temp_prim->name << " ";
			proc_vert = add_elem_stack<list*>(proc_vert, temp_prim);

			temp_second = temp_prim->second;
			while (temp_second != nullptr) {
				if (!search_elem_stack<list*>(proc_vert, temp_second->prim))	sp = add_elem_stack<list*>(sp, temp_second->prim);
				temp_second = temp_second->second;
			}
		}
	}
	setConsoleColor(7); std::cout << std::endl << std::endl;
}

void depth_traversal(int* matrix, std::string* names, int start_index) {
	suppotive_list<int>* proc_vert{ nullptr }, * sp{ nullptr };
	int temp_prim{ start_index };

	setConsoleColor(2); std::cout << std::endl;

	sp = add_elem_stack<int>(sp, temp_prim);
	while (sp != nullptr) {
		temp_prim = sp->elem;
		sp = delete_elem_stack<int>(sp);
		if (!search_elem_stack<int>(proc_vert, temp_prim)) {
			std::cout << names[temp_prim] << " ";
			proc_vert = add_elem_stack<int>(proc_vert, temp_prim);

			for (int i = 0; i < count_vert; i++)
				if (matrix[temp_prim * count_vert + i] == 1 && !search_elem_stack<int>(proc_vert, i)) sp = add_elem_stack<int>(sp, i);
		}
	}
	setConsoleColor(7); std::cout << std::endl << std::endl;
}

template<typename T>
suppotive_list<T>* add_elem_queue(suppotive_list<T>* queue, T elem) {
	suppotive_list<T>* new_elem{ new suppotive_list<T> };
	if (!new_elem) {
		std::cerr << "Ошибка выделения памяти!" << std::endl;
		exit(1);
	}
	new_elem->elem = elem;
	new_elem->next = nullptr;

	if (queue == nullptr) return new_elem;
	else {
		suppotive_list<T>* temp{ queue };
		while (temp->next != nullptr) temp = temp->next;
		temp->next = new_elem;
		return queue;
	}
}

void width_traversal(list* head) {
	suppotive_list<list*> *proc_vert{ nullptr }, *queue{ nullptr };
	list* temp_prim{ head->prim }, * temp_second{ head->prim->second };

	setConsoleColor(2); std::cout << std::endl;

	queue = add_elem_queue<list*>(queue, temp_prim);
	while (queue != nullptr) {
		temp_prim = queue->elem;
		queue = delete_elem_stack<list*>(queue);
		if (!search_elem_stack<list*>(proc_vert, temp_prim)) {
			std::cout << temp_prim->name << " ";
			proc_vert = add_elem_stack<list*>(proc_vert, temp_prim);

			temp_second = temp_prim->second;
			while (temp_second != nullptr) {
				if (!search_elem_stack<list*>(proc_vert, temp_second->prim)) queue = add_elem_queue<list*>(queue, temp_second->prim);
				temp_second = temp_second->second;
			}
		}
	}
	setConsoleColor(7); std::cout << std::endl << std::endl;
}

void width_traversal(int* matrix, std::string* names, int start_index) {
	suppotive_list<int>* proc_vert{ nullptr }, * queue{ nullptr };
	int temp_prim{ start_index };

	setConsoleColor(2); std::cout << std::endl;

	queue = add_elem_stack<int>(queue, temp_prim);
	while (queue != nullptr) {
		temp_prim = queue->elem;
		queue = delete_elem_stack<int>(queue);
		if (!search_elem_stack<int>(proc_vert, temp_prim)) {
			std::cout << names[temp_prim] << " ";
			proc_vert = add_elem_stack<int>(proc_vert, temp_prim);

			for (int i = 0; i < count_vert; i++)
				if (matrix[temp_prim * count_vert + i] == 1 && !search_elem_stack<int>(proc_vert, i)) queue = add_elem_queue<int>(queue, i);
		}
	}
	setConsoleColor(7); std::cout << std::endl << std::endl;
}

void conver_list_to_matrix(list* head, int** matrix, std::string** names) {
	*matrix = new int[count_vert * count_vert] {0};
	if (!matrix) {
		std::cerr << "Ошибка выделения памяти!" << std::endl;
		exit(1);
	}
	*names = new std::string[count_vert];
	if (!names) {
		std::cerr << "Ошибка выделения памяти!" << std::endl;
		exit(1);
	}
	list* fs_temp{ head->prim }, * sd_temp, * for_find_index_temp, * prim_for_index_temp, * deleted_elem{ nullptr };

	for (int i = 0; i < count_vert; i++) {
		(*names)[i] = fs_temp->name;
		fs_temp = fs_temp->prim;
	}

	fs_temp = head;
	for (int i = 0; i < count_vert; i++) {
		sd_temp = fs_temp->prim->second;
		while (sd_temp != nullptr) {
			int index{ 0 };
			for (; index < count_vert; index++) if ((*names)[index] == sd_temp->name) break;
			add_edge(*matrix, i, index);
			sd_temp = sd_temp->second;
		}
		delete_vert(fs_temp);
		count_vert++;
	}
}

void convert_matrix_to_list(list* head, int** matrix, std::string** names) {
	list* temp_first_elem, * temp_second_elem;

	for (int i = 0; i < count_vert; i++) {
		add_vert(head, (*names)[i]); count_vert--;
	}

	temp_first_elem = head->prim;
	for (int i = 0; i < count_vert; i++) {
		for (int j = 0; j < count_vert; j++) {
			
			if ((*matrix)[i * count_vert + j] == 1) {
				temp_second_elem = head->prim;
				for (int z = 0; z < j; z++) temp_second_elem = temp_second_elem->prim;
				add_edge(temp_first_elem, temp_second_elem);
			}
		}
		temp_first_elem = temp_first_elem->prim;
	}

	delete[] (*matrix); delete[] (*names);
	*matrix = nullptr; *names = nullptr;
}

bool is_empty(list* head) {
	if (head->prim == nullptr) {
		setConsoleColor(4);
		std::cout << "\nГраф пуст\n" << std::endl;
		setConsoleColor(7);
		return true;
	}
	else return false;
}

bool is_empty(int* matrix) {
	if (matrix == nullptr) {
		setConsoleColor(4);
		std::cout << "\nГраф пуст\n" << std::endl;
		setConsoleColor(7);
		return true;
	}
	else return false;
}

void status(list* head) {
	setConsoleColor(2); std::cout << std::endl;
	list* fs_temp{head->prim}, * sd_temp;
	for (int i = 0; i < count_vert; i++) {
		std::cout << "Вершина с именем " << fs_temp->name << " имеет связи с: ";
		sd_temp = fs_temp->second;
		while (sd_temp != nullptr) {
			std::cout << sd_temp->name << " ";
			sd_temp = sd_temp->second;
		}
		std::cout << std::endl;
		fs_temp = fs_temp->prim;
	}
	setConsoleColor(7); std::cout << std::endl;
}

void status(int* matrix, std::string* names) {
	setConsoleColor(2); std::cout << std::endl;
	int max_lenght{ 0 };
	for (int i = 0; i < count_vert; i++) if (names[i].length() > max_lenght) max_lenght = names[i].length();
	std::cout << std::setw(max_lenght + 1) << "";
	for (int i = 0; i < count_vert; i++) std::cout << std::setw(max_lenght + 1) << names[i];
	std::cout << std::endl;
	for (int i = 0; i < count_vert; i++) {
		std::cout << std::setw(max_lenght + 1) << names[i];
		for (int j = 0; j < count_vert; j++) {
			std::cout << std::setw(max_lenght + 1) << matrix[i * count_vert + j];
		}
		std::cout << std::endl;
	}
	setConsoleColor(7); std::cout << std::endl << std::endl;
}

void main_menu(list** head, int** adj_matrix, std::string** names) {
	int ans;

	/*add_vert(*head, "a");
	add_vert(*head, "b");
	add_vert(*head, "c");
	add_vert(*head, "d");
	add_vert(*head, "e");
	add_edge(search_elem(*head, "a")->prim, search_elem(*head, "b")->prim);
	add_edge(search_elem(*head, "a")->prim, search_elem(*head, "c")->prim);
	add_edge(search_elem(*head, "a")->prim, search_elem(*head, "d")->prim);
	add_edge(search_elem(*head, "a")->prim, search_elem(*head, "e")->prim);
	add_edge(search_elem(*head, "b")->prim, search_elem(*head, "e")->prim);
	add_edge(search_elem(*head, "c")->prim, search_elem(*head, "d")->prim);*/

	while (true) {
		std::cout << "Режим: ";
		if (mode) std::cout << "матрица смежности\n";
		else std::cout << "список вершин\n";
		std::cout << "Выберите действие:\n1. Добавить ребро\n2. Добавить вершину\n3. Удалить ребро\n4. Удалить вершину\n5. Обход графа в глубину\n6. Обход графа в ширину\n7. Вывод состояния\n";
		if (mode) std::cout << "8. Преобразовать матрицу в список\n";
		else std::cout << "8. Преобразовать список в матрицу\n";
		std::cout << "Ожидание ответа: ";
		std::cin >> ans;

		checking_input(ans, 1, 8);

		switch (ans) {
		case ADD_EDGE: 
			if (mode) {
				if (!is_empty(*adj_matrix)) {
					std::string fs_name, sd_name;
					int fs_index, sd_index;
					std::cout << "Введите имя первой вершины: ";
					std::getline(std::cin, fs_name);
					fs_index = get_names_index(*names, fs_name);
					if (fs_index == NON) {
						setConsoleColor(4);
						std::cout << "\nВершина не найдена\n\n";
						setConsoleColor(7);
						break;
					}

					std::cout << "Введите имя второй вершины: ";
					std::getline(std::cin, sd_name);
					sd_index = get_names_index(*names, sd_name);
					if (sd_index == NON) {
						setConsoleColor(4);
						std::cout << "\nВершина не найдена\n\n";
						setConsoleColor(7);
						break;
					}

					add_edge(*adj_matrix, fs_index, sd_index);

					setConsoleColor(2);
					std::cout << "\nРебро добавлено\n\n";
					setConsoleColor(7);
				}
			}
			else {
				if (!is_empty(*head)) {
					std::string fs_name, sd_name;
					list* fs_temp, * sd_temp;
					std::cout << "Введите имя первой вершины: ";
					std::getline(std::cin, fs_name);
					fs_temp = search_elem(*head, fs_name);
					fs_temp = fs_temp->prim;
					if (fs_temp == nullptr) {
						setConsoleColor(4);
						std::cout << "\nВершина не найдена\n\n";
						setConsoleColor(7);
						break;
					}

					std::cout << "Введите имя второй вершины: ";
					std::getline(std::cin, sd_name);
					sd_temp = search_elem(*head, sd_name);
					sd_temp = sd_temp->prim;
					if (sd_temp == nullptr) {
						setConsoleColor(4);
						std::cout << "\nВершина не найдена\n\n";
						setConsoleColor(7);
						break;
					}

					add_edge(fs_temp, sd_temp);

					setConsoleColor(2);
					std::cout << "\nРебро добавлено\n\n";
					setConsoleColor(7);
				}
			}
			break;
		case ADD_VERT:
			if (mode) {
				std::string name;
				std::cout << "Дайте название вершине: ";
				std::getline(std::cin, name);

				if (get_names_index(*names, name) == NON) {
					std::string* temp{ new std::string[count_vert + 1] };
					for (int i = 0; i < count_vert; i++) temp[i] = (*names)[i];
					temp[count_vert] = name;
					delete[] (*names);
					*names = temp;

					add_vert(adj_matrix);
					setConsoleColor(2);
					std::cout << "\nНовая вершина добавлена\n" << std::endl;
					setConsoleColor(7);
				}
			}
			else {
				std::string name;
				std::cout << "Дайте название вершине: ";
				std::getline(std::cin, name);

				if (search_elem(*head, name) == nullptr) {
					add_vert(*head, name);
					setConsoleColor(2);
					std::cout << "\nНовая вершина добавлена\n" << std::endl;
					setConsoleColor(7);
				}
				else {
					setConsoleColor(4);
					std::cout << "\nВеришина с таким названием уже существует\n" << std::endl;
					setConsoleColor(7);
					break;
				}
			}
			break;
		case DELETE_EDGE:
			if (mode) {
				if (!is_empty(*adj_matrix)) {
					std::string fs_name, sd_name;
					int fs_index, sd_index;
					std::cout << "Введите имя первой вершины: ";
					std::getline(std::cin, fs_name);
					fs_index = get_names_index(*names, fs_name);
					if (fs_index == NON) {
						setConsoleColor(4);
						std::cout << "\nВершина не найдена\n\n";
						setConsoleColor(7);
						break;
					}

					std::cout << "Введите имя второй вершины: ";
					std::getline(std::cin, sd_name);
					sd_index = get_names_index(*names, sd_name);
					if (sd_index == NON) {
						setConsoleColor(4);
						std::cout << "\nВершина не найдена\n\n";
						setConsoleColor(7);
						break;
					}

					delete_edge(*adj_matrix, fs_index, sd_index);

					setConsoleColor(2);
					std::cout << "\nРебро удалено\n\n";
					setConsoleColor(7);
				}
			}
			else {
				if (!is_empty(*head)) {
					std::string fs_name, sd_name;
					list* fs_temp, * sd_temp;
					std::cout << "Введите имя первой вершины: ";
					std::getline(std::cin, fs_name);
					fs_temp = search_elem(*head, fs_name);
					fs_temp = fs_temp->prim;
					if (fs_temp == nullptr) {
						setConsoleColor(4);
						std::cout << "\nВершина не найдена\n\n";
						setConsoleColor(7);
						break;
					}

					std::cout << "Введите имя второй вершины: ";
					std::getline(std::cin, sd_name);
					sd_temp = search_elem(*head, sd_name);
					sd_temp = sd_temp->prim;
					if (sd_temp == nullptr) {
						setConsoleColor(4);
						std::cout << "\nВершина не найдена\n\n";
						setConsoleColor(7);
						break;
					}
					if (delete_edge(fs_temp, sd_temp)) {
						setConsoleColor(2);
						std::cout << "\nРебро удалено\n" << std::endl;
						setConsoleColor(7);
					}
					else {
						setConsoleColor(4);
						std::cout << "\nРебра между вершинами не обнаружено\n" << std::endl;
						setConsoleColor(7);
					}
				}
			}
			break;
		case DELETE_VERT:
			if (mode) {
				if (!is_empty(*adj_matrix)) {
					int index_vert;
					std::string name;
					std::cout << "Название удаляемой вершины: ";
					std::getline(std::cin, name);

					index_vert = get_names_index(*names, name);
					if (index_vert == NON) {
						setConsoleColor(4);
						std::cout << "\nВершина не найдена\n\n";
						setConsoleColor(7);
						break;
					}
					else {
						std::string* temp{ new std::string[count_vert - 1] };
						for (int i = 0; i < count_vert - 1; i++) temp[i] = (*names)[i >= index_vert ? i+1 : i];
						temp[count_vert] = name;
						delete[](*names);
						*names = temp;

						delete_vert(adj_matrix, index_vert);
					}

					setConsoleColor(2);
					std::cout << "\nВершина удалена\n\n";
					setConsoleColor(7);
				}
			}
			else {
				if (!is_empty(*head)) {
					list* preceding_vert;
					std::string name;
					std::cout << "Название удаляемой вершины: ";
					std::getline(std::cin, name);

					preceding_vert = search_elem(*head, name);
					if (preceding_vert == nullptr) {
						setConsoleColor(4);
						std::cout << "\nВершина не найдена\n\n";
						setConsoleColor(7);
						break;
					}
					else delete_vert(preceding_vert);

					setConsoleColor(2);
					std::cout << "\nВершина удалена\n\n";
					setConsoleColor(7);
				}
			}
			break;
		case DEPTH_TRAVERSAL:
			if (mode) {
				if (!is_empty(*adj_matrix)) {
					std::string name;
					int index_vert;
					std::cout << "Введите наименование вершины, от которой надо начать обход: ";
					std::getline(std::cin, name);
					index_vert = get_names_index(*names, name);
					if (index_vert == NON) {
						setConsoleColor(4);
						std::cout << "\nВершина не найдена\n\n";
						setConsoleColor(7);
						break;
					}
					else depth_traversal(*adj_matrix, *names, index_vert);
				}
			}
			else {
				if (!is_empty(*head)) {
					list* elem;
					std::string name;
					std::cout << "Введите наименование вершины, от которой надо начать обход: ";
					std::getline(std::cin, name);
					elem = search_elem(*head, name);
					if (elem == nullptr) {
						setConsoleColor(4);
						std::cout << "\nВершина не найдена\n\n";
						setConsoleColor(7);
						break;
					}
					else depth_traversal(search_elem(elem, name));
				}
			}
			break;
		case WIDTH_TRAVERSAL:
			if (mode) {
				if (!is_empty(*adj_matrix)) {
					std::string name;
					int index_vert;
					std::cout << "Введите наименование вершины, от которой надо начать обход: ";
					std::getline(std::cin, name);
					index_vert = get_names_index(*names, name);
					if (index_vert == NON) {
						setConsoleColor(4);
						std::cout << "\nВершина не найдена\n\n";
						setConsoleColor(7);
						break;
					}
					else width_traversal(*adj_matrix, *names, index_vert);
				}
			}
			else {
				if (!is_empty(*head)) {
					list* elem;
					std::string name;
					std::cout << "Введите наименование вершины, от которой надо начать обход: ";
					std::getline(std::cin, name);
					elem = search_elem(*head, name);
					if (search_elem(*head, name) == nullptr) {
						setConsoleColor(4);
						std::cout << "\nВершина не найдена\n\n";
						setConsoleColor(7);
						break;
					}
					else width_traversal(search_elem(*head, name));
				}
			}
			break;
		case CURRENT_STATUS:
			if (mode) {
				if (!is_empty(*adj_matrix)) {
					status(*adj_matrix, *names);
				}
			}
			else {
				if (!is_empty(*head)) {
					status(*head);
				}
			}
			break;
		case CONVERT:
			if (mode) {
				if (!is_empty(*adj_matrix)) {
					convert_matrix_to_list(*head, adj_matrix, names);
					mode = LIST;

					setConsoleColor(2);
					std::cout << "\nПреобразовано в список\n" << std::endl;
					setConsoleColor(7);
				}
				else {
					mode = LIST;
					setConsoleColor(2);
					std::cout << "\nРежим изменён\n" << std::endl;
					setConsoleColor(7);
				}
			}
			else {
				if (!is_empty(*head)) {
					conver_list_to_matrix(*head, adj_matrix, names);
					mode = MATRIX;

					setConsoleColor(2);
					std::cout << "\nПреобразовано в матрицу смежности\n" << std::endl;
					setConsoleColor(7);
				}
				else {
					mode = MATRIX;
					setConsoleColor(2);
					std::cout << "\nРежим изменён\n" << std::endl;
					setConsoleColor(7);
				}
			}
			break;
		}
	}
}

list* init_list() {
	list* head{ new list };
	if (!head) {
		std::cerr << "Ошибка выделения памяти!" << std::endl;
		exit(1);
	}

	head->prim = nullptr;
	head->second = nullptr;
	return head;
}

int main()
{
	setlocale(LC_ALL, "ru");

	list* head{ init_list() };
	int* adj_matrix{ nullptr };
	std::string* names{ nullptr };

	main_menu(&head, &adj_matrix, &names);
}