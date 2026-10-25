#include <iostream>
#include <ctime>
using namespace std;

/*                                  //if 案例 2016.10.24
int main()
{
    int a = 0;
    int b = 0;
    int c = 0;
    cin >> a;
    cin >> b;
    cin >> c;
    if (a > b)
    {
        if (a > c)
        {
            cout << "Ada" << endl;
        }
        else
        {
            cout << "Cda" << endl;
        }
    }
    else if (a < b)
    {
        if (b > c)
        {
            cout << "Bda" << endl;
        }
        else
        {
            cout << "Cda" << endl;
        }
    }
    else
    {
        if (c > a)
        {
            cout << "Cda" << endl;
        }
    }

    return 0;
}
*/

/*                                   //猜数字 案例
int main()
{
srand((unsigned int)time(NULL)); //引入随机数
int a = rand() %100 + 1;
int b = 0;
while (1)
{
cin >> b;
    if (a > b)
    {
        cout << "小了" << endl;
    }
    else if (a < b)
    {
        cout << "大了" << endl;
    }
    else
    {
        cout << "对了" << endl;
        break;
    }
}
return 0;
}
*/

/*                                        //水仙花数
int main()
{
int a = 100;
int b = 0;
int c = 0;
int d = 0;

while (a < 1000)
{
    b = a%10;
    c = a/10%10;
    d = a/100;
    if (a == b*b*b + c*c*c + d*d*d)
    {
        cout << a << endl;

    }
a += 1;
}
return 0;    
}
*/

/*                                         //乘法口诀表for
int main()
{
for (int a = 1;a < 10;a++)
{
    for(int b = 1;b <= a;b++)
    {
        cout << a << "x" << b << "=" << a*b << " ";
    }
cout << endl;
}
return 0;
}
*/

/*                                        //乘法口诀表while
int main()                                 
{
int a = 1;
while(a < 10)
{
    int b = 1;
    while(b <= a)
    {
        cout << a << "x" << b << "=" << a*b << " ";
        b++;
    }
a++;
cout << endl;
}
return 0;
}
*/

/*                                          //元素逆置
int main()
{
int arr[6] = {1,2,3,4,5,6};
int start = 0;
int end = sizeof(arr) / sizeof(arr[0]) -1;

// int temp = arr[start];   //元素替换
// arr[start] = arr[end];
// arr[end] = temp;

while(start < end)
{
int temp = arr[start];   //元素替换
arr[start] = arr[end];
arr[end] = temp;

start++;
end--;
}

for(int a = 0;a < 6;a++)
{
     cout << arr[a] << endl;
}

return 0;
}
*/

/*                                        //冒泡排序
int main()
{
int temp;
int arr[6] = {5,8,1,3,9,2};


for(int num = 0;num < sizeof(arr) / sizeof(arr[0]) -1;num++)       //-1 优化运行速度
{
    for(int a = 0;a < sizeof(arr) / sizeof(arr[0]) -num -1;a++)    //-num 优化运行速度 -1防止越界
    {
        if(arr[a] < arr[a + 1])
        {
            temp = arr[a];
            arr[a] = arr[a + 1];
            arr[a + 1] = temp;
        }
    }
}
for(int b = 0;b < 6;b++)
{
     cout << arr[b] << endl;
}


return 0;
}
*/

/*                                          //函数 案例
int swap(int num1,int num2);


int main()
{
int a = 10;
int b = 20;

cout << swap(a,b) << endl;
}


int swap(int num1,int num2)
{
    int temp = num1;
    num1 = num2;
    num2 = temp;
    return(num1);
}
*/

/*                                        //指针 函数 数组 案例 2026.10.25
void bubble(int * arr,int len)
{
for(int i = 0;i < len - 1;i++)
{
    for(int j = 0;j < len - i -1;j++)
    {
        if(arr[j]  > arr[j +1])
        {
            int temp = arr[j];
            arr[j] = arr[j + 1];
            arr[j + 1] = temp;
        }
    }
}
}

void print(int * arr,int len)
{
for(int v = 0;v < len ;v++)
{
    cout << arr[v]<< endl;
}
}


int main()
{
int arr[10] = {4,3,6,9,1,2,10,8,7,5};
int len = sizeof(arr) / sizeof(arr[0]);

bubble(arr,len);
print(arr,len);

return 0;
}
*/

















