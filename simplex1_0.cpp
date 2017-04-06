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

// ������ �������
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


// �������� ������������� �����
bool optima(double *table, int n, int m, double task_type){
	for(int j = 1; j < m; j++){
		if ( round(*(table + (n-2)*m + j),10000)* task_type < 0) return 0;
		if (( round(*(table + (n-2)*m + j),10000)  == 0) &&  (round(*(table + (n-1)*m + j),10000)* task_type < 0)) return 0;
	}
	return 1;
}


// ������ � �������� � ������� ������
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
	
////////////////////////////// 	���� ������� �� ����� + ������ //////////////////////////////////
 	int num_var,  // ���-�� ����������
 		num_lim;  //  ���-�� ������� 

	ifstream fin("input.txt"); // ���� � ��������
	fin >> num_var >> num_lim;
	
	int tn = num_lim + 1, // ���-�� ����� � ������� �������
		tm = num_var + 2; // ���-�� �������� � ������� �������
	
	double task[tn][tm]; // ������� �������
	
	cout << " �������� ������� �� ����� input.txt " << endl;
	cout << "##############################################" << endl;
	cout << "������: " << endl;
	cout << " <���������� ����������>  <���������� �������>" << endl;
	cout << " [������������ ��� �������] > [1, ���� max | -1, ���� min]" << endl;
	cout << " [������������ ��� �������] [>, <, =]  <C>" << endl;
	cout << " ..................................... " << endl;
	cout << " [������������ ��� �������] [>, <, =]  <C> " << endl;
	cout << "##############################################" << endl << endl;
	
	
	for(int i = 0; i < tn; i++) {     // ���� �������
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
	// ����� �������
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

////////////////////////����������� �������/////////////////////////////////////////////////////////

	int add_var = 0; // ���-�� �������������� ����������
	for (int i = 1; i < tn; i++){
		if (task[i][tm-2] == 1) add_var+=2;
		else add_var++;
	}
	
	int n = num_lim + 2,  // ���-�� ����� �������� �������
		m = tm + add_var -1;  // ���-�� �������� �������� �������
	double table[n][m]; // �������� �������
	
	for (int i = 0; i < n; i++)  // ���������� ������
		for(int j = 0; j < m; j++) table[i][j] = 0;
	
	
	for(int i = 0; i < tn-1; i++) table[i][0] = task[i+1][tm-1];

	for (int i = 1; i < tn; i++)  
		for(int j = 0; j < tm-2; j++) table[i-1][j+1] = task[i][j];
	
	
	double C[m-1],   // ������������ ��� x
		   M[m-1];   // ����������� ��� MX
	
	for(int i = 0; i < m-1; i++){
		C[i] = 0;
		M[i] = 0;
	}
	
	// ������� ������������� ��� x
	for(int i = 0; i < num_var; i++) C[i] = task[0][i];
	
	// ���������  ������� ����� � ������� ������������� ��� MX
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
 	
 	
 	int basis[num_lim];  // ������� �����
 	
 	for(int j = 1, k = 0; j < m; j ++){ // ����� ��������� ��� ������� ������
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
 	
 	
 	
 	estimates_in_tables((double *)table, basis,C ,M, n, m);    // ������ � �������� � ������� ������
 	
 	print((double *)table, n, m);
	system("pause");
 	
 	
 	while(! optima((double *)table, n, m, task_type) ){     // ���� ���� �� ���������
	
	
		int in_base;     // ����� �������� ������� 
		int out_base;    // ����� ������� ������ 
 		
 		in_base = 1;  // ����� �������� ������� 
 		for(int i = 2; i < m; i++) {
			if(table[n-2][i]* task_type < table[n-2][in_base]* task_type) in_base = i;
			if(table[n-2][i] == table[n-2][in_base] && table[n-1][i]* task_type < table[n-1][in_base]* task_type) in_base = i;
		}
 		
		 	
		double Q[n-2]; // ��� ����������� ������� ������ 
		for(int i = 0; i < n-2; i++){
			if(table[i][in_base] <= 0){
				Q[i] = -1;
				continue;
			}
			Q[i] =  table[i][0] / table[i][in_base] ;   
		}
 			
		// ����� ������� ������ (���� ������������ ���������������� �������� � Q)		
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
                   // ����� �����? ���� ��� ��������!
				}

			}	

		}
		
		double r = table[out_base][in_base]; // ������� �������
		
		/////////���� ���������� ����������
		cout << "Q: ";
		for(int i = 0; i < n-2; i++) cout << Q[i] << " " ;
		cout << endl;
		cout << "min Q = "<< Q[out_base] << endl;
		cout << "in base = " << in_base <<  endl;
		cout << "out base = " <<out_base << " (X" << basis[out_base]+1 << ") " << endl  ;
		cout << "������� �������: " << r << endl;
		///////////////////////////////////
		
		
		//  �������� ��������-�������
		double  m_[n-2][m]; // ������� - ����� table ��� �����������
		for(int i = 0; i < n-2; i++)
			for(int j = 0; j < m; j++) m_[i][j] = table[i][j];
		
		for(int i = 0; i < n-2; i++){
			for(int j = 0; j < m; j++){
				if (i == out_base) table[i][j] = m_[i][j] / r;
				else table[i][j] = m_[i][j] - m_[i][in_base]*m_[out_base][j] / r;
			}
		}
		
		basis[out_base] = in_base-1; // ���������� ������
		
		estimates_in_tables((double *)table, basis,C ,M, n, m);   // ������ � �������� � ������� ������
		
		print((double *)table, n, m);
	//	system("pause");
	}
 	
 	
 	// ����� �����������
 	cout << "++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
	for(int i = 0; i < n-2; i++)	
			if(basis[i] < num_var) {
				cout << "X" << basis[i]+1 << " " << table[i][0] << endl; 
			}
	cout << "++++++++++++++++++++++++++++++++++++++++++++++++";

	
 	
	return 0; 	
 	
}
