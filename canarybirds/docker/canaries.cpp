// c++ canaries.cpp -o canary -fstack-protector
#include <iostream>
#include <unistd.h>
#include <fstream>
using namespace std;


void getflag() {
    fstream my_file;
    char ch;
    my_file.open("./flag", ios::in);
    if (!my_file) {
        cout << "Error opening flag, run on remote\n";
        return;
    }
    while (1) {
		my_file >> ch;
		if (my_file.eof())
			break;

		cout << ch;
	}
}

int main() {
    int x;
    char buf[32];
    char msg[256];
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    cout << "Welcome user\nHow long is your name?\n";
    cin >> x;
    cout << "What is your name?\n";
    read(0, buf, sizeof(buf));
    cout << "You entered: ";
    write(1, buf, x);
    cout << "How long is your message?\n";
    cin >> x;
    cout << "What is your message?\n";
    read(0, buf, x);
    return 0;
}
