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

int main()
{
	setlocale(LC_ALL, "Rus");

	vector <vector <double> > matrix;
	vector <vector <double> > LU_matrix;

	if (readFile(matrix))
	{
		return -1;
	}

	LU_matrix = LU_decomp(matrix);

	return 0;
}