#include <iostream>
#include <windows.h>
#undef max
#include <chrono>
#include <string>

typedef std::chrono::time_point<std::chrono::high_resolution_clock> type_time;

enum ACNTION_MENU {
	NON = -100001, EXCHANGE = 1, INSERT = 2, SELECTION = 3, SHELL = 4, FAST = 5, PYRAMIDAL = 6
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

	int compare{ 0 }, swap, assign{ 0 }, step, temp;
	type_time start, end;
	start = std::chrono::high_resolution_clock::now();

	for (int m = num_steps - 1; m > -1; m--) {
		step = arr_steps[m];
		for (int i = step; i < N; i++) {
			temp = arr[i];
			assign++;
			int j = i - step;
			for (; j > -1; j -= step) {
				compare++;
				if (arr[j] > temp) {
					arr[j + step] = arr[j];
					assign++;
				}
				else break;
			}
			arr[j + step] = temp;
			assign++;
		}
	}

	end = std::chrono::high_resolution_clock::now();
	swap = assign / 3;
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

//Specif* analog_fast_method(int* orig_arr, int N) {
//	int* arr{ new int[N] };
//	if (!arr) {
//		std::cerr << "Ошибка выделения памяти!" << std::endl;
//		exit(1);
//	}
//	std::copy(orig_arr, orig_arr + N, arr);
//
//	setConsoleColor(2); std::cout << std::endl;
//	std::cout << "Было: " << std::endl;
//	for (int i = 0; i < N; i++)
//		std::cout << arr[i] << " ";
//	std::cout << std::endl;
//	std::cout << "Стало: " << std::endl;
//
//	int compare{ 0 }, swap{ 0 };
//	int left, right, sred, i, j, temp;
//	int fs_rand, sd_rand, fd_rand;
//	type_time start, end;
//	stack* sp = add_elem(nullptr, 0, N - 1);
//	start = std::chrono::high_resolution_clock::now();
//
//	while (sp != nullptr) {
//		left = sp->left;
//		right = sp->right;
//		sp = delete_elem(sp);
//
//		sred = arr[right];
//		j = left;
//
//		for (i = left; i < right; i++) {
//			compare++;
//			if (sred > arr[i]) {
//				swap++;
//				temp = arr[i];
//				arr[i] = arr[j];
//				arr[j] = temp;
//				j++;
//			}
//		}
//
//		swap++;
//		arr[right] = arr[j];
//		arr[j] = sred;
//
//		if (j > left + 1) sp = add_elem(sp, left, j - 1);
//		if (right > j + 1) sp = add_elem(sp, j + 1, right);
//	}
//
//	end = std::chrono::high_resolution_clock::now();
//	for (int i = 0; i < N; i++)
//		std::cout << arr[i] << " ";
//	setConsoleColor(7); std::cout << std::endl;
//	delete[] arr;
//
//	Specif* result{ new Specif };
//	if (!result) {
//		std::cerr << "Ошибка выделения памяти!" << std::endl;
//		exit(1);
//	}
//	result->compare = compare;
//	result->swap = swap;
//	result->time = (int)std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
//	return result;
//}

//Specif* fast_method(int* orig_arr, int N) {
//		int* arr{ new int[N] };
//		if (!arr) {
//			std::cerr << "Ошибка выделения памяти!" << std::endl;
//			exit(1);
//		}
//		std::copy(orig_arr, orig_arr + N, arr);
//
//		setConsoleColor(2); std::cout << std::endl;
//		std::cout << "Было: " << std::endl;
//		for (int i = 0; i < N; i++)
//			std::cout << arr[i] << " ";
//		std::cout << std::endl;
//		std::cout << "Стало: " << std::endl;
//
//		int compare{ 0 }, swap{ 0 };
//		int left, right, sred, i, j, temp;
//		type_time start, end;
//		stack* sp = add_elem(nullptr, 0, N - 1);
//		start = std::chrono::high_resolution_clock::now();
//
//		while (sp != nullptr) {
//			left = sp->left;
//			right = sp->right;
//			sp = delete_elem(sp);
//
//			i = left; j = right;
//			sred = arr[(left + right) / 2];
//			
//
//			do {
//				while (sred > arr[i]) {
//					compare++;
//					i++;
//				}
//				compare++;
//
//				while (arr[j] > sred) {
//					compare++;
//					j--;
//				}
//				compare++;
//
//				if (j >= i) {
//					swap++;
//					temp = arr[i];
//					arr[i] = arr[j];
//					arr[j] = temp;
//					i++; j--;
//				}
//			} while (j >= i);
//
//			if (right > i) sp = add_elem(sp, i, right);
//			if (j > left) sp = add_elem(sp, left, j);
//		}
//
//		end = std::chrono::high_resolution_clock::now();
//		for (int i = 0; i < N; i++)
//			std::cout << arr[i] << " ";
//		setConsoleColor(7); std::cout << std::endl;
//		delete[] arr;
//
//		Specif* result{ new Specif };
//		if (!result) {
//			std::cerr << "Ошибка выделения памяти!" << std::endl;
//			exit(1);
//		}
//		result->compare = compare;
//		result->swap = swap;
//		result->time = (int)std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
//		return result;
//}

void fast_method(int* arr, int left, int right, int& compare, int& swap) {
	int i{ left }, j{ right };
	int sred{arr[(left + right) / 2]}, temp;
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

void sito(int* arr, int left, int right, int& compare, int& assign) {
	int i{ left }, j{ 2 * left }, x{ arr[left - 1] };
	assign++;
	if (j < right && arr[j - 1 + 1] > arr[j - 1]) j++;
	compare++;
	while (j <= right && arr[j - 1] > x) {
		compare++;
		arr[i - 1] = arr[j - 1];
		assign++;
		i = j; 
		j = 2 * j;
		if (j < right && arr[j - 1 + 1] > arr[j - 1]) j++;
		compare++;
	}
	arr[i - 1] = x;
	assign++;
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
	int compare{ 0 }, swap{ 0 }, assign{ 0 };
	type_time start, end;
	start = std::chrono::high_resolution_clock::now();

	while (left > 1) {
		left--;
		sito(arr, left, right, compare, assign);
	}
	while (right > 1) {
		temp = arr[0];
		arr[0] = arr[right - 1];
		arr[right - 1] = temp;
		swap++;
		right--;
		sito(arr, left, right, compare, assign);
	}

	end = std::chrono::high_resolution_clock::now();
	swap += assign / 3;
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

void filling_array(int* arr, int N) {
	setConsoleColor(2); std::cout << std::endl;
	std::cout << "Массив из " << N << " элементов: " << std::endl;
	for (int i = 0; i < N; i++) {
		arr[i] = std::rand() % (2 * N + 1) - N;
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

	filling_array(arr10, 10);
	filling_array(arr100, 100);
	filling_array(arr1k, 1000);
	filling_array(arr10k, 10000);

	hist* history{ new hist };
	if (!history) {
		std::cerr << "Ошибка выделения памяти!" << std::endl;
		exit(1);
	}
	history->next = nullptr;

	while (true) {
		int ans;
		std::cout << "Выберите метод сортировки:\n1.Метод обмена\n2.Метод вставок\n3.Метод выбора\n4.Метод Шелла\n5.Быстрый метод\n6.Пирамидальный метод\nОжидание ответа: ";
		std::cin >> ans;
		checking_input(ans, 1, 6);
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
		}

		if (history->next != nullptr) {
			std::cout << "\nПараметры предыдущих выполнений.\n";
			output_hist(history);
		}
		add_elem(history, ret_10, ret_100, ret_1000, ret_10000, (ans == 1 ? "\nМетод обмена" : (ans == 2 ? "\nМетод вставок" : ( ans == 3 ? "\nМетод выбора" : \
			(ans == 4 ? "\nМетод Шелла" : (ans == 5 ? "\nБыстрый метод" : "\nПирамидальный метод"))))));

		std::cout << "\nПараметры текущего выполнения." << \
			(ans == 1 ? "\nМетод обмена" : (ans == 2 ? "\nМетод вставок" : (ans == 3 ? "\nМетод выбора" : (ans == 4 ? "\nМетод Шелла" : (ans == 5 ? "\nБыстрый метод" : "\nПирамидальный метод"))))) << \
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
