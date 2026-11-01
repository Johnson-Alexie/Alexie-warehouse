#include <iostream>
using namespace std;

class MyClass {
public:
    void fun(int arr[], int n) 
    {
        if (n <= 1) return;

        int minIndex = 0;
        for (int i = 1; i < n; i++) {
            if (arr[i] < arr[minIndex]) {
                minIndex = i;
            }
        }

        int temp = arr[minIndex];
        arr[minIndex] = arr[n - 1];
        arr[n - 1] = temp;
    }

    void print(int arr[], int n) 
    {
        for (int i = 0; i < n; i++) {
            cout << arr[i];
            if (i < n - 1) cout << ", ";
        }
        cout << endl;
    }
};

int main() 
{
    int n;
    cin >> n;
    int arr[1000];
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    MyClass obj;
    obj.fun(arr, n);
    obj.print(arr, n);
    return 0;
}
