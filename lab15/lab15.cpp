#include <iostream>
#include <windows.h>
#undef max
#include <chrono>
#include <string>

typedef std::chrono::time_point<std::chrono::high_resolution_clock> type_time;

enum ACNTION_MENU {
	NON = -100001, EXCHANGE = 1, INSERT = 2, SELECTION = 3, SHELL = 4, FAST = 5, PYRAMIDAL = 6, SIMPLEST_POCKET_ARR = 7, SIMPLEST_POCKET_NOT_ARR = 8, GENERALIZED_POCKET = 9, BY_CLASS = 10
};

struct Specif {
	int compare, swap, time;
};

struct hist {
	std::string method;
	hist* next;
	Specif* inf_10;
	Specif* inf_100;
	Specif* inf_1000;
	Specif* inf_10000;
};

void add_elem(hist* head, Specif* inf_10, Specif* inf_100, Specif* inf_1000, Specif* inf_10000, std::string method) {
	hist* new_elem{ new hist };
	if (!new_elem) {
		std::cerr << "Ошибка выделения памяти!" << std::endl;
		exit(1);
	}
	new_elem->method = method;
	new_elem->inf_10 = inf_10;
	new_elem->inf_100 = inf_100;
	new_elem->inf_1000 = inf_1000;
	new_elem->inf_10000 = inf_10000;
	new_elem->next = nullptr;

	hist* temp{ head };
	while (temp->next != nullptr) temp = temp->next;
	temp->next = new_elem;
}

void output_hist(hist* head) {
	hist* temp{ head->next };
	while (temp != nullptr) {
		std::cout << \
			"\n" << temp->method << \
			"\n10 элементов:" << \
			"\n\tКол-во сравнений = " << temp->inf_10->compare << \
			"\n\tКол-во перестановок = " << temp->inf_10->swap << \
			"\n\tВремя выполнения = " << temp->inf_10->time << \
			"\n100 элементво: " << \
			"\n\tКол-во сравнений = " << temp->inf_100->compare << \
			"\n\tКол-во перестановок = " << temp->inf_100->swap << \
			"\n\tВремя выполнения = " << temp->inf_100->time << \
			"\n1тыс. элементов: " << \
			"\n\tКол-во сравнений = " << temp->inf_1000->compare << \
			"\n\tКол-во перестановок = " << temp->inf_1000->swap << \
			"\n\tВремя выполнения = " << temp->inf_1000->time << \
			"\n10тыс. элементов: " << \
			"\n\tКол-во сравнений = " << temp->inf_10000->compare << \
			"\n\tКол-во перестановок = " << temp->inf_10000->swap << \
			"\n\tВремя выполнения = " << temp->inf_10000->time << std::endl << std::endl;
		temp = temp->next;
	}
}

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

Specif* exchange_method(int* orig_arr, int N) {
	int* arr{ new int[N] };
	if (!arr) {
		std::cerr << "Ошибка выделения памяти!" << std::endl;
		exit(1);
	}
	std::copy(orig_arr, orig_arr + N, arr);

	setConsoleColor(2); std::cout << std::endl;
	std::cout << "Было: " << std::endl;
	for (int i = 0; i < N; i++)
		std::cout << arr[i] << " ";
	std::cout << std::endl;
	std::cout << "Стало: " << std::endl;

	int compare{ 0 }, swap{ 0 };
	type_time start, end;
	start = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < N - 1; i++) {
		for (int j = N - 1; j > i; j--) {
			if (arr[j - 1] > arr[j]) {
				int temp = arr[j];
				arr[j] = arr[j - 1];
				arr[j - 1] = temp;
				swap++;
			}
			compare++;
		}
	}

	end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

	for (int i = 0; i < N; i++)
		std::cout << arr[i] << " ";
	setConsoleColor(7); std::cout << std::endl;
	delete[] arr;

	Specif* result{ new Specif };
	if (!result) {
		std::cerr << "Ошибка выделения памяти!" << std::endl;
		exit(1);
	}
	result->compare = compare;
	result->swap = swap;
	result->time = (int)std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	return result;
}

Specif* insert_method(int* orig_arr, int N) {
	int* arr{ new int[N] };
	if (!arr) {
		std::cerr << "Ошибка выделения памяти!" << std::endl;
		exit(1);
	}
	std::copy(orig_arr, orig_arr + N, arr);

	setConsoleColor(2); std::cout << std::endl;
	std::cout << "Было: " << std::endl;
	for (int i = 0; i < N; i++)
		std::cout << arr[i] << " ";
	std::cout << std::endl;
	std::cout << "Стало: " << std::endl;

	int compare{ 0 }, swap{ 0 }, temp;
	type_time start, end;
	start = std::chrono::high_resolution_clock::now();

	for (int i = 1; i < N; i++) {
		temp = arr[i];
		int j = i - 1;
		for (; j > -1; j--) {
			compare++;
			if (arr[j] > temp) {
				arr[j + 1] = arr[j];
				swap++;
			}
			else break;
		}
		arr[j + 1] = temp;
	}

	end = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++)
		std::cout << arr[i] << " ";
	setConsoleColor(7); std::cout << std::endl;
	delete[] arr;

	Specif* result{ new Specif };
	if (!result) {
		std::cerr << "Ошибка выделения памяти!" << std::endl;
		exit(1);
	}
	result->compare = compare;
	result->swap = swap;
	result->time = (int)std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	return result;
}

Specif* selection_method(int* orig_arr, int N) {
	int* arr{ new int[N] };
	if (!arr) {
		std::cerr << "Ошибка выделения памяти!" << std::endl;
		exit(1);
	}
	std::copy(orig_arr, orig_arr + N, arr);

	setConsoleColor(2); std::cout << std::endl;
	std::cout << "Было: " << std::endl;
	for (int i = 0; i < N; i++)
		std::cout << arr[i] << " ";
	std::cout << std::endl;
	std::cout << "Стало: " << std::endl;

	int compare{ 0 }, swap{ 0 };
	type_time start, end;
	start = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < N - 1; i++) {
		int min_index{ i };
		for (int j = i + 1; j < N; j++) {
			if (arr[j] < arr[min_index]) min_index = j;
			compare++;
		}
		if (min_index != i) {
			int temp{ arr[i] };
			arr[i] = arr[min_index];
			arr[min_index] = temp;
			swap++;
		}
	}

	end = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++)
		std::cout << arr[i] << " ";
	setConsoleColor(7); std::cout << std::endl;
	delete[] arr;

	Specif* result{ new Specif };
	if (!result) {
		std::cerr << "Ошибка выделения памяти!" << std::endl;
		exit(1);
	}
	result->compare = compare;
	result->swap = swap;
	result->time = (int)std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	return result;
}

Specif* shell_method(int* orig_arr, int N, int num_steps) {
	int* arr{ new int[N] };
	if (!arr) {
		std::cerr << "Ошибка выделения памяти!" << std::endl;
		exit(1);
	}
	std::copy(orig_arr, orig_arr + N, arr);

	int* arr_steps{ new int[num_steps] };
	if (!arr_steps) {
		std::cerr << "Ошибка выделения памяти!" << std::endl;
		exit(1);
	}
	arr_steps[0] = 1;
	for (int i = 1; i < num_steps; i++) {
		arr_steps[i] = arr_steps[i - 1] * 2 + 1;
	}

	setConsoleColor(2); std::cout << std::endl;
	std::cout << "Было: " << std::endl;
	for (int i = 0; i < N; i++)
		std::cout << arr[i] << " ";
	std::cout << std::endl;
	std::cout << "Стало: " << std::endl;

	int compare{ 0 }, swap{ 0 }, step, temp;
	type_time start, end;
	start = std::chrono::high_resolution_clock::now();

	for (int m = num_steps - 1; m > -1; m--) {
		step = arr_steps[m];
		for (int i = step; i < N; i++) {
			temp = arr[i];
			int j = i - step;
			for (; j > -1; j -= step) {
				compare++;
				if (arr[j] > temp) {
					arr[j + step] = arr[j];
					swap++;
				}
				else break;
			}
			arr[j + step] = temp;
		}
	}

	end = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++)
		std::cout << arr[i] << " ";
	setConsoleColor(7); std::cout << std::endl;
	delete[] arr;

	Specif* result{ new Specif };
	if (!result) {
		std::cerr << "Ошибка выделения памяти!" << std::endl;
		exit(1);
	}
	result->compare = compare;
	result->swap = swap;
	result->time = (int)std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	return result;
}


struct stack {
	stack* next;
	int left;
	int right;
};

stack* add_elem(stack* sp, int left, int right) {
	stack* new_elem{ new stack };
	if (!new_elem) {
		std::cerr << "Ошибка выделения памяти!" << std::endl;
		exit(1);
	}
	new_elem->left = left;
	new_elem->right = right;
	new_elem->next = sp;
	return new_elem;
}

stack* delete_elem(stack* sp) {
	stack* temp{ sp->next };
	delete sp;
	return temp;
}

void fast_method(int* arr, int left, int right, int& compare, int& swap) {
	int i{ left }, j{ right };
	int sred{ arr[(left + right) / 2] }, temp;
	do {
		while (sred > arr[i]) {
			compare++; i++;
		}
		compare++;
		while (arr[j] > sred) {
			compare++; j--;
		}
		compare++;

		if (i <= j) {
			temp = arr[i];
			arr[i] = arr[j];
			arr[j] = temp;
			swap++; i++; j--;
		}
	} while (j >= i);

	if (j > left) fast_method(arr, left, j, compare, swap);
	if (right > i) fast_method(arr, i, right, compare, swap);
}

Specif* wrapper_fast_method(int* orig_arr, int N) {
	int* arr{ new int[N] };
	if (!arr) {
		std::cerr << "Ошибка выделения памяти!" << std::endl;
		exit(1);
	}
	std::copy(orig_arr, orig_arr + N, arr);

	setConsoleColor(2); std::cout << std::endl;
	std::cout << "Было: " << std::endl;
	for (int i = 0; i < N; i++)
		std::cout << arr[i] << " ";
	std::cout << std::endl;
	std::cout << "Стало: " << std::endl;

	int compare{ 0 }, swap{ 0 };
	type_time start, end;
	start = std::chrono::high_resolution_clock::now();

	fast_method(arr, 0, N - 1, compare, swap);

	end = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++)
		std::cout << arr[i] << " ";
	setConsoleColor(7); std::cout << std::endl;
	delete[] arr;

	Specif* result{ new Specif };
	if (!result) {
		std::cerr << "Ошибка выделения памяти!" << std::endl;
		exit(1);
	}
	result->compare = compare;
	result->swap = swap;
	result->time = (int)std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	return result;
}

void sito(int* arr, int left, int right, int& compare, int& swap) {
	int i{ left }, j{ 2 * left }, x{ arr[left - 1] };
	if (j < right && arr[j - 1 + 1] > arr[j - 1]) j++;
	compare++;
	while (j <= right && arr[j - 1] > x) {
		compare++; swap++;
		arr[i - 1] = arr[j - 1];
		i = j;
		j = 2 * j;
		if (j < right && arr[j - 1 + 1] > arr[j - 1]) j++;
		compare++;
	}
	arr[i - 1] = x;
}

Specif* pyramidal_method(int* orig_arr, int N) {
	int* arr{ new int[N] };
	if (!arr) {
		std::cerr << "Ошибка выделения памяти!" << std::endl;
		exit(1);
	}
	std::copy(orig_arr, orig_arr + N, arr);

	setConsoleColor(2); std::cout << std::endl;
	std::cout << "Было: " << std::endl;
	for (int i = 0; i < N; i++)
		std::cout << arr[i] << " ";
	std::cout << std::endl;
	std::cout << "Стало: " << std::endl;

	int left{ N / 2 }, right{ N }, temp;
	int compare{ 0 }, swap{ 0 };
	type_time start, end;
	start = std::chrono::high_resolution_clock::now();

	while (left > 1) {
		left--;
		sito(arr, left, right, compare, swap);
	}
	while (right > 1) {
		temp = arr[0];
		arr[0] = arr[right - 1];
		arr[right - 1] = temp;
		right--;
		sito(arr, left, right, compare, swap);
	}

	end = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++)
		std::cout << arr[i] << " ";
	setConsoleColor(7); std::cout << std::endl;
	delete[] arr;

	Specif* result{ new Specif };
	if (!result) {
		std::cerr << "Ошибка выделения памяти!" << std::endl;
		exit(1);
	}
	result->compare = compare;
	result->swap = swap;
	result->time = (int)std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	return result;
}

Specif* simplest_pocket_method_arr(int* orig_arr, int N) {
	int* arr{ new int[N] };
	if (!arr) {
		std::cerr << "Ошибка выделения памяти!" << std::endl;
		exit(1);
	}
	std::copy(orig_arr, orig_arr + N, arr);

	int* sup_arr{ new int[N] };
	if (!sup_arr) {
		std::cerr << "Ошибка выделения памяти!" << std::endl;
		exit(1);
	}

	setConsoleColor(2); std::cout << std::endl;
	std::cout << "Было: " << std::endl;
	for (int i = 0; i < N; i++)
		std::cout << arr[i] << " ";
	std::cout << std::endl;
	std::cout << "Стало: " << std::endl;

	type_time start, end;
	start = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < N; i++) 
		sup_arr[arr[i]] = arr[i];

	end = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++)
		std::cout << sup_arr[i] << " ";
	setConsoleColor(7); std::cout << std::endl;
	delete[] arr; delete[] sup_arr;

	Specif* result{ new Specif };
	if (!result) {
		std::cerr << "Ошибка выделения памяти!" << std::endl;
		exit(1);
	}
	result->compare = 0;
	result->swap = N / 3;
	result->time = (int)std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	return result;
}

Specif* simplest_pocket_method_not_arr(int* orig_arr, int N) {
	int* arr{ new int[N] };
	if (!arr) {
		std::cerr << "Ошибка выделения памяти!" << std::endl;
		exit(1);
	}
	std::copy(orig_arr, orig_arr + N, arr);

	setConsoleColor(2); std::cout << std::endl;
	std::cout << "Было: " << std::endl;
	for (int i = 0; i < N; i++)
		std::cout << arr[i] << " ";
	std::cout << std::endl;
	std::cout << "Стало: " << std::endl;

	int temp;
	int compare{ 0 }, swap{ 0 };
	type_time start, end;
	start = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < N; i++) {
		while (arr[i] != i) {
			compare++;
			swap++;
			temp = arr[i];
			arr[i] = arr[temp];
			arr[temp] = temp;
		}
		compare++;
	}

	end = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++)
		std::cout << arr[i] << " ";
	setConsoleColor(7); std::cout << std::endl;
	delete[] arr;

	Specif* result{ new Specif };
	if (!result) {
		std::cerr << "Ошибка выделения памяти!" << std::endl;
		exit(1);
	}
	result->compare = compare;
	result->swap = swap;
	result->time = (int)std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	return result;
}

struct list {
	list* next;
	int data;
};

struct Connect {
	list* first{ nullptr };
	list* last{ nullptr };
};

void add_elem(Connect* head, int data) {
	list* new_elem{ new list };
	if (!new_elem) {
		std::cerr << "Ошибка выделения памяти!" << std::endl;
		exit(1);
	}
	new_elem->data = data;
	new_elem->next = nullptr;
	if (head->last != nullptr) head->last->next = new_elem;
	head->last = new_elem;
	if (head->first == nullptr) head->first = new_elem;
}

void add_elem_in_existing_list(Connect* head, int data) {
	list* temp;
	if (head->last == nullptr) temp = head->first;
	else temp = head->last->next;
	temp->data = data;
	head->last = temp;
}

void create_elem(Connect* head) {
	list* new_elem{ new list };
	if (!new_elem) {
		std::cerr << "Ошибка выделения памяти!" << std::endl;
		exit(1);
	}
	new_elem->data = -1;
	new_elem->next = head->first;
	head->first = new_elem;
}

void delete_elem(Connect* head) {
	list* temp{ head->first->next };
	delete head->first;
	head->first = temp;
}

Specif* generalized_pocket_method(int* orig_arr, int N) {
	int* arr{ new int[N] };
	if (!arr) {
		std::cerr << "Ошибка выделения памяти!" << std::endl;
		exit(1);
	}
	std::copy(orig_arr, orig_arr + N, arr);

	Connect* sup_arr{ new Connect[N] };
	if (!sup_arr) {
		std::cerr << "Ошибка выделения памяти!" << std::endl;
		exit(1);
	}
	for (int i = 0; i < N; i++)
		for (int j = 0; j < 10; j++) create_elem(&sup_arr[i]);

	setConsoleColor(2); std::cout << std::endl;
	std::cout << "Было: " << std::endl;
	for (int i = 0; i < N; i++)
		std::cout << arr[i] << " ";
	std::cout << std::endl;
	std::cout << "Стало: " << std::endl;

	int compare{ 0 }, swap{ 0 }, assign{ 0 };
	type_time start, end;
	start = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < N; i++) {
		assign++;
		add_elem_in_existing_list(&sup_arr[arr[i]], arr[i]);
	}

	end = std::chrono::high_resolution_clock::now();
	swap = assign / 3;
	int j{ 0 }, current_index{ 0 };
	list* temp;
	while (j < N) {
		if (sup_arr[current_index].last != nullptr) {
			temp = sup_arr[current_index].first;

			while (temp != sup_arr[current_index].last->next) {
				swap++;
				arr[j] = temp->data;
				temp = temp->next;
				j++;
			}
		}
		current_index++;
	}

	for (int i = 0; i < N; i++)
		std::cout << arr[i] << " ";
		
	setConsoleColor(7); std::cout << std::endl;

	for (int i = 0; i < N; i++)
		for (int j = 0; j < 10; j++) delete_elem(&sup_arr[i]);
	delete[] arr; 
	delete[] sup_arr;

	Specif* result{ new Specif };
	if (!result) {
		std::cerr << "Ошибка выделения памяти!" << std::endl;
		exit(1);
	}
	result->compare = compare;
	result->swap = swap;
	result->time = (int)std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	return result;
}

Specif* by_class_method(int* orig_arr, int N) {
	int* arr{ new int[N] };
	if (!arr) {
		std::cerr << "Ошибка выделения памяти!" << std::endl;
		exit(1);
	}
	std::copy(orig_arr, orig_arr + N, arr);

	Connect* sup_arr{ new Connect[10] };
	if (!sup_arr) {
		std::cerr << "Ошибка выделения памяти!" << std::endl;
		exit(1);
	}
	for (int i = 0; i < 10; i++) 
		for (int j = 0; j < N / 2; j++) create_elem(&sup_arr[i]);

	setConsoleColor(2); std::cout << std::endl;
	std::cout << "Было: " << std::endl;
	for (int i = 0; i < N; i++)
		std::cout << orig_arr[i] << " ";
	std::cout << std::endl;
	std::cout << "Стало: " << std::endl;

	int compare{ 0 }, swap{ 0 }, assign{ 0 };
	int k{ 1 };
	while (N / (int)(pow(10, k)) != 1) k++;

	/*int** sup_arr{ new int* [k + 1] };
	if (!sup_arr) {
		std::cerr << "Ошибка выделения памяти!" << std::endl;
		exit(1);
	}
	for (int i = 0; i < k + 1; i++) {
		sup_arr[i] = new int[N];
		if (!sup_arr[i]) {
			std::cerr << "Ошибка выделения памяти!" << std::endl;
			exit(1);
		}
	}
	std::copy(orig_arr, orig_arr + N, sup_arr[0]);*/

	type_time start, end;
	start = std::chrono::high_resolution_clock::now();

	for (int i = 1; i <= k; i++) {
		/*int count[10]{ 0 }, indexes[10]{ 0 };

		for (int j = 0; j < N; j++)
			count[(sup_arr[i - 1][j] % (int)pow(10, i)) / (int)pow(10, i - 1)]++;
		for (int j = 1; j < 10; j++)
			indexes[j] = indexes[j - 1] + count[j - 1];

		for (int j = 0; j < N; j++) {
			swap++;
			int ind{ (sup_arr[i - 1][j] % (int)pow(10, i)) / (int)pow(10, i - 1) };
			sup_arr[i][indexes[ind]] = sup_arr[i - 1][j];
			indexes[ind]++;
		}*/
		
		for (int j = 0; j < N; j++) {
			assign++;
			add_elem_in_existing_list(&sup_arr[(arr[j] % (int)pow(10, i)) / (int)pow(10, i - 1)], arr[j]);
		}
		int current_index{ 0 };
		list *temp;

		int j{ 0 };
		while (j < N) {
			if (sup_arr[current_index].last != nullptr) {
				temp = sup_arr[current_index].first;
				assign++;
				while (temp != sup_arr[current_index].last->next) {
					
					arr[j] = temp->data;
					assign++;
					temp = temp->next;
					assign++; j++;
				}
				sup_arr[current_index].last = nullptr;
			}
			current_index++;
		}
	}

	end = std::chrono::high_resolution_clock::now();
	swap += assign / 3;
	for (int i = 0; i < N; i++)
		std::cout << arr[i] << " ";
	/*for (int i = 0; i < N; i++)
		std::cout << sup_arr[k][i] << " ";*/
	setConsoleColor(7); std::cout << std::endl;

	for (int i = 0; i < 10; i++)
		for (int j = 0; j < N / 2; j++) delete_elem(&sup_arr[i]);

	delete[] arr;
	/*for (int i = 0; i < k + 1; i++)
		delete[] sup_arr[i];*/
	delete[] sup_arr;

	Specif* result{ new Specif };
	if (!result) {
		std::cerr << "Ошибка выделения памяти!" << std::endl;
		exit(1);
	}
	result->compare = compare;
	result->swap = swap;
	result->time = (int)std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	return result;
}

void filling_array_non_repeating(int* arr, int N) {
	int temp, rand_index;
	setConsoleColor(2); std::cout << std::endl;
	std::cout << "Массив из " << N << " элементов: " << std::endl;
	for (int i = 0; i < N; i++) 
		arr[i] = i;
	for (int i = 0; i < N; i++) {
		rand_index = std::rand() % N;
		temp = arr[i];
		arr[i] = arr[rand_index];
		arr[rand_index] = temp;
	}
	for (int i = 0; i < N; i++)
		std::cout << arr[i] << " ";
	setConsoleColor(7); std::cout << std::endl << std::endl;
}

void filling_array(int* arr, int N) {
	setConsoleColor(2); std::cout << std::endl;
	std::cout << "Массив из " << N << " элементов: " << std::endl;
	for (int i = 0; i < N; i++) {
		arr[i] = std::rand() % N;
		std::cout << arr[i] << " ";
	}
	setConsoleColor(7); std::cout << std::endl << std::endl;
}

int main()
{
	setlocale(LC_ALL, "ru");
	srand(std::time(0));

	int* arr10{ new int[10] }, * arr100{ new int[100] }, * arr1k{ new int[1000] }, * arr10k{ new int[10000] };
	if (!arr10 || !arr100 || !arr1k || !arr10k) {
		std::cerr << "Ошибка выделения памяти!" << std::endl;
		exit(1);
	}

	int ans;
	while (true) {
		std::cout << "Элементы не должны повторяться?\n1.Да\n2.Нет\nОжидание ответа: ";
		std::cin >> ans;
		checking_input(ans, 1, 2);
		if (ans != NON) break;
	}

	if (ans == 1) {
		filling_array_non_repeating(arr10, 10);
		filling_array_non_repeating(arr100, 100);
		filling_array_non_repeating(arr1k, 1000);
		filling_array_non_repeating(arr10k, 10000);
	}
	else {
		filling_array(arr10, 10);
		filling_array(arr100, 100);
		filling_array(arr1k, 1000);
		filling_array(arr10k, 10000);
	}

	hist* history{ new hist };
	if (!history) {
		std::cerr << "Ошибка выделения памяти!" << std::endl;
		exit(1);
	}
	history->next = nullptr;

	while (true) {
		int ans;
		std::cout << "Выберите метод сортировки:\n1.Метод обмена\n2.Метод вставок\n3.Метод выбора\n4.Метод Шелла\n\
5.Быстрый метод\n6.Пирамидальный метод\n7.Простейший карманный метод (с доп.массивом)\n8.Простейший карманный метод (без доп.массива)\n\
9.Обобщённый карманный метод\n10.Поразрядный метод\nОжидание ответа: ";
		std::cin >> ans;
		checking_input(ans, 1, 10);
		if (ans == NON) continue;

		Specif* ret_10{ nullptr }, * ret_100{ nullptr }, * ret_1000{ nullptr }, * ret_10000{ nullptr };
		int num_steps;
		switch (ans) {
		case EXCHANGE:
			ret_10 = exchange_method(arr10, 10);
			ret_100 = exchange_method(arr100, 100);
			ret_1000 = exchange_method(arr1k, 1000);
			ret_10000 = exchange_method(arr10k, 10000);
			break;
		case INSERT:
			ret_10 = insert_method(arr10, 10);
			ret_100 = insert_method(arr100, 100);
			ret_1000 = insert_method(arr1k, 1000);
			ret_10000 = insert_method(arr10k, 10000);
			break;
		case SELECTION:
			ret_10 = selection_method(arr10, 10);
			ret_100 = selection_method(arr100, 100);
			ret_1000 = selection_method(arr1k, 1000);
			ret_10000 = selection_method(arr10k, 10000);
			break;
		case SHELL:
			std::cout << "Введите количество шагов для 10 элементов (от 1 до 3): ";
			std::cin >> num_steps;
			checking_input(num_steps, 1, 3);
			if (num_steps == NON) break;
			ret_10 = shell_method(arr10, 10, num_steps);

			std::cout << "Введите количество шагов для 100 элементов (от 1 до 6): ";
			std::cin >> num_steps;
			checking_input(num_steps, 1, 6);
			if (num_steps == NON) break;
			ret_100 = shell_method(arr100, 100, num_steps);

			std::cout << "Введите количество шагов для 1тыс. элементов (от 1 до 9): ";
			std::cin >> num_steps;
			checking_input(num_steps, 1, 9);
			if (num_steps == NON) break;
			ret_1000 = shell_method(arr1k, 1000, num_steps);

			std::cout << "Введите количество шагов для 10тыс. элементов (от 1 до 13): ";
			std::cin >> num_steps;
			checking_input(num_steps, 1, 2147483647);
			if (num_steps == NON) break;
			ret_10000 = shell_method(arr10k, 10000, num_steps);
			break;
		case FAST:
			ret_10 = wrapper_fast_method(arr10, 10);
			ret_100 = wrapper_fast_method(arr100, 100);
			ret_1000 = wrapper_fast_method(arr1k, 1000);
			ret_10000 = wrapper_fast_method(arr10k, 10000);
			break;
		case PYRAMIDAL:
			ret_10 = pyramidal_method(arr10, 10);
			ret_100 = pyramidal_method(arr100, 100);
			ret_1000 = pyramidal_method(arr1k, 1000);
			ret_10000 = pyramidal_method(arr10k, 10000);
			break;
		case SIMPLEST_POCKET_ARR:
			ret_10 = simplest_pocket_method_arr(arr10, 10);
			ret_100 = simplest_pocket_method_arr(arr100, 100);
			ret_1000 = simplest_pocket_method_arr(arr1k, 1000);
			ret_10000 = simplest_pocket_method_arr(arr10k, 10000);
			break;
		case SIMPLEST_POCKET_NOT_ARR:
			ret_10 = simplest_pocket_method_not_arr(arr10, 10);
			ret_100 = simplest_pocket_method_not_arr(arr100, 100);
			ret_1000 = simplest_pocket_method_not_arr(arr1k, 1000);
			ret_10000 = simplest_pocket_method_not_arr(arr10k, 10000);
			break;
		case GENERALIZED_POCKET:
			ret_10 = generalized_pocket_method(arr10, 10);
			ret_100 = generalized_pocket_method(arr100, 100);
			ret_1000 = generalized_pocket_method(arr1k, 1000);
			ret_10000 = generalized_pocket_method(arr10k, 10000);
			break;
		case BY_CLASS:
			ret_10 = by_class_method(arr10, 10);
			ret_100 = by_class_method(arr100, 100);
			ret_1000 = by_class_method(arr1k, 1000);
			ret_10000 = by_class_method(arr10k, 10000);
			break;
		}

		if (history->next != nullptr) {
			std::cout << "\nПараметры предыдущих выполнений.\n";
			output_hist(history);
		}
		add_elem(history, ret_10, ret_100, ret_1000, ret_10000, (ans == 1 ? "\nМетод обмена" : (ans == 2 ? "\nМетод вставок" : (ans == 3 ? "\nМетод выбора" : \
			(ans == 4 ? "\nМетод Шелла" : (ans == 5 ? "\nБыстрый метод" : (ans == 6 ? "\nПирамидальный метод" : \
				(ans == 7 ? "\nПростейший карманный метод (с доп.массивом)" : \
					(ans == 8 ? "\nПростейший карманный метод (без доп.массива)" : \
						(ans == 9 ? "\nОбобщённый карманный метод" : "\nПоразрядный метод"))))))))));

		std::cout << "\nПараметры текущего выполнения." << \
			(ans == 1 ? "\nМетод обмена" : (ans == 2 ? "\nМетод вставок" : (ans == 3 ? "\nМетод выбора" : (ans == 4 ? "\nМетод Шелла" : (ans == 5 ? "\nБыстрый метод" : (ans == 6 ? "\nПирамидальный метод" : \
				(ans == 7 ? "\nПростейший карманный метод (с доп.массивом)" : (ans == 8 ? "\nПростейший карманный метод (без доп.массива)" : \
					(ans == 9 ? "\nОбобщённый карманный метод" : "\nПоразрядный метод"))))))))) << \
			"\n10 элементов:" << \
			"\n\tКол-во сравнений = " << ret_10->compare << \
			"\n\tКол-во перестановок = " << ret_10->swap << \
			"\n\tВремя выполнения = " << ret_10->time << \
			"\n100 элементво: " << \
			"\n\tКол-во сравнений = " << ret_100->compare << \
			"\n\tКол-во перестановок = " << ret_100->swap << \
			"\n\tВремя выполнения = " << ret_100->time << \
			"\n1тыс. элементов: " << \
			"\n\tКол-во сравнений = " << ret_1000->compare << \
			"\n\tКол-во перестановок = " << ret_1000->swap << \
			"\n\tВремя выполнения = " << ret_1000->time << \
			"\n10тыс. элементов: " << \
			"\n\tКол-во сравнений = " << ret_10000->compare << \
			"\n\tКол-во перестановок = " << ret_10000->swap << \
			"\n\tВремя выполнения = " << ret_10000->time << std::endl << std::endl;
	}
}
