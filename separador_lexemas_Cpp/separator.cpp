#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
int main(int argc, char *argv[]){
	ifstream arq;
	string lexema;
	arq.open("Texto.txt");
	while(!arq.eof()){
		arq>> lexema;
		cout<< lexema <<" : ";
	}
	cout<<endl;
	system("PAUSE");
	return EXIT_SUCCESS;
}
