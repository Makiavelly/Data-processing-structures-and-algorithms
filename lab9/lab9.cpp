#include <iostream>
#include <windows.h>
#undef max
#define NON -1

struct tree {
	tree *left, *right;
	int data;
};

int num_vert;
tree* root;

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

void forward_view(tree* locale_root, int depth) {
	if (locale_root != nullptr) {
		for (int i = 0; i < depth; i++) std::cout << "     ";
		std::cout << locale_root->data << std::endl;
		forward_view(locale_root->left, depth + 1);
		forward_view(locale_root->right, depth + 1);
	}
}

void symmetric_view(tree* locale_root, int depth) {
	if (locale_root != nullptr) {
		symmetric_view(locale_root->left, depth + 1);
		for (int i = 0; i < depth; i++) std::cout << "     ";
		std::cout << locale_root->data << std::endl;
		symmetric_view(locale_root->right, depth + 1);
	}
}

void back_symmetric_view(tree* locale_root, int depth) {
	if (locale_root != nullptr) {
		back_symmetric_view(locale_root->right, depth + 1);
		for (int i = 0; i < depth; i++) std::cout << "     ";
		std::cout << locale_root->data << std::endl;
		back_symmetric_view(locale_root->left, depth + 1);
	}
}

tree* init(int num_vert) {
	if (num_vert == 0) return nullptr;

	int num_left{ num_vert / 2 };
	int num_right{ num_vert - num_left - 1 };

	tree* temp = (tree*)malloc(sizeof(tree));
	temp->data = std::rand() % 100;
	temp->left = init(num_left);
	temp->right = init(num_right);
	return temp;
}

int main()
{
	setlocale(LC_ALL, "ru");
	srand(std::time(0));

	while (true) {
		std::cout << "Введите количество вершин ИСД: " << std::endl;
		std::cin >> num_vert;
		checking_input(num_vert, 1, 2147483647);
		if (num_vert != NON) break;
	}
	root = init(num_vert);

	std::cout << std::endl;
	setConsoleColor(2);
	std::cout << "Прямой обход." << std::endl;
	forward_view(root, 0);
	std::cout << std::endl;

	std::cout << std::endl;
	setConsoleColor(12);
	std::cout << "Симметричный обход." << std::endl;
	symmetric_view(root, 0);
	std::cout << std::endl;

	std::cout << std::endl;
	setConsoleColor(6);
	std::cout << "Обратно-симметричный обход." << std::endl;
	back_symmetric_view(root, 0);
	std::cout << std::endl;
	setConsoleColor(7);
}