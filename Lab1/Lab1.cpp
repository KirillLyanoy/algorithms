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

//вывод результата
void print_result(double** matrix, int numbers_of_variables, int numbers_of_equations) {

    std::cout << std::endl;

    bool no_solution;
    //проверка на 0 != X
    for (int i = 0; i < numbers_of_equations; i++) {      
        no_solution = true;
        for (int j = 0; j < numbers_of_variables; j++)
        {
            if (matrix[i][j] != 0) {
                no_solution = false;
                break;
            }
        }
        if (no_solution && matrix[i][numbers_of_variables + 1] != 0) no_solution = true;
    }
    if (no_solution) {
        std::cout << "Решений нет" << std::endl;
        return;
    }
    else { 
        //проверка матрицы на ступенчатость
        if (row_echelon_form_check(matrix, numbers_of_variables, numbers_of_equations)) {
            //вывод ступенчатой матрицы
            for (int i = 0; i < numbers_of_equations; i++)
                std::cout << "x" << i + 1 << " = " << matrix[i][numbers_of_variables] << "\n";
        }
        else {
            if (numbers_of_equations < numbers_of_variables) {
                for (int i = 0; i < numbers_of_equations; i++) {
                    for (int j = 0; j < numbers_of_variables; j++) {     

                        //проверка столбца на преобразованность
                        if (check_col(matrix, numbers_of_equations, j, i)) {

                            //выражение переменных 
                            std::cout << "x" << j + 1 << " = ";
                            if (matrix[i][numbers_of_variables] != 0) std::cout << matrix[i][numbers_of_variables] << " ";

                            for (int variables = 0; variables < numbers_of_variables; variables++) {
                                if (variables == j || matrix[i][variables] == 0) continue;
                                else {
                                    if (matrix[i][variables] < 0) std::cout << " + " << abs(matrix[i][variables]) << "x" << variables + 1;
                                    else std::cout << " - " << matrix[i][variables] << "x" << variables + 1;
                                }
                            }
                            break;
                        }
                        else continue;
                    }
                    std::cout << std::endl;
                }                
            }
        }
    }
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

//удаление строки из матрицы
void remove_string(double*** old_matrix, int string_to_del, int numbers_of_variables, int &numbers_of_equations) {
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

void gauss_jordan_elementation(double** matrix, int &numbers_of_variables, int &numbers_of_equations) {
    
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
    print_result(matrix, numbers_of_variables, numbers_of_equations);
    delete_matrix(matrix, numbers_of_equations);
}

int main()
{
    setlocale(LC_ALL, "Rus");
    char c;
    double** matrix;
    int numbers_of_variables; 
    int numbers_of_equations;

    while (true) {
        system("cls");

        std::cout << "1. Первый пример (6 вариант)\n" << std::endl;
        std::cout << "2. Второй пример\n" << std::endl;
        std::cout << "3. Третий пример\n" << std::endl;
        std::cout << "4. Четвертый пример\n" << std::endl;
        std::cout << "5. Свой пример\n" << std::endl;
        std::cout << "6. Выход\n" << std::endl;

        std::cin >> c;
        switch (c) {
            case ('1'):
                system("cls");
                numbers_of_variables = 5;
                numbers_of_equations = 5;
                matrix = new double* [5];
               
                matrix[0] = new double[6] { 6, 9, -2, -8, -8, -144 };
                matrix[1] = new double[6] { 14, 11, 4, 7, 3, -32 };
                matrix[2] = new double[6] { 8, 10, 2, 4, -1, -59 };
                matrix[3] = new double[6] { 8, -4, 6, -5, -3, -10 };
                matrix[4] = new double[6] { -1, 6, -7, 7, 7, 14 };  

              
                gauss_jordan_elementation(matrix, numbers_of_variables, numbers_of_equations);                  
                break;
            case ('2'):
                numbers_of_variables = 4;
                numbers_of_equations = 4;
                matrix = new double* [4];

                matrix[0] = new double[5] { 4, -17, -6, -5, -17 };
                matrix[1] = new double[5] { 43, 24, -1, 3, 28 };
                matrix[2] = new double[5] { 0, 1, 2, 1, 9 };
                matrix[3] = new double[5] { 2, 1, 0, 0, 1 };

                gauss_jordan_elementation(matrix, numbers_of_variables, numbers_of_equations);                
                break;
            case ('3'):
                numbers_of_variables = 4;
                numbers_of_equations = 3;
                matrix = new double* [4];

                matrix[0] = new double[5] { 3, 2, 5, 4, 3 };
                matrix[1] = new double[5] { 1, -1, -1, -4, -2 };
                matrix[2] = new double[5] { 4, 1, 4, 0, 2 };

                gauss_jordan_elementation(matrix, numbers_of_variables, numbers_of_equations);
                break;
            case ('4'):
                numbers_of_variables = 4;
                numbers_of_equations = 3;
                matrix = new double* [4];

                matrix[0] = new double[5] { 2, -3, 5, 7, 1 };
                matrix[1] = new double[5] { 4, -6, 2, 3, 2 };
                matrix[2] = new double[5] { 2, -3, -11, -15, 1 };

                gauss_jordan_elementation(matrix, numbers_of_variables, numbers_of_equations);
                break;
            case ('5'):
                std::cout << "Введите количество уравнений: ";
                std::cin >> numbers_of_equations;
                std::cout << "Введите количество переменных: ";
                std::cin >> numbers_of_variables;

                matrix = new double* [numbers_of_equations];
                for (int i = 0; i < numbers_of_equations; i++) {
                    matrix[i] = new double[numbers_of_variables + 1];
                }
                fill_matrix(matrix, numbers_of_variables, numbers_of_equations);

                gauss_jordan_elementation(matrix, numbers_of_variables, numbers_of_equations);
                break;
            case ('6'):
                
                return 0;
            default:
                break;
        }
        system("pause");
    }
}