#include <bits/stdc++.h>
using namespace std;

int main(){ 
	int x=85; 
	string message="Hello World"; 
	x-=6; 
	if (x>50){ 
		if (x<80){ 
			printf("Beshi boro na\n"); 
		} 
		else { 
			printf("Valoi boro\n"); 
		} 
	} 
	else if (x==49){ 
		printf("x holo 49\n"); 
	} 
	else { 
		printf("Number is not greater than 50\n"); 
	} 
	for (int i=0; 
	i<10; 
	i++){ 
		printf("loop: %d\n",i+1); 
	} 
	return 0; 
} 