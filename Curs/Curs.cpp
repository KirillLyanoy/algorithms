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

	std::cout << std::endl << std::endl << "БП |" << std::setw(10) << "1" << std::setw(10) << "x1" << std::setw(10) << "x2" << std::setw(10) <<
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

//проверка на оптимальность решения
bool check_simple_table(double** simplex_table, minmax minmax) {
	if (minmax == min) {
		for (int i = equations, j = 1; j < variables + 1; j++) {
			if (simplex_table[i][j] > 0) return false;
		}
		return true;
	}
	else {
		for (int i = equations, j = 1; j < variables + 1; j++) {
			if (simplex_table[i][j] < 0) return false;
		}
		return true;
	}
}

//определение разрешающего элемента
void calculation_of_resolving_element(double** simplex_table, int &resolving_x, int &resolving_y, minmax minmax) {
	
	int CO_size = 0;
	double* CO = new double[CO_size];
	//индексный массив СО
	int* CO_index_arr = new int[CO_size];

	if (minmax == min) 
		resolving_y = index_of_max_element(simplex_table[equations], variables + 1, 1);
	else 
		resolving_y = index_of_min_element(simplex_table[equations], variables + 1, 1);


	for (int i = 0; i < equations; i++) {
		if (simplex_table[i][resolving_y] > 0) {

			add_element_to_array(CO, CO_size);
			add_element_to_array(CO_index_arr, CO_size);

			CO[CO_size] = simplex_table[i][0] / simplex_table[i][resolving_y];
			CO_index_arr[CO_size] = i;

			CO_size++;
		}
	}		

	if (minmax == min)
		resolving_x = CO_index_arr[index_of_min_element(CO, CO_size, 0)];
	else 
		resolving_x = CO_index_arr[index_of_min_element(CO, CO_size, 0)];

	delete[] CO;
	delete[] CO_index_arr;
}

//перерасчет симплексной таблицы
void simplex_table_rebuilding(double**& simplex_table, int resolving_x, int resolving_y, bool* basis) {

	//временная новая симплексная таблица
	double** temp_table = new double* [variables + 1];
	for (int i = 0; i < equations + 1; i++)
		temp_table[i] = new double[variables + 1];


	//int old_basis_index;
	//int* basis_index = new int[equations];
	//for (int i = 0, k = 0; i < variables; i++) {
	//	if (basis[i]) {
	//		basis_index[k] = i;
	//		k++;
	//	}
	//}
	//for (int i = 0; i < variables; i++) {
	//	if (basis_index[i] == resolving_x) {
	//		old_basis_index = basis_index[i];
	//		basis[basis_index[i]] = false;
	//	}
	//}
	//basis[resolving_y - 1] = true;

	//delete[] basis_index;

	int old_basis_index = 0;
	int basis_count = 0;

	//замена базисной переменной
	for (int i = 0; i < variables; i++) {
		if (basis[i]) {
			if (basis_count == resolving_x)
				break;
			else basis_count++;
		}
		old_basis_index++;
	}

	basis[old_basis_index] = false;
	basis[resolving_y - 1] = true;

	//Элементы разрешающей строки делим на разрешающий элемент
	for (int i = resolving_x, j = 0; j < variables + 1; j++) {
		temp_table[i][j] = simplex_table[i][j] / simplex_table[resolving_x][resolving_y];
	}

	//Остальные элементы разрешающего столбца заполняем нулями
	for (int i = 0; i < equations + 1; i++) {
		if (i != resolving_x) temp_table[i][resolving_y] = 0;
	}

	//Столбцы, соответствующие остальным базисным переменным не изменяются
	for (int i = 0; i < equations + 1; i++) {
		for (int j = 1; j < variables + 1; j++) {
			if (basis[j - 1] && j != resolving_y)
				temp_table[i][j] = simplex_table[i][j];
		}
	}

	//В прежднем базисном столбце меняем элементы на элементы нового базисного столбца, деленного на разрешающий элемент с противоположным знаком
	for (int i = 0; i < equations + 1; i++) {
		if (i != resolving_x) {
			temp_table[i][old_basis_index + 1] = 0 - (simplex_table[i][resolving_y] / simplex_table[resolving_x][resolving_y]);
		}		
	}

	//Остальные элементы пересчитываем по правилу прямоугольников
	for (int i = 0; i < equations + 1; i++) {
		for (int j = 0; j < variables + 1; j++) {
			if (i != resolving_x && j != resolving_y && j != old_basis_index + 1) {			
				if (j > 0 && basis[j - 1]) continue;
				temp_table[i][j] = simplex_table[i][j] - (simplex_table[i][resolving_y] * simplex_table[resolving_x][j] / simplex_table[resolving_x][resolving_y]);			
			}

		}
	}

	delete_array(simplex_table, equations + 1);
	simplex_table = temp_table;
}

//симплекс метод
void simplex_method(double** matrix, double* z, bool* basis, minmax minmax) {

	int iteration = 0;
	bool solution_is_optimal = false;

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
		simplex_table[i][j] = 0 - z[j - 1];
	}
	
	while (!solution_is_optimal && iteration < 100) {

		iteration++;

		//вывод симплексной таблицы
		print_simplex_table(simplex_table, basis);

		solution_is_optimal = check_simple_table(simplex_table, minmax);

		if (solution_is_optimal) {
			std::cout << std::endl << "Решение оптмально" << std::endl;
			break;
		}		
		else {
			std::cout << std::endl << "Решение не оптимально. Расчет новой таблицы" << std::endl;

			//строка разрешающего элемента
			int resolving_x;
			//столбец разрешающего элемента
			int resolving_y;

			//определение разрешающего элемента
			calculation_of_resolving_element(simplex_table, resolving_x, resolving_y, minmax);

			simplex_table_rebuilding(simplex_table, resolving_x, resolving_y, basis);
		}
	}

	//вывод результата
	if (!solution_is_optimal) 
		std::cout << "Решений нет" << std::endl;
	else {

		for (int j = 1; j < variables + 1; j++) {
			if (basis[j - 1]) {
				for (int k = 0; k < equations; k++) {
					if (simplex_table[k][j] == 1)
						z[j - 1] = simplex_table[k][0];
				}
			}
			else z[j - 1] = 0;
		}
		
		std::cout << "(X" << iteration << ") = ( ";
		for (int i = 0; i < variables; i++) {
			std::cout << z[i] << "; ";
		}
		std::cout << ")" << std::endl;
	}


	delete_array(simplex_table, equations + 1);
}

//вывод результирующей функции с ответом
void result_equation(double* z, double* F) {
	double answer = 0;

	std::cout << "F(x) = ";
	for (int i = 0; i < variables; i++) {
		std::cout << z[i] << " * " << F[i];
		if (i + 1 < variables) std::cout << " + ";
		else std::cout << " = ";
	}

	for (int i = 0; i < variables; i++) {
		answer += z[i] * F[i];
	}

	std::cout << answer << std::endl << std::endl;
}

void define_basic_variables(double** matrix, bool* basis) {

	int basis_row_with_one = 0;

	for (int j = 0; j < variables; j++) {
		
		if (col_basis_check(matrix, equations, j, basis_row_with_one)) {
			basis[j] = true;
			basis_row_with_one++;
		}
		else
			basis[j] = false;
	}
}

int main()
{
	setlocale(LC_ALL, "Rus");
	char c;

	double** matrix;
	double* z;
	double* F;
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

				F = new double[variables] { 7, 1, 0, 0, 0 };
				z = new double[variables];
				copy_array(F, z, variables);

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

				result_equation(z, F);

				delete_array(matrix, equations);
				delete_array(z);
				delete_array(F);
				delete_array(basis);
				break;
			case('2'):

				while (true) {
					std::cout << "Введите количество уравнений: ";
					std::cin >> equations;

					if (equations < 2 || equations > 100) std::cout << std::endl << "Неправильное количество уравнений. Повторите ввод: ";
					else break;
				}

				while (true) {
					std::cout << "Введите количество переменных: ";
					std::cin >> variables;

					if (variables < 2 || variables > 100) std::cout << std::endl << "Неправильное количество переменных. Повторите ввод: ";
					else break;
				}

				matrix = new double* [equations];
				for (int i = 0; i < equations; i++) {
					matrix[i] = new double[variables + 1];
				}


				fill_array(matrix, variables, equations);

				std::cout << std::endl << "Ввод ффункции." << std::endl;
				
				F = new double[variables];

				for (int i = 0; i < variables; i++) {

					std::cout << "Коэффициент при х" << i + 1 << " : ";
					std::cin >> F[i];
				}

				minmax minmax;

				std::cout << "Найти минимум или максимум функции?" << std::endl << "1. Минимум" << std::endl << "2. Максимум" << std::endl;


				std::cin >> c;
				
				switch (c) {
					case ('1'):
						minmax = min;
						break;
					case ('2'):
						minmax = max;
						break;
					default: 
						minmax = min;
						break;
				}					
				

				basis = new bool[variables];

				z = new double[variables];
				copy_array(F, z, variables);					

				check_negative_coefficients(matrix, variables, equations);

				if (!row_echelon_form_check(matrix, variables, equations)) {
					std::cout << "Опорное решение, найденное методом Жордана - Гаусса:" << std::endl;
					gauss_jordan_elementation(matrix, variables, equations);
					print_matrix(matrix, variables, equations);					
				}

				

				define_basic_variables(matrix, basis);

				//выразить базисные переменные функции через свободные члены
				check_the_base_variable(matrix, z, basis);

				//решение симплекс методом
				simplex_method(matrix, z, basis, minmax);

				result_equation(z, F);

				delete_array(matrix, equations);
				delete_array(z);
				delete_array(F);
				delete_array(basis);
				break;

				break;
			case('3'):
				return 0;
			default:
				break;
		}		
	}
}

