#include <iostream>
#include <fstream>
#include "robione.h"

using namespace std;

int main()
{
	try{
		cout << endl;
		cout<<"stworzenie i wyswietlenie 2 macierzy"<<endl;
	  	CMatrix A1(2,5,1.0);			//Create matrix A1 = [	1.0  0.0  0.0  0.0  0.0
							//			0.0  1.0  0.0  0.0  0.0  ]
		cout << A1 << endl;
		
		CMatrix A2(5,2,0.0,6.3);		//Create matrix A1 = [	0.0  6.3  6.3 
							//			6.3  0.0  6.3 
							//			6.3  6.3  0.0
							//			6.3  6.3  6.3
							//			6.3  6.3  6.3  ]
		cout << A2 << endl;
		cout<<"stworzenie macierzy, przypisanie do niej wyniku mnozenia i wyswietlenie jej"<<endl;
		CMatrix S;
		S = A1 * A2;			//Multiply A1 by A2
		cout << S << endl;
		cout<<"Odczyt macierzy z pliku i wyswietlenie jej"<<endl;
		fstream f1;
		f1.open("plik.txt", fstream::in);
		CMatrix B(f1);				//Read the matrix data from file matrix.dat
		f1.close();				//First two values in this file specify the matrix dimensions
		cout << B << endl;
		cout<<"zabawa komorkami macierzy"<<endl;
		S = B;						//Assign B to S
		S[0][0] = 12;					//Modify S
		cout << "S[0][0]=" << S[0][0] << endl;		//Verify S
		cout << "B[0][0]=" << B[0][0] << endl;		//Verify B
}
catch(bad_alloc)
	{
		cout << "coz z pamiecia nie tak" << endl;
}
catch(CMatrix::WrongDim)
	{
		cout << "Nie moge przemnozyc tych macierzy" << endl;
}
catch(CMatrix::BadIndex)
	{
		cout << "Macierz nie ma takiego indeksu" << endl;
}
}
//A moze by tak dodac odejmowanie macierzy
//Tak to dobry pomysl
