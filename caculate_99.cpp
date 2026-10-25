#include <iostream>

using namespace std;

int main()

{

	for (int b = 1; b < 10; b++)

	{

		for (int a = 1; a < b+1; a++)

		{

	cout << a << "x" << b << "=" << a * b << "\t";

		

		}

		cout << endl;

	}

	return 0;

}
