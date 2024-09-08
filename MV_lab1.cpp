#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

//чтение файла
bool readFile(vector <vector <double> >& matrix)
{
	int str_start = 0, str_end = 1, str_num = -1;
	double number;
	bool result = false, end = false;
	string str;

	//открытие файла с исходными данными
	ifstream f("input.txt");

	//файл для чтения не открылся
	if (!f)
	{
		cout << " Не удалось открыть файл 'input.txt'" << endl;
		result = true;
	}

	if (!result)
	{
		//заполнение матрицы системы уравнений
		while (str_start != str_end && !end)
		{
			str_start = f.tellg();
			getline(f, str);
			str_end = f.tellg();
			str_end -= 2;
			f.seekg(str_start);

			if (str_start != str_end)
			{
				matrix.push_back(vector <double>());
				str_num++;
			}

			while (f.tellg() < str_end)
			{
				f >> number;
				matrix[str_num].push_back(number);
			}

			getline(f, str);

			if (str_num > matrix[0].size()) 
			{
				end = true;
			}
		}

		if (!end)
		{
			str_num = 0;

			//заполнение столбца свободных членов
			while (!f.eof())
			{
				f >> number;
				matrix[str_num++].push_back(number);
			}

			//проверка числа строк и столбцов
			for (int i = 1; i < str_num && !end; i++)
			{
				if (matrix[0].size() != matrix[i].size())
					end = true;
			}

			if (str_num + 1 != matrix[0].size())
			{
				end = true;
			}
		}

		f.close();

		if (end)
		{
			cout << "Неверный формат данных!" << endl;
			result = true;
		}
	}

	return result;
}

//вывод матрицы
void printMatrix(vector <vector <double> > matrix)
{
	int k = matrix.size();

	for (int i = 0; i < k; i++)
	{
		for (int j = 0; j < k; j++)
		{
			cout << matrix[i][j] << " ";
		}

		cout << endl;
	}

	return;
}

//вывод вектора
void printVector(vector <double> vec)
{
	int k = vec.size();

	for (int i = 0; i < k; i++)
	{
		cout << vec[i] << " " << endl;
	}

	cout << endl;

	return;
}

//вывод вектора
void printVector(vector <vector <double> > matrix, int column)
{
	for (int i = 0; i < column; i++)
	{
		cout << matrix[i][column] << " " << endl;
	}

	cout << endl;

	return;
}

//LU-разложение
vector <vector <double> > LU_decomp(vector <vector <double> > matrix)
{
	vector <vector <double> > result;

	result = matrix;

	int n = matrix.size();

	for (int k = 0; k < n;)
	{
		if (result[k][k] == 0)
		{
			cout << "Главный минор равен нулю! Нельзя применить LU-разложение.";
			exit(0);
		}

		//столбец L
		for (int i = k + 1; i < n; i++)
		{
			double sum = 0;

			for (int p = 0; p < k; p++)
			{
				sum += result[i][p] * result[p][k];
			}

			result[i][k] = (result[i][k] - sum) / result[k][k];
		}

		k++;

		//строка U
		for (int j = k; j < n; j++)
		{
			double sum = 0;

			for (int p = 0; p < k; p++)
			{
				sum += result[k][p] * result[p][j];
			}

			result[k][j] -= sum;
		}
	}

	return result;
}

//решение системы Ly = b (поиск y)
vector <vector <double> > countY(vector <vector <double> > matrix)
{
	vector <vector <double> > result;

	result = matrix;

	int n = matrix.size();

	for (int i = 0; i < n; i++)
	{
		double sum = 0;

		for (int k = 0; k < i; k++)
		{
			sum += result[k][n] * result[i][k];
		}

		result[i][n] -= sum;
	}

	return result;
}

//решение системы Ux = y (поиск x)
vector <double> countX(vector <vector <double> > matrix)
{
	vector <double> result;

	int n = matrix.size();

	result.resize(n);

	for (int i = n - 1; i >= 0; i--)
	{
		double sum = 0;

		for (int k = i + 1; k < n; k++)
		{
			sum += matrix[i][k] * result[k];
		}

		result[i] = (matrix[i][n] - sum) / matrix[i][i];
	}

	return result;
}

int main()
{
	setlocale(LC_ALL, "Rus");

	vector <vector <double> > matrix;
	vector <vector <double> > LU_matrix;
	vector <vector <double> > triangle_matrix;
	vector <double> vector_X;

	if (readFile(matrix))
	{
		return -1;
	}

	cout << "--- ДАНО ---" << endl;
	cout << "Матрица A" << endl;
	printMatrix(matrix);

	int n = matrix.size();

	cout << endl << "Вектор B" << endl;
	printVector(matrix, n);

	LU_matrix = LU_decomp(matrix);

	cout << "--- РЕШЕНИЕ ---" << endl;
	cout << "Матрица L - E + U" << endl;
	printMatrix(LU_matrix);
	
	triangle_matrix = countY(LU_matrix);

	cout << endl << "Вектор Y" << endl;
	printVector(triangle_matrix, n);

	vector_X = countX(triangle_matrix);

	cout << "--- ОТВЕТ ---" << endl;
	cout << "Вектор X" << endl;
	printVector(vector_X);

	return 0;
}