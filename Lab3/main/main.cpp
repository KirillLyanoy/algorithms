#include <iostream>
#include <fstream>

#define filename "data3_4.bin"
#define suppliers_count 3
#define consumers_count 4

bool degenerate_plan = false;

void read_from_file(int** tariff_matrix, int* suppliers, int* consumers, int number_of_suppliers, int number_of_consumers) {

	FILE* file;
	fopen_s(&file, filename, "rb");

	if (file == nullptr) {
		std::cerr << "Ошибка открытия файла." << std::endl;
		return;
	}

	size_t size;

	for (int i = 0; i < number_of_suppliers; i++) {
		size = fread(tariff_matrix[i], sizeof(int), number_of_consumers, file);
	}
	size = fread(suppliers, sizeof(int), number_of_suppliers, file);
	size = fread(consumers, sizeof(int), number_of_consumers, file);
	fclose(file);
}

void delete_array(int** array, int size) {
	for (int i = 0; i < size; i++)
		delete[] array[i];
	delete[] array;
}

int array_sum(int* array, int &size) {
	int sum = 0;
	for (int i = 0; i < size; i++) {
		sum += array[i];
	}
	return sum;
}

void delete_element_from_array(int* &array, int array_size, int index) {

	int* temporary_array = new int[array_size - 1];

	for (int i = 0, k = 0; i < array_size; i++) {
		if (i != index) {
			temporary_array[k] = array[i];
			k++;
		}
	}
	delete[] array;
	array = temporary_array;
}

void increase_array(int** &array, int rows, int cols, int new_rows, int new_cols, int new_value) {
	int** temp_matrix = new int* [new_rows];

	for (int i = 0; i < new_rows; i++)
		temp_matrix[i] = new int[new_cols];

	for (int i = 0; i < new_rows; i++) {
		for (int j = 0; j < new_cols; j++) {
			if (i < rows && j < cols) temp_matrix[i][j] = array[i][j];
			else temp_matrix[i][j] = new_value;
		}
	}

	delete_array(array, rows);
	array = temp_matrix;
}

void increase_array(int*& array, int size, int new_size, int new_value) {
	int* temp = new int[new_size];

	for (int i = 0; i < new_size; i++) {
		if (i < size) temp[i] = array[i];
		else temp[i] = new_value;
	}

	delete[] array;
	array = temp;
}


int min(int x, int y) {
	if (x < y) return x;
	else return y;
}

void reference_plan(int** distribution_matrix, int* suppliers_stocks, int* consumers_needs, 
					int number_of_suppliers, int number_of_consumers) {
	int temp;

	int start_consumer = 0;
	for (int i = 0; i < number_of_suppliers; i++) {
		for (int j = start_consumer; j < number_of_consumers; j++) {									
			//минимальное значение запас/потребность
			//вычитание min у поставщика и потребителя
			temp = min(suppliers_stocks[i], consumers_needs[j]);

			suppliers_stocks[i] -= temp;
			consumers_needs[j] -= temp;


			//запись в опорный план
			distribution_matrix[i][j] = temp;

			//проверка на вырожденность плана
			if (consumers_needs[i] == 0 && suppliers_stocks[j] == 0 && i + 1 != number_of_suppliers && j + 1 != number_of_consumers) {
				distribution_matrix[i][j + 1] = 0;
				start_consumer++;
				degenerate_plan = true;
				break;
			}

			//если потребность = 0 -> потребитель больше не участвует
			if (consumers_needs[j] == 0)
				start_consumer++;

			//если запасы поставщика = 0 -> переход к след. поставщику
			if (suppliers_stocks[i] == 0) break;
		}
	}
}

//стоимость всех перевозок
int cost_of_transportation(int** tariff_matrix, int** distribution_matrix, int number_of_suppliers, int number_of_consumers) {
	
	int cost = 0;
	for (int i = 0; i < number_of_suppliers; i++) {
		for (int j = 0; j < number_of_consumers; j++)
			if (distribution_matrix[i][j] != 0)
				cost += tariff_matrix[i][j] * distribution_matrix[i][j];
	}
	return cost;
}

void print_array(int** matrix, int number_of_suppliers, int number_of_consumers, bool print_null) {
	for (int i = 0; i < number_of_consumers; i++)
		std::cout << "\t" << "B" << i + 1;

	std::cout << std::endl << std::endl;
	for (int i = 0; i < number_of_suppliers; i++) {
		std::cout << "A" << i + 1 << "\t";
		for (int j = 0; j < number_of_consumers; j++) {
			if (matrix[i][j] == -1 && !print_null) std::cout << "-" << "\t";
			else std::cout << matrix[i][j] << "\t";
		}
		std::cout << std::endl;
	}
}

void print_array(int* array, int size) {
	for (int i = 0; i < size; i++)
		std::cout << array[i] << "\t";
}

int main()
{
	setlocale(LC_ALL, "Rus");

	int number_of_suppliers = suppliers_count;
	int number_of_consumers = consumers_count;
	
	int** tariff_matrix = new int*[number_of_suppliers];
	for (int i = 0; i < number_of_suppliers; i++) {
		tariff_matrix[i] = new int[number_of_consumers];
	}	

	int* suppliers = new int[number_of_suppliers];
	int* consumers = new int[number_of_consumers];

	read_from_file(tariff_matrix, suppliers, consumers, number_of_suppliers, number_of_consumers);

	int total_stocks = array_sum(suppliers, number_of_suppliers);
	int total_needs = array_sum(consumers, number_of_consumers);

	//проверка на закрытость транспортной задачи
	if (total_stocks < total_needs) {
		increase_array(suppliers, number_of_suppliers, number_of_suppliers + 1, total_needs - total_stocks);
		increase_array(tariff_matrix, number_of_suppliers, number_of_consumers, number_of_suppliers + 1, number_of_consumers, 0);
		number_of_suppliers++;
	}
	else if (total_stocks > total_needs) {
		increase_array(consumers, number_of_consumers, number_of_consumers + 1, total_stocks - total_needs);
		increase_array(tariff_matrix, number_of_suppliers, number_of_consumers, number_of_suppliers, number_of_consumers + 1, 0);
		number_of_consumers++;
	}

	//создание таблицы опорного плана 
	int** distribution_matrix = new int* [number_of_suppliers];
	for (int i = 0; i < number_of_suppliers; i++)
		distribution_matrix[i] = new int[number_of_consumers];

	for (int i = 0; i < number_of_suppliers; i++) {
		for (int j = 0; j < number_of_consumers; j++) {
			distribution_matrix[i][j] = -1;
		}
	}


	//вывод матрицы тарифов
	std::cout << "Матрица тарифов\n\n";
	print_array(tariff_matrix, number_of_suppliers, number_of_consumers, true);

	std::cout << "\n\nПоставщики: " << std::endl;
	print_array(suppliers, number_of_suppliers);

	std::cout << "\n\nПотребители: " << std::endl;
	print_array(consumers, number_of_consumers);


	//опорное решение
	reference_plan(distribution_matrix, suppliers, consumers, number_of_suppliers, number_of_consumers);

	std::cout << std::endl;
	//вывод опорного решения
	std::cout << "\nОпорное решение\n\n";
	print_array(distribution_matrix, number_of_suppliers, number_of_consumers, false);

	if (degenerate_plan) std::cout << "\nПлан вырожденный. Добавлена нулевая перевозка." << std::endl;

	std::cout << std::endl << "Стоимость перевозок = " << 
		cost_of_transportation(tariff_matrix, distribution_matrix, number_of_suppliers, number_of_consumers) << " рублей." << std::endl;

	delete_array(distribution_matrix, number_of_suppliers);
	delete_array(tariff_matrix, number_of_suppliers);
	delete[] suppliers;
	delete[] consumers;
	return 0;
}