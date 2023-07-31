#include <iostream>
#include <Windows.h>

using namespace std;

int main(){
	cout << "Start" << endl;
	for (int i=0; i<50; i++){
		cout << "-";
		fflush(stdout);
		Sleep(500);
	}
	cout << "100%" << endl;
	cout << "Stop" << endl;
	return 0;
}
