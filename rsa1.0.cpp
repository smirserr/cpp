#include <iostream>
#include <windows.h>
#include <ctime>
#include <cmath>
using namespace std;




int nod(int a, int b){
	while(a > 0 && b > 0)
		if(a > b) 
			a %= b;
		else 
			b %= a;
	return a + b;
}


int Kc_gen(int a, int b, int m){
	int x = m;
	int y = a;
	int q,
		p1 = 0,
	    p2 = 1,
		r  = 1;
	int n;
	for(n = 0; x % y != 0; n++){
		q = x / y;
		r = x % y;
		x = y;
		y = r;
	//	cout << "q[" << n << "] = " << q << endl;  
		int temp = p2;
		p2 = p2 * q + p1;
	//	cout << "p[" << n << "] = " << p2 << endl; 
		p1 = temp;
	}
	
	int Kc = b * p2;
	if (n % 2 == 1) Kc = -Kc;
	while ( Kc <= 0) Kc += m;
	return Kc;
}


void key_gen(int& Ko, int& N, int& Kc){
	int  P, Q, m;
	cout << "¬ведите P и Q: ";
	cin >> P >> Q;
	N = P*Q;
	m = (P-1)*(Q-1);
	
	cout << "¬ведите открытый ключ (0 - сгенерировать): ";
	cin >> Ko;
	if (Ko == 0){
		srand(time(0));
		Ko = 2 + rand() % (m-1);
		while(nod(Ko, m ) != 1) Ko = 2 + rand() % (m-1);
	}
	Kc = Kc_gen(Ko,1,m);

}





int main(){
	SetConsoleCP(1251);
 	SetConsoleOutputCP(1251);
 	setlocale (LC_ALL, "rus");
	
	int menu;
	cout << "0 - выйти" << endl;
	cout << "1 - генерировать ключи" << endl;
	cout << "2 - закодировать" << endl;
	cout << "3 - раскодировать" << endl;
	cin >> menu;
	
	string s;
	int  C,
		 M;
	
	int Ko,Kc,N = 0;
	while (menu != 0){
		if(menu == 1) {
			key_gen(Ko, N, Kc);	
			cout << "Ko = " << Ko  << endl;
			cout << "N  = " << N << endl;
			cout << "Kc = " << Kc << endl;
		}
		if(menu == 2){
			if(N == 0){
				cout << "¬ведите  ключи Ko и N: ";
				cin >> Ko >> N;
			}
			cout << "¬ведите сообщение: ";
			cin >> s;
			for(int i = 0; i <= s.length(); i++){
				M = (int)s[i];
				C = M;
				for(int j = 2; j <= Ko; j++)
									C = C*M % N;
				cout << C << "  ";
			}
			cout << endl;
		}
		if(menu == 3) {
			if (N == 0){
				cout << "¬ведите  ключи Kс и N: ";
				cin >> Kc >> N;
			}
			cout << "¬ведите шифрограмму: " << endl;
			for(int i = 0;;i++){
				cin >> C ;
				M = C;
				for(int j = 2; j <= Kc; j++)
								M = M*C % N;
				s[i] = (char)M;
				if(s[i] == '\0') break;
			}
			cout << s <<endl;
		}
		cin >> menu;
	}
	
}


