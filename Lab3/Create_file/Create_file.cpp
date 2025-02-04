#include <iostream>
#include <fstream>
#include <string>

//количество поставщиков
int number_of_suppliers;		
//количество потребителей
int number_of_consumers;		


//функция вывода двумерного массива на экран
void print_matrix(int** matrix) {
	for (int i = 0; i < number_of_suppliers; i++) {
		for (int j = 0; j < number_of_consumers; j++) {
			std::cout << matrix[i][j] << "\t";
		}
		std::cout << std::endl;
	}
}

//функция вывода одномерного массива на экран
void print_matrix(int* matrix, int size) {
	for (int i = 0; i < size; i++) 
		 std::cout << matrix[i] << "\t";	
	std::cout << std::endl;
}

void write_data_to_binary_file(char* filename, int** tariff_matrix, int* suppliers, int* consumers) {

	FILE* file;

	fopen_s(&file, filename, "wb");

	if (file == nullptr) {
		std::cout << "Ошибка при открытии файла." << std::endl;
		return;
	}

	size_t size;

	for (int i = 0; i < number_of_suppliers; i++) {
		size = fwrite(tariff_matrix[i], sizeof(int), number_of_consumers, file);
	}
	size = fwrite(suppliers, sizeof(int), number_of_suppliers, file);
	size = fwrite(consumers, sizeof(int), number_of_consumers, file);

	fclose(file);
}

void read_arrays_from_file(char* filename, int** tariff_matrix, int* suppliers, int* consumers) {
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

void fill_array_random(int** array, int cols, int rows) {
	for (int i = 0; i < cols; i++) {
		for (int j = 0; j < rows; j++) {
			array[i][j] = (1 + rand() % 99) * 100;
		}
	}
}

void fill_array_random(int* array, int size) {
	for (int i = 0; i < size; i++) {
		array[i] = 10 + (rand() % 50) * 10;
	}
}

void fill_array(int* array, int size) {
	for (int i = 0; i < size; i++) {
		std::cout << "Введите " << i + 1 << " элемент: " << std::endl;

		while (true) {
			std::cin >> array[i];
			if (array[i] < 0 || array[i] > 10000) std::cout << "Неправильное значение. Повторите" << std::endl;
			else break;
		}		
	}
}

int main()
{
	//предварительное создание файла с матрицами
	srand(time(NULL));
	setlocale(LC_ALL, "Rus");

	while (true) {
		std::cout << "Введите количество поставщиков: ";
		std::cin >> number_of_suppliers;
		if (number_of_suppliers < 1) std::cout << "Количество поставщиков не может быть меньше 1.\n";
		else {
			if (number_of_suppliers > 1000) std::cout << "\n Количество поставщиков не может быть больше 1000.\n";
			else break;
		}
	}

	while (true) {
		std::cout << "Введите количество потребителей: ";
		std::cin >> number_of_consumers;
		if (number_of_consumers < 1) std::cout << "Количество потребителей не может быть меньше 1.\n";
		else {
			if (number_of_consumers > 1000) std::cout << "\n Количество потребителей не может быть больше 1000.\n";
			else break;
		}
	}

	std::string name = "data" + std::to_string(number_of_suppliers) + "_" + std::to_string(number_of_consumers) + ".bin";

	char filename[15];
	strcpy_s(filename, name.c_str());

	//матрица тарифов
	int** tariff_matrix = new int* [number_of_suppliers];
	for (int i = 0; i < number_of_suppliers; i++)
		tariff_matrix[i] = new int[number_of_consumers];
	int* stocks = new int[number_of_suppliers];
	int* needs = new int[number_of_consumers];

	//заполнение матрицы тарифов случайными числами
	fill_array_random(tariff_matrix, number_of_suppliers, number_of_consumers);

	std::cout << "1. Заполнить случайными числами" << std::endl;
	std::cout << "2. Заполнить вручную" << std::endl;
	char c;

	std::cin >> c;


	switch (c) {
	case ('1'):
		//заполнение запасов поставщика случайными числами
		fill_array_random(stocks, number_of_suppliers);
		//заполнение потребностей потребителя случайными числами
		fill_array_random(needs, number_of_consumers);
		break;
	case ('2'):
		//заполнение запасов поставщика вручную
		std::cout << std::endl << "Ввод поставщиков: " << std::endl;
		fill_array(stocks, number_of_suppliers);
		//заполнение потребностей потребителя вручную
		std::cout << "Ввод потребителей: " << std::endl;
		fill_array(needs, number_of_consumers);
		break;
	default:
		return 0;
	}	


	//запись в файл
	write_data_to_binary_file(filename, tariff_matrix, stocks, needs);	

	//вывод на экран
	print_matrix(tariff_matrix);
	std::cout << std::endl;
	print_matrix(stocks, number_of_suppliers);
	std::cout << std::endl;
	print_matrix(needs, number_of_consumers);

	//удаление матриц
	delete[] stocks;
	delete[] needs;
	for (int i = 0; i < number_of_suppliers; i++) {
		delete[] tariff_matrix[i];
	}
	delete[] tariff_matrix;
	return 0;
}