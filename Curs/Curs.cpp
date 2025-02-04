#include "gauss_jordan.h"
#include <iostream>
#include <iomanip>

int equations;
int variables;

enum minmax {
	min = 1,
	max = 2
};

//выразить базисные переменные функции через свободные члены
void check_the_base_variable(double** matrix, double* z, bool* basis) {
	
	double* temp;

	for (int i = 0; i < variables; i++) {
		if (basis[i] && z[i] != 0) {

			temp = new double[variables];

			for (int j = 0; j < variables; j++) {
				if (i == j) 
					temp[j] = 0;
				else 
					temp[j] = 0 - matrix[i][j];
			}

			for (int j = 0; j < variables; j++) {
				if (j == i) continue;

				z[j] += temp[j] * z[i];
			}
			z[i] = 0;

			delete_array(temp);
		}
	}	
}
//вывод симплексной таблицы
void print_simplex_table(double** simplex_table, bool* basis) {

	std::cout << std::endl << "БП |" << std::setw(10) << "1" << std::setw(10) << "x1" << std::setw(10) << "x2" << std::setw(10) <<
		"x3" << std::setw(10) << "x4" << std::setw(10) << "x5" << std::endl;
	for (int i = 0; i < 65; i++) std::cout << "-";
	std::cout << std::endl;
	int basis_count = 0;

	for (int i = 0; i < equations + 1; i++) {

		if (i != equations) {
			for (int k = basis_count; k < variables; k++) {
				if (basis[k]) {
					std::cout << "x" << basis_count + 1 << " |";
					basis_count++;
					break;
				}
				basis_count++;
			}
		}
		else std::cout << "Z  |";

		for (int j = 0; j < variables + 1; j++) {
			std::cout << std::setw(10) << simplex_table[i][j];
		}
		std::cout << std::endl;
	}
}

bool check_simple_table(double** simplex_table, minmax minmax) {
	if (minmax == min) {
		for (int i = equations, j = 1; j < variables; j++) {
			if (simplex_table[i][j] > 0) return false;
		}
		return true;
	}
	else {
		for (int i = equations, j = 1; j < variables; j++) {
			if (simplex_table[i][j] < 0) return false;
		}
		return true;
	}
}

void calculation_of_resolving_element(double** simplex_table, int &x, int &y, minmax minmax) {


}

void simplex_method(double** matrix, double* z, bool* basis, minmax minmax) {

	//выделение памяти под симплексную таблицу
	double** simplex_table = new double*[equations + 1];
	for (int i = 0; i < equations + 1; i++) {
		simplex_table[i] = new double[variables + 1];
	}

	//первичное заполнение таблицы
	for (int i = 0, j = 0; i < equations; i++) {
		simplex_table[i][j] = matrix[i][variables];
	}
	for (int i = 0; i < equations; i++) {
		for (int j = 1; j < variables + 1; j++) {
			simplex_table[i][j] = matrix[i][j - 1];
		}
	}
	simplex_table[equations][0] = 0;
	for (int i = equations, j = 1; j < variables + 1; j++) {
		simplex_table[i][j] = z[j - 1];
	}
	
	//вывод симплексной таблицы
	print_simplex_table(simplex_table, basis);

	if (!check_simple_table(simplex_table, minmax)) {

		std::cout << "Решение не оптимально. Расчет новой таблицы" << std::endl;

		//строка разрешающего элемента
		int x;
		//столбец разрешающего элемента
		int y;

		calculation_of_resolving_element(simplex_table, x, y, minmax);



	}


	else {

	}

	delete_matrix(simplex_table, equations + 1);
}

int main()
{
	setlocale(LC_ALL, "Rus");
	char c;

	double** matrix;
	double* z;
	bool* basis;

	while (true) {
		std::cout << "1. Вариант из задания" << std::endl;
		std::cout << "2. Ввести пример вручную" << std::endl;
		std::cout << "3. Выход" << std::endl;

		std::cin >> c;

		switch (c) {
			case('1'):

				equations = 3;
				variables = 5;

				matrix = new double* [equations];

				z = new double[variables] { 7, 1, 0, 0, 0 };
				basis = new bool[variables] { true, true, true, false, false };

				matrix[0] = new double[variables + 1] { 4, 1, -1, 0, 0, 11 };
				matrix[1] = new double[variables + 1] { 2, 3, 0, -1, 0, 13 };
				matrix[2] = new double[variables + 1] { 1, 7, 0, 0, -1, 12 };

				//нахождение опорного решения методом Жордана - Гаусса
				std::cout << "Опорное решение, найденное методом Жордана - Гаусса:" << std::endl;
				gauss_jordan_elementation(matrix, variables, equations);
				print_matrix(matrix, variables, equations);

				//выразить базисные переменные функции через свободные члены
				check_the_base_variable(matrix, z, basis);

				//решение симплекс методом
				simplex_method(matrix, z, basis, min);

				delete_matrix(matrix, equations);
				delete_array(z);
				delete_array(basis);
				break;
			case('2'):
				break;
			case('3'):
				return 0;
			default:
				break;
		}		
	}
}

