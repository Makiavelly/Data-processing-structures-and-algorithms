#include <iostream>
#include <windows.h>
#undef max
#include <chrono>
#include <string>

typedef std::chrono::time_point<std::chrono::high_resolution_clock> type_time;

enum ACNTION_MENU {
	NON = -100001, EXCHANGE = 1, INSERT = 2, SELECTION = 3
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

	int compare{ 0 }, swap, assign{ 0 };
	type_time start, end;
	start = std::chrono::high_resolution_clock::now();

	for (int i = 1; i < N; i++) {
		int temp = arr[i];
		assign++;
		int j = i - 1;
		for (; j > -1; j--) {
			compare++;
			if (arr[j] > temp) {
				arr[j + 1] = arr[j];
				assign++;
			}
			else break;
		}
		arr[j + 1] = temp;
		assign++;
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

	int *arr10{ new int[10] }, *arr100{ new int[100] }, *arr1k{ new int[1000] }, *arr10k{ new int[10000] };
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
		std::cout << "Выберите метод сортировки:\n1.Метод обмена\n2.Метод вставок\n3.Метод выбора\nОжидание ответа: ";
		std::cin >> ans;
		checking_input(ans, 1, 3);

		Specif *ret_10{nullptr}, * ret_100{ nullptr }, * ret_1000{ nullptr }, * ret_10000{ nullptr };
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
		}

		if (history->next != nullptr) {
			std::cout << "\nПараметры предыдущих выполнений.\n";
			output_hist(history);
		}
		add_elem(history, ret_10, ret_100, ret_1000, ret_10000, (ans == 1 ? "\nМетод обмена" : (ans == 2 ? "\nМетод вставок" : "\nМетод выбора")));
		
		std::cout << "\nПараметры текущего выполнения."<< \
			(ans == 1 ? "\nМетод обмена" : (ans == 2 ? "\nМетод вставок" : "\nМетод выбора")) << \
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
