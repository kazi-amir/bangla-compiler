#include <iostream>
#include <string>

using namespace std;

int main() {
    int amarNumber = 40;
    string message = "Hello World";
    if (amarNumber > 50) {
        if (amarNumber < 80) {
            cout << "Beshi boro na" << endl;
        } else {
            cout << "Valoi boro" << endl;
        }
    } else {
        cout << "Number is not greater than 50" << endl;
    }
    return 0;
}
