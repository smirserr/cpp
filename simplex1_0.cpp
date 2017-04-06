#include <iostream>
#include <fstream>
#include <windows.h>
#include <ctime>
#include <iomanip>
#include <cmath>
using namespace std;



double round(double value, double n){
	return round(value*n)/n;
}

// Печать матрицы
void print(double *a, int n, int m){
	cout << "==================================================================" << endl;
	for(int i = 0; i < n; i++){
		for (int j = 0; j < m; j++)
			cout << setw(8)<< round(*(a + i*m + j), 100) << "";	
		cout << endl;
	}
	cout << "==================================================================";
	cout << endl;
}


// проверка оптимальности плана
bool optima(double *table, int n, int m, double task_type){
	for(int j = 1; j < m; j++){
		if ( round(*(table + (n-2)*m + j),10000)* task_type < 0) return 0;
		if (( round(*(table + (n-2)*m + j),10000)  == 0) &&  (round(*(table + (n-1)*m + j),10000)* task_type < 0)) return 0;
	}
	return 1;
}


// Расчет и внесение в таблицу оцкнок
void estimates_in_tables(double *table, int basis[], double C[], double M[], int n, int m){
	for(int j = 0; j < m; j++){
	 		double sumC = 0;
			double sumM = 0;	
			for (int i = 0; i < n-2; i++){
				sumC += (*(table + i*m + j))*C[basis[i]];
				sumM += (*(table + i*m + j))*M[basis[i]]; 
			}
			if(j >0){
				sumC -= C[j-1];
				sumM -= M[j-1]; 
			}
			(*(table + (n-2)*m + j)) = sumM;
			(*(table + (n-1)*m + j)) = sumC;
		}
}



int main(){
	SetConsoleCP(1251);
 	SetConsoleOutputCP(1251);
 	setlocale (LC_ALL, "rus");
	
////////////////////////////// 	ВВОД УСЛОВИЯ ИЗ ФАЙЛА + ПЕЧАТЬ //////////////////////////////////
 	int num_var,  // кол-во переменных
 		num_lim;  //  кол-во условий 

	ifstream fin("input.txt"); // файл с заданием
	fin >> num_var >> num_lim;
	
	int tn = num_lim + 1, // кол-во строк в матрице условия
		tm = num_var + 2; // кол-во столбцов в матрице условия
	
	double task[tn][tm]; // матрица условия
	
	cout << " Загрузка условия из файла input.txt " << endl;
	cout << "##############################################" << endl;
	cout << "Формат: " << endl;
	cout << " <количество переменных>  <количество условий>" << endl;
	cout << " [коэффициенты при функции] > [1, ищем max | -1, ищем min]" << endl;
	cout << " [коэффициенты при условии] [>, <, =]  <C>" << endl;
	cout << " ..................................... " << endl;
	cout << " [коэффициенты при условии] [>, <, =]  <C> " << endl;
	cout << "##############################################" << endl << endl;
	
	
	for(int i = 0; i < tn; i++) {     // ввод условия
			for(int j = 0; j < tm-2; j++)  fin >> task[i][j];
			char tmp;
			fin >> tmp;
			if (tmp == '<') task[i][tm-2] = -1;
			if (tmp == '>') task[i][tm-2] = 1;
			if (tmp == '=') task[i][tm-2] = 0;
			fin >> task[i][tm-1];
	}
	fin.close();

	
	
	double task_type = task[0][tm-1];
	// вывод условия
	for(int j = 0; j < tm-2; j++){
		if (j > 0) cout << " + ";
		cout << task[0][j] << "X" << j+1;
	}
	cout << " -> ";
	if (task[0][tm-1] == 1) cout << "max" << endl;
	if (task[0][tm-1] == -1) cout << "min" << endl;
	for(int i = 1; i < tn; i++){   
		cout << "{ ";  
		for(int j = 0; j < tm-2; j++){
			if (j > 0) cout << " + ";
			cout << task[i][j] << "X" << j+1;	
		}
		if(task[i][tm-2] == -1) cout << " <= ";
		if(task[i][tm-2] == 1) cout << " >= ";
		if(task[i][tm-2] == 0) cout << " = ";
		cout << task[i][tm-1] << endl;
	}
	cout << endl;
	system("pause");
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////ЗАПОЛНКЕНИЕ ТАБДИЦЫ/////////////////////////////////////////////////////////

	int add_var = 0; // кол-во дополнительных переменных
	for (int i = 1; i < tn; i++){
		if (task[i][tm-2] == 1) add_var+=2;
		else add_var++;
	}
	
	int n = num_lim + 2,  // кол-во строк симпликс таблицы
		m = tm + add_var -1;  // кол-во столбцов симпликс таблицы
	double table[n][m]; // симпликс таблица
	
	for (int i = 0; i < n; i++)  // заполнение нулями
		for(int j = 0; j < m; j++) table[i][j] = 0;
	
	
	for(int i = 0; i < tn-1; i++) table[i][0] = task[i+1][tm-1];

	for (int i = 1; i < tn; i++)  
		for(int j = 0; j < tm-2; j++) table[i-1][j+1] = task[i][j];
	
	
	double C[m-1],   // Коэффициенты при x
		   M[m-1];   // Коэффициент при MX
	
	for(int i = 0; i < m-1; i++){
		C[i] = 0;
		M[i] = 0;
	}
	
	// Задание коэффициентов при x
	for(int i = 0; i < num_var; i++) C[i] = task[0][i];
	
	// Генерация  первого плана и задание коэффициентов при MX
	for(int i = 1, j = tm-1; i < n-1; i++){
		if(task[i][tm-2] == 1){
			table[i-1][j++] = -1;
			table[i-1][j++] = 1;
			M[j-2] = -1* task_type;
		}
		if (task[i][tm-2] == 0){
			M[j-1] = -1 * task_type;
			table[i-1][j++] = 1;
		} 
		if (task[i][tm-2] == -1) table[i-1][j++] = 1;
	}
	
	


///////////////////////////////////////////////////////////////////////////////////////////////////////	
 	
 	
 	int basis[num_lim];  // текуший базис
 	
 	for(int j = 1, k = 0; j < m; j ++){ // поиск перменных для первого базиса
 		double sum = 0;
		int quan = 0;
		for(int i = 0; i < n-2; i++){
			sum += table[i][j];
			if(table[i][j] != 0) quan ++;
		}	
		if (sum == 1 && quan == 1) basis[k++] = j-1;
	}
	
	cout << "basis: ";
	for(int i = 0; i < num_lim; i++){
		cout << "X" << basis[i]+1 << " ";
	}
	cout << endl;
 	
 	
 	
 	estimates_in_tables((double *)table, basis,C ,M, n, m);    // Расчет и внесение в таблицу оцкнок
 	
 	print((double *)table, n, m);
	system("pause");
 	
 	
 	while(! optima((double *)table, n, m, task_type) ){     // пока план не оптимален
	
	
		int in_base;     // номер ведушего столбца 
		int out_base;    // номер ведушей строки 
 		
 		in_base = 1;  // поиск ведушего столбца 
 		for(int i = 2; i < m; i++) {
			if(table[n-2][i]* task_type < table[n-2][in_base]* task_type) in_base = i;
			if(table[n-2][i] == table[n-2][in_base] && table[n-1][i]* task_type < table[n-1][in_base]* task_type) in_base = i;
		}
 		
		 	
		double Q[n-2]; // для определения ведущей строки 
		for(int i = 0; i < n-2; i++){
			if(table[i][in_base] <= 0){
				Q[i] = -1;
				continue;
			}
			Q[i] =  table[i][0] / table[i][in_base] ;   
		}
 			
		// поиск ведушей строки (поск минимального неотрицательного элеиента в Q)		
 		out_base = -1;
		for(int i = 0; i < n-2; i++){
			if (Q[i] >= 0) {
				if(out_base == -1){
					out_base = i;
					continue;
				}
				if(Q[i] < Q[out_base])
				{
					out_base = i;			
					continue;
				}
				if(Q[i] == Q[out_base]) {
                   // Метод Креко? есть что доделать!
				}

			}	

		}
		
		double r = table[out_base][in_base]; // опроный элемент
		
		/////////ВЫОД ОТЛАДОЧНОЙ ИНФОРМАЦИИ
		cout << "Q: ";
		for(int i = 0; i < n-2; i++) cout << Q[i] << " " ;
		cout << endl;
		cout << "min Q = "<< Q[out_base] << endl;
		cout << "in base = " << in_base <<  endl;
		cout << "out base = " <<out_base << " (X" << basis[out_base]+1 << ") " << endl  ;
		cout << "Опорный элемент: " << r << endl;
		///////////////////////////////////
		
		
		//  Пересчет симплекс-таблицы
		double  m_[n-2][m]; // матрица - копия table для перерасчета
		for(int i = 0; i < n-2; i++)
			for(int j = 0; j < m; j++) m_[i][j] = table[i][j];
		
		for(int i = 0; i < n-2; i++){
			for(int j = 0; j < m; j++){
				if (i == out_base) table[i][j] = m_[i][j] / r;
				else table[i][j] = m_[i][j] - m_[i][in_base]*m_[out_base][j] / r;
			}
		}
		
		basis[out_base] = in_base-1; // обновление базиса
		
		estimates_in_tables((double *)table, basis,C ,M, n, m);   // Расчет и внесение в таблицу оцкнок
		
		print((double *)table, n, m);
	//	system("pause");
	}
 	
 	
 	// ВЫВОД РЕЗУЛЬТАТОВ
 	cout << "++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
	for(int i = 0; i < n-2; i++)	
			if(basis[i] < num_var) {
				cout << "X" << basis[i]+1 << " " << table[i][0] << endl; 
			}
	cout << "++++++++++++++++++++++++++++++++++++++++++++++++";

	
 	
	return 0; 	
 	
}
