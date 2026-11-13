#include <iostream>
#include <string>
using namespace std;


#define MAX 3
struct person
{
    string p_name;
    string p_sex;
    int p_age;
    int p_num;

};

struct addressbook
{
    int size = 0;
    person p_book[MAX];
};

void printmanu()                                //打印菜单
{
    cout << "***********************" << endl;
    cout << "****  1.添加联系人  ****" << endl;
    cout << "****  2.显示联系人  ****" << endl;
    cout << "****  3.删除联系人  ****" << endl;
    cout << "****  4.查找联系人  ****" << endl;
    cout << "****  5.修改联系人  ****" << endl;
    cout << "****  6.清空联系人  ****" << endl;
    cout << "****  0.退出通讯录  ****" << endl;
    cout << "***********************" << endl;
}

void addperson(addressbook* ads)                //添加联系人
{
    if(ads->size >= MAX)
    {
        system("clear");
        cout << "人数已满" << endl;
    }
    else
    {
        string name;
        cout << "请输入姓名" << endl;
        cin >> name;
        ads->p_book[ads->size].p_name = name;

        string sex;
        cout << "请输入性别" << endl;
        cin >> sex;
        ads->p_book[ads->size].p_sex = sex;

        int age;
        cout << "请输入年龄" << endl;
        cin >> age;
        ads->p_book[ads->size].p_age = age;

        int num;
        cout << "请输入电话" << endl;
        cin >> num;
        ads->p_book[ads->size].p_num = num;

        ads->size++;
        system("clear");
        cout <<  "添加成功" << endl;
        
    }
}

void showperson(addressbook* ads)               //显示联系人
{
    system("clear");
    if(ads->size == 0)
    {
        system("clear");
        cout << "无联系人" << endl;
    }
    else
    {
        for(int i = 0;i < ads->size;i++)
        {
        cout << "联系人:" << i + 1 << "\t";
        cout << "姓名:" <<ads->p_book[i].p_name << "\t";
        cout << "性别:" << ads->p_book[i].p_sex << "\t";
        cout << "年龄:" <<ads->p_book[i].p_age << "\t";
        cout << "电话号码:" << ads->p_book[i].p_num << endl;
        }
    }
}

int isExist(addressbook* ads,string name)       //查看有无联系人
{
    for(int i = 0;i < ads->size;i++)
    {
        if(ads->p_book[i].p_name == name)
        {
            return i ;
        }
    }
    return -1;
}

void deleteperson(addressbook *ads,string name)     //删除联系人
{
    cout << "输入删除联系人名字" << endl;
    cin >> name;
    int ret = isExist(ads,name);

    if(ret != -1)
    {
        for(int i = ret;i < ads->size - 1;i++)  //-1防止超界
        {
        ads->p_book[i] = ads->p_book[i + 1];
        }
        ads->size--;
        system("clear");
        cout << "删除成功" << endl;
    }
    else
    {
        system("clear");
        cout << "查无此人" << endl;
    }
    
}
    
void findperson(addressbook * ads,string name)      //查找联系人
{
    cout << "输入查找联系人姓名" << endl;
    cin >> name;

    int ret = isExist(ads,name);

    if(ret != -1)
    {
        system("clear");
        cout << "姓名:" <<ads->p_book[ret].p_name << "\t";
        cout << "性别:" << ads->p_book[ret].p_sex << "\t";
        cout << "年龄:" <<ads->p_book[ret].p_age << "\t";
        cout << "电话号码:" << ads->p_book[ret].p_num << endl;
    }
    else
    {
        system("clear");
        cout << "查无此人" << endl;
    }
}

void modifyperson(addressbook * ads)            //修改联系人
{
    cout << "修改联系人姓名" << endl;
    string name;
    cin >> name;

    int ret = isExist(ads,name);

    if(ret != -1)
    {
        cout << "请输入姓名" << endl;
        cin >> name;
        ads->p_book[ret].p_name = name;

        cout << "请输入性别" << endl;
        string sex;
        cin >> sex;
        ads->p_book[ret].p_sex = sex;

        cout << "请输入年龄" << endl;
        int age = 0;
        cin >> age;
        ads->p_book[ret].p_age = age;

        cout << "请输入电话号码" << endl;
        int num = 0;
        cin >> num;
        ads->p_book[ret].p_num = num;
        system("clear");
        cout << "修改成功" << endl;
    }
    else
    {
        system("clear");
        cout << "查无此人" << endl;
    }
}

void clearperson(addressbook * ads)         //清空通讯录
{
    cout << "确定清空联系人吗:确定按1,不确定按0" << endl;
    int a = 0;
    cin >> a;
    if(a == 1)
    {
        ads->size = 0;
    system("clear");
    cout << "通讯录已清空"<< endl;
    }
    else if (a == 0)
    {       
        system("clear");
    }
    else
    {
        system("clear");
        cout << "无效数字" << endl;
    }
}
    


int main()
{
    int select = 0;
    addressbook ads;
    string name;
while(true)
{
    printmanu();

    cin >> select;
    switch(select)
    {
    case 1:
        addperson(&ads);
        break;
    case 2:
        showperson(&ads);
        break;
    case 3:
        deleteperson(&ads,name);
        break;
    case 4:
        findperson(&ads,name);
        break;
    case 5:
        modifyperson(&ads);
        break;
    case 6:
        clearperson(&ads);
        break;
    case 0:
        cout << "欢迎下次使用" << endl;
        return 0;
    }
}

return 0;
}