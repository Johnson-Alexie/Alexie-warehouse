#include <iostream>
using namespace std;

int main()
{
    int b = 1;
    while (b < 10)
    {
        int a = 1;
        while (a < b + 1)
        {
            cout << a << "x" << b << "=" << a * b << "\t";
            a++;                       
        }
        cout << endl;
        b++;
    }
    return 0;
}
