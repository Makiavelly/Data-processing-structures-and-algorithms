#include <iostream>
#include <windows.h>
#include <time.h>
#undef max


enum PROC_USER_REACT {
	NOTHING, CONTINUE, EXIT
};

enum ACTION_MAIN_MENU {
	NON = 0, ADD_CELL = 1, DELETE_CELL = 2, CURRENT_STATUS = 3
};

HHOOK hHook;
bool esc_was_pressed{ false };

struct mem_cell;

struct queue {
	mem_cell *head, *tail;
};

struct mem_cell {
	char data;
	mem_cell* ptr;
};

void setConsoleColor(int color) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

bool is_empty(queue* q) {
	if (q->head == nullptr) {
		setConsoleColor(4);
		std::cout << "\nОчередь пуста\n" << std::endl;
		setConsoleColor(7);
		return true;
	}
	else return false;
}

void add_cell(queue* q, char data) {
	if(q->head == nullptr){
		mem_cell* temp{ (mem_cell*)malloc(sizeof(mem_cell)) };
		if (temp == nullptr) {
			std::cerr << "Ошибка: Не удалось выделить память." << std::endl;
			exit(1);
		}
		temp->ptr = nullptr;
		temp->data = data;
		q->head = temp;
		q->tail = temp;
	}
	else {
		mem_cell* temp{ (mem_cell*)malloc(sizeof(mem_cell)) };
		if (temp == nullptr) {
			std::cerr << "Ошибка: Не удалось выделить память." << std::endl;
			exit(1);
		}
		temp->ptr = nullptr;
		temp->data = data;
		q->tail->ptr = temp;
		q->tail = temp;
	}
}

void delete_cell(queue* q) {
	mem_cell* temp{ q->head };
	if (q->head == q->tail) { q->head = nullptr; q->tail = nullptr; }
	else q->head = q->head->ptr;
	free(temp);
}

void current_status(queue* q) {
	std::cout << std::endl;
	setConsoleColor(2);
	mem_cell* temp{ q->head };
	while (temp != nullptr) {
		std::cout << temp->data << " ";
		temp = temp->ptr;
	}
	setConsoleColor(7);
	std::cout << std::endl << std::endl;
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode >= 0) {
		KBDLLHOOKSTRUCT* pKeyBoard = (KBDLLHOOKSTRUCT*)lParam;
		if (wParam == WM_KEYDOWN) {
			if (pKeyBoard->vkCode == VK_ESCAPE) esc_was_pressed = true;
			UnhookWindowsHookEx(hHook);
			PostQuitMessage(0);
		}
	}
	return CallNextHookEx(hHook, nCode, wParam, lParam);
}

void main_menu(queue** q) {
	int ans, count;

	while (true) {
		ans = (std::rand() % 100 + 1) % 2 == 0 ? ADD_CELL : DELETE_CELL;

		switch (ans) {
		case ADD_CELL:
			count = std::rand() % 3 + 1;
			//count = std::rand() % 4 + 1;
			for (int i = 0; i < count; i++) {
				add_cell(*q, std::rand() % 26 + 65);
			}
			setConsoleColor(2);
			std::cout << "\nДобавлено " << count << " элем." << std::endl;
			setConsoleColor(7);
			break;
		case DELETE_CELL:
			//count = std::rand() % 3 + 1;
			count = std::rand() % 4 + 1;
			int i;
			for (i = 0; i < count; i++) {
				if (!((*q)->head == nullptr)) {
					delete_cell(*q);
				}
				else break;
			}
			if (i == count) {
				setConsoleColor(2);
				std::cout << "\nУдалено " << count << " элем." << std::endl;
				setConsoleColor(7);
			}
			else {
				setConsoleColor(2);
				std::cout << "\nУдалено " << i << " элем." << std::endl;
				setConsoleColor(7);
			}
			break;
		}
		if (!is_empty(*q)) current_status(*q);

		std::cout << "Для выхода нажмите ESC. Для продолжения нажмите любую другую клавишу... " << std::endl;
		DWORD threadId = GetCurrentThreadId();
		hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, GetModuleHandle(nullptr), 0);

		if (hHook == nullptr) {
			std::cerr << "Не удалось установить хук. Ошибка: " << GetLastError() << std::endl;
			exit(1);
		}

		MSG msg;
		while (GetMessage(&msg, nullptr, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (esc_was_pressed) break;
	}
}

void init_queue(queue** q) {
	queue* temp_q = (queue*)malloc(sizeof(queue));
	if (temp_q == nullptr) {
		std::cerr << "Ошибка: Не удалось выделить память." << std::endl;
		exit(1);
	}
	(*q) = temp_q;
	(*q)->head = nullptr;
	(*q)->tail = nullptr;
}

void init_random() {
	std::srand(std::time(0));
	std::cout << "Сид установлен\n" << std::endl;
}

int main()
{
	setlocale(LC_ALL, "ru");

	queue* q{ nullptr };

	init_queue(&q);
	init_random();
	main_menu(&q);
}
