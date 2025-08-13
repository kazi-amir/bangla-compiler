#include <bits/stdc++.h>
using namespace std;

int main(){ 
	int x=85; 
	string message="Hello World"; 
	x-=6; 
	if (x>50){ 
		if (x<80){ 
			cout << "Beshi boro na\n"; 
		} 
		else { 
			cout << "Valoi boro\n"; 
		} 
	} 
	else if (x==49){ 
		cout << "x holo 49\n"; 
	} 
	else { 
		cout << "Number is not greater than 50\n"; 
	} 
	for (int i=0; 
	i<10; 
	i++){ 
		cout << "loop: "; 
		cout << i+1; 
		cout << "\n"; 
	} 
	return 0; 
} 