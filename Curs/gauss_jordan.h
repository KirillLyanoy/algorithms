#pragma once
#include <iostream>
#include <cmath>
#include <iomanip>

//Точность
const int e = 100000;

//Проверка на ступенчатый вид
bool row_square_echelon_form_check(double** matrix, int numbers_of_variables, int numbers_of_equations) {
    //if (numbers_of_equations != numbers_of_variables) return false;

    for (int i = 0; i < numbers_of_equations; i++) {
        for (int j = 0; j < numbers_of_variables; j++) {
            if (i == j) {
                if (matrix[i][j] != 1) return false;
            }
            else {
                if (matrix[i][j] != 0) return false;
            }
        }
    }
    return true;
}

//проверка на базисный столбец
bool col_basis_check(double** matrix, int numbers_of_equations, int column, int basis_row_with_one) {

    for (int i = 0; i < numbers_of_equations; i++) {

        if (i == basis_row_with_one) {
            if (matrix[i][column] != 1) return false;
        }
        else {
            if (matrix[i][column] != 0) return false;
        }
    }

    return true;
}

//Проверка на ступенчатый вид
bool row_echelon_form_check(double** matrix, int numbers_of_variables, int numbers_of_equations) {

    int k = numbers_of_equations;
    int start_variable = 0;

    for (int i = 0; i < numbers_of_equations; i++) {
        for (int j = start_variable; j < numbers_of_variables; j++) {

            if (matrix[i][j] == 1 && col_basis_check(matrix, numbers_of_equations, j, i)) {
                k--;
                start_variable = j + 1;
                break;
            }
        }
    }
    if (k == 0) return true;
    else return false;
}

//проверка на отрицательный свободный коэффициент
void check_negative_coefficients(double** matrix, int numbers_of_variables, int numbers_of_equations) {
    for (int i = 0; i < numbers_of_equations; i++) {
        if (matrix[i][numbers_of_variables] < 0) {
            for (int j = 0; j < numbers_of_variables + 1; j++) {
                matrix[i][j] = 0 - matrix[i][j];
            }
        }
    }
}

//проверка принадлежности столбца к преобразованной матрице
bool check_col(double** matrix, int numbers_of_equations, int checked_col, int resolving_row) {
    for (int i = 0; i < numbers_of_equations; i++) {
        if (i == resolving_row) {
            if (matrix[i][checked_col] != 1) return false;
        }
        else
            if (matrix[i][checked_col] != 0) return false;
    }
    return true;
}

//заполнеине массива
void fill_array(double** matrix, int numbers_of_variables, int numbers_of_equations) {
    for (int i = 0; i < numbers_of_equations; i++) {
        for (int j = 0; j < numbers_of_variables + 1; j++) {
            std::cout << "Введите " << j + 1 << " константу " << i + 1 << " строки: ";
            std::cin >> matrix[i][j];
            std::cout << std::endl;
        }
    }
}

//печать массива
void print_matrix(double** matrix, int numbers_of_variables, int numbers_of_equations) {
    std::cout << std::endl << "-------------------------------------------------------------" << std::endl;
    for (int i = 0; i < numbers_of_equations; i++) {
        for (int j = 0; j < numbers_of_variables + 1; j++)
            std::cout << std::setw(10) << matrix[i][j];
        if (i + 1 < numbers_of_equations) std::cout << std::endl;
    }
}

//копирование массива
void copy_array(double** fromMatrix, double** toMatrix, int numbers_of_variables, int numbers_of_equations) {
    for (int i = 0; i < numbers_of_equations; i++) {
        for (int j = 0; j < numbers_of_variables + 1; j++) {
            toMatrix[i][j] = fromMatrix[i][j];
        }
    }
}

//копирование массива
void copy_array(double* fromMatrix, double* toMatrix, int size) {
    for (int i = 0; i < size; i++) {
        toMatrix[i] = fromMatrix[i];
    }
}

//удаление массива
void delete_array(double** matrix, int numbers_of_equations) {
    for (int i = 0; i < numbers_of_equations; i++)
        delete[] matrix[i];
    delete[] matrix;
}

//удаление массива
template <typename T>
void delete_array(T* array) {
    if (array != nullptr) 
        delete[] array;
}

//добавление элемента в массив
template <typename T>
void add_element_to_array(T* &array, int size) {
    if (array == nullptr) return;
    T* new_arr = new T[size + 1];
    for (int i = 0; i < size; i++) {
        new_arr[i] = array[i];
    }
    delete[] array;
    array = new_arr;
}

//индекс максимального элемента массива
int index_of_max_element(double* array, int size, int start_index) {

    if (array == nullptr || size == 0 || start_index < 0 || start_index >= size) return -1;

    int index = start_index;

    for (int i = 0; i < size; i++) {
        if (array[i] > array[index]) index = i;
    }

    return index;
}

//индекс минимального элемента массива
int index_of_min_element(double* array, int size, int start_index) {

    if (array == nullptr || size == 0 || start_index < 0 || start_index >= size) return -1;

    int index = start_index;

    for (int i = 0; i < size; i++) {
        if (array[i] < array[index]) index = i;
    }

    return index;
}

//удаление строки из матрицы
void remove_string(double*** old_matrix, int string_to_del, int numbers_of_variables, int& numbers_of_equations) {
    double** new_matrix = new double* [numbers_of_equations - 1];
    for (int i = 0; i < numbers_of_equations - 1; i++) {
        new_matrix[i] = new double[numbers_of_variables + 1];
    }

    int old_matrix_index = 0;
    int new_matrix_index = 0;

    for (new_matrix_index = 0; new_matrix_index < numbers_of_equations - 1; new_matrix_index++, old_matrix_index++) {
        if (new_matrix_index != string_to_del) {
            for (int j = 0; j < numbers_of_variables + 1; j++) {
                new_matrix[new_matrix_index][j] = (*old_matrix)[old_matrix_index][j];
            }
        }
        else old_matrix_index++;
    }

    delete_array(*old_matrix, numbers_of_equations);
    *old_matrix = new_matrix;
    numbers_of_equations--;
}

void gauss_jordan_elementation(double** &matrix, int& numbers_of_variables, int& numbers_of_equations) {

    int current_numbers_of_equations = numbers_of_equations;

    int iteration = 0;

    while (iteration < numbers_of_equations) {

        double** temp_matrix = new double* [numbers_of_equations];
        for (int i = 0; i < numbers_of_equations; i++) {
            temp_matrix[i] = new double[numbers_of_variables + 1];
        }

        int resolving_el_row = iteration;
        int resolving_el_col = iteration;
         
        //проверка разрешающего элемента != 0
        while (matrix[resolving_el_row][resolving_el_col] == 0 && resolving_el_col < numbers_of_variables) {
            bool is_changed = false;
            for (int i = resolving_el_row + 1; i < numbers_of_equations; i++) {
                if (matrix[i][resolving_el_col] != 0) {
                    //перестановка уравнений если разрешающий элемент = 0
                    double* temp = matrix[resolving_el_row];
                    matrix[resolving_el_row] = matrix[i];
                    matrix[i] = temp;
                    is_changed = true;
                    break;
                }
            }
            if (!is_changed)
                resolving_el_col++;
        }

        //if (matrix[resolving_el_row][resolving_el_col] == 0) {
        //    printf("Решений нет.\n");
        //    return;
        //}


        //копирование преобразованных элементов матрицы во временную матрицу
        for (int i = 0; i < numbers_of_equations; i++) {
            for (int j = 0; j < resolving_el_col; j++) {
                temp_matrix[i][j] = matrix[i][j];
            }
        }

        for (int i = 0; i < numbers_of_equations; i++) {
            if (i == resolving_el_row) {
                for (int j = resolving_el_col; j < numbers_of_variables + 1; j++) {
                    //деление всех элементов строки на разрешающий элемент
                    temp_matrix[i][j] = std::round((matrix[i][j] / matrix[resolving_el_row][resolving_el_col]) * e) / e;
                }
            }
            else
                //все нули столбца с разрешающим элементом = 0
                temp_matrix[i][resolving_el_col] = 0;
        }

        for (int i = 0; i < numbers_of_equations; i++) {
            if (i != resolving_el_row) {
                for (int j = resolving_el_col + 1; j < numbers_of_variables + 1; j++) {
                    //вычисление по правилу прямоугольника
                    temp_matrix[i][j] = std::round((matrix[i][j] - (matrix[resolving_el_row][j] * matrix[i][resolving_el_col]) / matrix[resolving_el_row][resolving_el_col]) * e) / e;
                }
            }
        }
        //удаление нулевой строки
        bool null_string = true;
        for (int i = resolving_el_row + 1; i < numbers_of_equations; i++) {
            for (int j = 0; j < numbers_of_variables + 1; j++)
            {
                if (temp_matrix[i][j] != 0) {
                    null_string = false;
                    break;
                }
            }
            if (null_string) remove_string(&temp_matrix, i, numbers_of_variables, numbers_of_equations);
            null_string = true;
        }

        iteration++;

        delete_array(matrix, current_numbers_of_equations);
        //замена временной матрицы на основную
        matrix = temp_matrix;
        current_numbers_of_equations = numbers_of_equations;

        print_matrix(matrix, numbers_of_variables, numbers_of_equations);

    }
    //delete_matrix(matrix, numbers_of_equations);
}