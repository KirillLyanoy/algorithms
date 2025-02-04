#include <iostream>
#include <time.h>

const int matrix_size = 2;
const int number_of_parties = 1000;

enum players {
    ONE = 1,
    TWO = 2
};

//проверка на сведловую точку
bool saddle_point_check(int** matrix) {
  
    int* min_rows = new int[matrix_size];
    int* max_cols = new int[matrix_size];
    
    int min, max;

    for (int i = 0; i < matrix_size; i++) {

        min = matrix[i][0];
        for (int j = 0; j < matrix_size; j++) {
            if (matrix[i][j] < min) min = matrix[i][j];
        }
        min_rows[i] = min;
    }
    for (int j = 0; j < matrix_size; j++) {

        max = matrix[0][j];
        for (int i = 0; i < matrix_size; i++) {
            if (matrix[i][j] > max) max = matrix[i][j];
        }
        max_cols[j] = max;
    }   

    max = min_rows[0];
    for (int i = 0; i < matrix_size; i++) {
        if (min_rows[i] > max) max = min_rows[i];
    }

    min = max_cols[0];
    for (int i = 0; i < matrix_size; i++) {
        if (min_rows[i] < min) max = min_rows[i];
    }    
    delete[] min_rows, max_cols;
    
    if (min != max) return false;
    else return true;
}

template <typename T>
void delete_matrix(T** matrix) {
    for (int i = 0; i < matrix_size; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

//вычисление теоретической цены методом крамера
void theoretical_price_calculation(int** matrix, double* result, players player) {
    
    int a1, a2, b1, b2, c1, c2;
    if (player == ONE) {
        a1 = matrix[0][0] - matrix[0][1];
        b1 = matrix[1][0] - matrix[1][1];
        c1 = 0;
        a2 = 1; b2 = 1; c2 = 1;
    }
    else if (player == TWO) {
        a1 = matrix[0][0] - matrix[1][0];
        b1 = matrix[0][1] - matrix[1][1];
        c1 = 0;
        a2 = 1; b2 = 1; c2 = 1;
    }

    double det = a1 * b2 - a2 * b1;

    if (det == 0) {
        std::cout << "Решений нет." << std::endl;
    }
    else {
        result[0] = round(((c1 * b2 - c2 * b1) / det) * 1000) / 1000;
        result[1] = round(((a1 * c2 - a2 * c1) / det * 1000)) / 1000;
    }

    if (player == ONE) {
        result[2] = round((matrix[0][0] * result[0] + matrix[1][0] * result[1]) * 1000)  / 1000;
    }
    else if (player == TWO) {
        result[2] = round((matrix[0][0] * result[0] + matrix[0][1] * result[1]) * 1000) / 1000 ;
    }
    
}

//заполнение случайными числами
void fill_random_numbers(double* array) {
    for (int i = 0; i < number_of_parties; i++) {
        array[i] =  double(rand()) / RAND_MAX;
        array[i] = round(array[i] * 1000) / 1000;
    }
}

//розыгрыш 1000 партий игры
void practical_price_calculation(int** matrix, double* random_number_player_one, double* random_number_player_two, double* practical_result) {

    double win = 0;
    double average_win = 0;
    double all_win = 0;

    int a1 = 0;
    int a2 = 0;
    int b1 = 0;
    int b2 = 0;

    std::cout << "Номер\t" << "Случайное\t" << "Стратегия\t" << "Случайное\t" << "Стратегия\t" << "Выигрыш\t\t" << "Накопленный\t" << "Средний" << std::endl;
    std::cout << "партии\t" << "число (А)\t" << "игрока (A)\t" << "число (B)\t" << "игрока (B)\t" << "(А)\t\t" << "выигрыш (A)\t" << "выигрыш (А)" << std::endl << std::endl;

    for (int i = 0; i < number_of_parties; i++) {

        std::cout << i + 1 << "\t";

        if (random_number_player_one[i] < 0.5) {
            if (random_number_player_two[i] < 0.5) {
                win = matrix[0][0];
                std::cout << random_number_player_one[i] << "\t\tA1\t\t" << random_number_player_two[i] << "\t\tB1\t";
                a1++; 
                b1++;
            }
            else {
                win = matrix[0][1];
                std::cout << random_number_player_one[i] << "\t\tA1\t\t" << random_number_player_two[i] << "\t\tB2\t";
                a1++;
                b2++;
            }
        }
        else {
            if (random_number_player_two[i] < 0.5) {
                win = matrix[1][0];
                std::cout << random_number_player_one[i] << "\t\tA2\t\t" << random_number_player_two[i] << "\t\tB1\t";
                a2++;
                b1++;
            }
            else {
                win = matrix[1][1];
                std::cout << random_number_player_one[i] << "\t\tA2\t\t" << random_number_player_two[i] << "\t\tB2\t";
                a2++;
                b2++;
            }
        }
        all_win += win;
        average_win = round(all_win / (i + 1) * 1000) / 1000;
        std::cout << "\t" << win << "\t\t" << all_win << "\t\t" << average_win << std::endl;

        practical_result[0] = a1;
        practical_result[1] = a2;
        practical_result[2] = b1;
        practical_result[3] = b2;
        practical_result[4] = average_win;
    }
}

int main()
{
    setlocale(LC_ALL, "rus");
    srand(time(NULL));

    int** matrix = new int* [matrix_size];

    matrix[0] = new int [matrix_size] { 7, 15 };
    matrix[1] = new int [matrix_size] { 22, 11 };

    if (saddle_point_check(matrix)) {
        delete_matrix(matrix);
        return 0;
    }

    double* result_player_one = new double[matrix_size + 1];
    double* result_player_two = new double[matrix_size + 1];

    theoretical_price_calculation(matrix, result_player_one, ONE);
    theoretical_price_calculation(matrix, result_player_two, TWO);


    double* random_number_player_one = new double[number_of_parties];
    fill_random_numbers(random_number_player_one);

    double* random_number_player_two = new double[number_of_parties];
    fill_random_numbers(random_number_player_two);
    
    //for (int i = 0; i < number_of_parties; i++) std::cout << random_number_player_one[i] << "\t" << random_number_player_two[i] << std::endl;

    double* practical_result = new double[5];
    practical_price_calculation(matrix, random_number_player_one, random_number_player_two, practical_result);

    std::cout << std::endl;
    std::cout << "Теоретический результат:" << std::endl;
    std::cout << "p = (" << result_player_one[0] << ", " << result_player_one[1] << ")\tv = " << result_player_one[2] << std::endl;
    std::cout << "q = (" << result_player_two[0] << ", " << result_player_two[1] << ")\tv = " << result_player_two[2] << std::endl << std::endl;

    std::cout << "Практический результат:" << std::endl;
    std::cout << "Игрок А использовал стратегию А1: " << practical_result[0] << "/" << number_of_parties << std::endl;
    std::cout << "Игрок А использовал стратегию А2: " << practical_result[1] << "/" << number_of_parties << std::endl;
    std::cout << "Игрок B использовал стратегию B1: " << practical_result[2] << "/" << number_of_parties << std::endl;
    std::cout << "Игрок B использовал стратегию B2: " << practical_result[3] << "/" << number_of_parties << std::endl;
    std::cout << "Средний выигрыш игрока А: " << practical_result[4] << std::endl;

    delete[] practical_result;
    delete[] result_player_one;
    delete[] result_player_two;
    delete[] random_number_player_one;
    delete[] random_number_player_two;
    delete_matrix(matrix);

    return 0;
}
