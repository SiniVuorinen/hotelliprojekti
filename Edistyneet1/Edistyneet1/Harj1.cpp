//Osoitinmuuttujaharjoitus

#include<iostream>
using namespace std;

int main()
{
	int x = 25; // int variable
	int *ptr = nullprt; // pointer variable

	ptr = &x; // Store the address of x in ptr

	cout << "X:n arvo on: " << x << endl;
	cout << "X:n osoite on: " << ptr << endl;

	return 0;
}