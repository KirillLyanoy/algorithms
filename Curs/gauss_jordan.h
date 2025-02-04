#pragma once
#include <iostream>
#include <cmath>

//Точность
const int e = 1000;

//Проверка на ступенчатый вид
bool row_echelon_form_check(double** matrix, int numbers_of_variables, int numbers_of_equations) {
    if (numbers_of_equations != numbers_of_variables) return false;

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
void fill_matrix(double** matrix, int numbers_of_variables, int numbers_of_equations) {
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
    std::cout << std::endl << "----------------------------------------------------" << std::endl;
    for (int i = 0; i < numbers_of_equations; i++) {
        for (int j = 0; j < numbers_of_variables + 1; j++)
            std::cout << matrix[i][j] << "\t";
        std::cout << std::endl;
    }
}

void copy_matrix(double** fromMatrix, double** toMatrix, int numbers_of_variables, int numbers_of_equations) {
    for (int i = 0; i < numbers_of_equations; i++) {
        for (int j = 0; j < numbers_of_variables + 1; j++) {
            toMatrix[i][j] = fromMatrix[i][j];
        }
    }
}

//удаление массива
void delete_matrix(double** matrix, int numbers_of_equations) {
    for (int i = 0; i < numbers_of_equations; i++)
        delete[] matrix[i];
    delete[] matrix;
}

template <typename T>
void delete_array(T* array) {
    if (array != nullptr) 
        delete[] array;
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

    delete_matrix(*old_matrix, numbers_of_equations);
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

        delete_matrix(matrix, current_numbers_of_equations);
        //замена временной матрицы на основную
        matrix = temp_matrix;
        current_numbers_of_equations = numbers_of_equations;

        print_matrix(matrix, numbers_of_variables, numbers_of_equations);

    }
    //delete_matrix(matrix, numbers_of_equations);
}