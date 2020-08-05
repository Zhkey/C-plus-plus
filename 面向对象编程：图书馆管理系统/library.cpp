#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <cctype>
#include <iomanip>
#include <conio.h>
#include <windows.h>
using namespace std;
struct student
{
    int id;//读者编号
    string name;//读者姓名
    int borrowsum;//你已借阅多少本书,默认为0
    int number;//现在还有多少本书未还，默认为0
    string borrowday;//上次借阅时间，默认为0000.00.00
    int b[10];//你所借书的的编号，最多10本
};
struct book
{
    int idnum;//图书检索号
    int BorrowCount;//图书借阅量,初始化为0
    string name;//书名
    string kind;//图书种类
    int sum;//图书总库存存量
    int nowsum;//图书现库存量
    string author;//图书作者
    bool ok;//是否可借,初始为可以
    string borrowdate;//图书最近一次借出时间，默认为0000-00-00；
    string returndate;//图书最近一次归还时间，默认为0000-00-00；
};
bool cmpByidnum(book a,book b)//布尔型变量，返回YES和NO值 
{
    return a.idnum<b.idnum;
}
bool cmpByCount(book a,book b)
{
    return a.BorrowCount>b.BorrowCount;
}
bool cmpByBorrowsum(student a,student b)
{
    return a.borrowsum>b.borrowsum;
}
bool cmpByid(student a,student b)
{
    return a.id<b.id;
}
class Library
{
private:
    int borrownum;//每月借出书数量
    int returnnum;//每月还书数量
    vector<book> data;//建立book动态数组 
    vector<student> data1;//建立student动态数组
public:
    Library();
    void AddBook(book NewBook);  //增加图书
    void DeleteBook(string bookname,string author);//删除图书
    void BorrowBook(string name,string author);//借阅图书
    void BackBook(string name,string author,int k);//归还图书
    void ShowAllBook(); //输出系统所有图书
    void  SearchBookPosWithname(string thebook); //按书名查询
    void  SearchBookPosWithAuthor(string theauthor);//按作者查询
    void  SearchBookPosWithKind(string kind);//按种类查询
    int  SearchBookPosWithAB(string theauthor,string thebook);//按作者和书名查询
    int  SearchBookPosWithid(int id); //按检索号搜寻
    void SortBook(int ca);  //排序
    void SortStudent(int ca);//按读者借阅量排序
    void Save();  //存入图书馆文件
    void Save1();//存入学生文件
    void printbook(book a);//输出某本书的所有信息
    void revisebook(string name,string author);//修改某本书的信息
    int SerchStudent(int id);//查询某个读者
    void AddStudent(student a);//增加一个读者
    void PrintStudent(int kid);//输出读者信息
    int GetStudent();//返回读者总数
    int readsum(int id);//获取某个读者的借阅量
    int getborrownum();//获取本月外借量
    int getreturnnum();//获取本月还书量
};
Library::Library()
{
    borrownum=0;
    returnnum=0;
    int AllBook,AllStudent;
    ifstream fin("book.txt");
    if(fin)
    {
        fin>>AllBook;
        for(int i=0; i<AllBook; i++)
        {
            book tem;
            fin>>tem.idnum>>tem.name>>tem.author>>tem.kind>>tem.sum>>tem.nowsum>>tem.BorrowCount>>tem.ok>>tem.borrowdate>>tem.returndate;
            data.push_back(tem);//将book“tem”压入vector    data中 
        }
        fin.close();
    }
 
    ifstream tfin("student.txt");
    if(tfin)
    {
        tfin>>AllStudent;
        for(int i=0; i<AllStudent; i++)
        {
            student tem;
            tfin>>tem.id>>tem.name>>tem.borrowsum>>tem.number>>tem.borrowday;
            for(int j=0;j<10;j++)
            {
                tfin>>tem.b[j];
            }
            data1.push_back(tem);//将元素tem压入data1容器
        }
        tfin.close();
    }
}
int Library::readsum(int a)
{
    return data1[a-1].borrowsum;
}
int Library::getborrownum()//获取本月外借量
{
    return borrownum;
}
int Library::getreturnnum()//获取本月还书量
{
    return returnnum;
}
int Library::GetStudent()
{
    int k=(int)data1.size();
    return k+1;
}
void Library::AddBook(book NewBook)
{
    data.push_back(NewBook);
}
void Library::AddStudent(student newstudent)
{
    data1.push_back(newstudent);
}
void Library::DeleteBook(string bookname,string author)
{
    int pos = SearchBookPosWithAB(author,bookname);
    if (pos!=-1)
    {
        data.erase(data.begin()+pos);
        return ;
    }
    else
        cout<<"查无此书！\n";
}
void Library::BorrowBook(string name,string author)
{
    string BorrowDate;
    string BackDate;
    char c;
    int flag=0;
    SortStudent(1);
    int sid=-1;
    for (int i = 0; i <(int)data.size(); i++)
    {
        if (data[i].name==name&&data[i].author==author)
        {
            if(data[i].nowsum)
            {
                cout<<"借书读者的读者号是：";
                cin>>sid;
                if(data1[sid-1].number>10)
                {
                    cout<<"现你同时借了10本书！不可再借！"<<endl;
                    break;
                }
                flag=1;
                data[i].nowsum=data[i].nowsum-1;
                data[i].BorrowCount=data[i].BorrowCount+1;
                cout<<"请输入借阅日期"<<endl;
                cin>>BorrowDate;
                data[i].borrowdate=BorrowDate;
                cout<<"请输入预计归还日期(最多可借一月)"<<endl;
                cin>>BackDate;
                data[i].returndate=BackDate;
                data[i].ok=bool(data[i].nowsum);
                borrownum++;
                data1[sid-1].number++;
                for(int j=0;j<10;j++)
                {
                    if(data1[sid-1].b[j]==0)
                    data1[sid-1].b[j]=data[i].idnum;
                    Save();
                    Save1();
                }
            }
            else
            {
                cout<<"此书暂不在馆！"<<endl;
            }
        }
    }
    if(!flag)
        cout<<"抱歉，未找到您要找的书。"<<endl;
}
void Library::BackBook(string name,string author,int k)//k表示还书途径
{
    int c=-1;
    SortStudent(1);
    if(k!=-1)//读者自助还书
    {
        c=k-1;
    }
    else//读者通过借还总台还书
    {
        cout<<"请输入你的读者号：";
        cin>>c;
        c=c-1;
    }
    for (int i = 0; i <(int)data.size(); i++)
    {
        if (data[i].name==name&&data[i].author==author)
        {
            data[i].nowsum=data[i].nowsum+1;
            data[i].ok=bool(data[i].nowsum);
            returnnum++;
            for(int j=0;j<10;j++)
                {
                    if(data1[c].b[j]==data[i].idnum)
                        data1[c].b[j]=0;
                }
                data1[c].number--;
            break;
        }
    }
    Save();
    Save1();
}
void Library::printbook(book a)
{
    cout<<setw(8)<<a.idnum;
    cout<<setw(10)<<a.name;
    cout<<setw(10)<<a.author;
    cout<<setw(10)<<a.kind;
    cout<<setw(10)<<a.sum;
    cout<<setw(10)<<a.nowsum;
    cout<<setw(10)<<a.BorrowCount;
    cout<<setw(10)<<(a.ok==0?"不可借":"可借");
    cout<<setw(14)<<a.borrowdate;
    cout<<setw(14)<<a.returndate<<endl;
}

void Library::PrintStudent(int kid)
{
   int id=kid-1;

    SortStudent(1);
    SortBook(1);
    cout<<setw(8)<<data1[id].id;
    cout<<setw(8)<<data1[id].name;
    cout<<setw(14)<<data1[id].borrowsum;
    cout<<setw(18)<<data1[id].number;
    cout<<setw(14)<<data1[id].borrowday<<endl;
    if(data1[id].number)
    {
        cout<<"你当前借了这些书：\n";
        cout<<setw(16)<<"检索号"<<setw(16)<<"书名"<<setw(16)<<"作者"<<endl;
        for(int i=0;i<10;i++)
        {
            if(data1[id].b[i]!=0)
            cout<<setw(16)<<data[data1[id].b[i]-1].idnum<<setw(16)<<data[data1[id].b[i]-1].name<<setw(16)<<data[data1[id].b[i]-1].author<<endl;
        }
    }
    else
        cout<<"你当前并未借任何书!\n";
}
void Library::ShowAllBook()
{
    cout<<setw(8)<<"检索号"<<setw(10)<<"书名"<<setw(10)<<"作者"<<setw(10)<<"种类"<<setw(10)<<"总库存量"<<setw(10)<<"在馆量"<<setw(10)<<"借阅量"<<setw(10)<<"是否可借";
    cout<<setw(14)<<"借出时间"<<setw(14)<<"归还时间"<<endl;
    for (int i = 0; i <(int)data.size(); i++)
    {
        printbook(data[i]);
       
    }
}
int Library::SerchStudent(int id)
{
    int m=-1;
     for (int i = 0; i <(int)data1.size(); i++)
     {
        if (data1[i].id==id)
        {
            return i;
        }
     }
     return m;
 }
void Library::SearchBookPosWithname(string thebook)//按书名查询
{
    int flag=0;
    for (int i = 0; i <(int)data.size(); i++)
    {
        if (data[i].name==thebook)
        {
            printbook(data[i]);
            flag=1;
        }
    }
    if(!flag) cout<<"查无此书！\n";
}
void Library::SearchBookPosWithAuthor(string theauthor)//按作者查询
{
    bool flag=false;
    for (int i = 0; i <(int)data.size(); i++)
    {
        if (data[i].author==theauthor)
        {
            flag=true;
            printbook(data[i]);
        }
    }
    if(!flag) cout<<"查无此作者的书！";
}
void Library::SearchBookPosWithKind(string kind)//按种类查询
{
    bool flag=false;
    for (int i = 0; i <(int)data.size(); ++i)
    {
        if (data[i].kind==kind)
        {
            flag=true;
            printbook(data[i]);
        }
    }
    if(!flag) cout<<"查无此类书！";
}
int Library::SearchBookPosWithAB(string theauthor,string thebook)//按作者和书名查询
{
    for (int i = 0; i <(int)data.size(); ++i)
    {
        if (data[i].author==theauthor&&data[i].name==thebook)
        {
            printbook(data[i]);
            return i;
        }
    }
    cout<<"查无此书！";
    return -1;
}
int Library::SearchBookPosWithid(int id)
{
    for (int i = 0; i <(int)data.size(); i++)
    {
        if (data[i].idnum==id)
        {
            return i;
        }
    }
    return -1;
}
void Library::SortBook(int ca)  //排序
{
    if (ca==0)   //按借阅量排序
    {
        sort(data.begin(),data.end(),cmpByCount);
    }
    else    //按检索号排序
    {
        sort(data.begin(),data.end(),cmpByidnum);
    }
}
void Library::SortStudent(int ca)
{
    if(ca==0)//按借阅量排序
        sort(data1.begin(),data1.end(),cmpByBorrowsum);
    else
        sort(data1.begin(),data1.end(),cmpByid);//按读者号排序
}
void Library::Save() //存入书籍文件
{
    ofstream fout("book.txt");
    if (fout)
    {
        fout<<data.size()<<endl;
        for (int i = 0; i <(int)data.size(); i++)
        {
            fout<<data[i].idnum<<" "<<data[i].name<<" "<<data[i].author<<" "<<data[i].kind<<" "<<data[i].sum<<" "<<data[i].nowsum<<" "<<data[i].BorrowCount<<" "<<data[i].ok<<" "<<data[i].borrowdate<<" "<<data[i].returndate<<" "<<endl;
        }
        fout.close();
    }
}
void Library::Save1() //存入学生文件
{
    ofstream fout("student.txt");
    if (fout)
    {
        fout<<data1.size()<<endl;

        for (int i = 0; i <(int)data1.size(); i++)
        {
            fout<<data1[i].id<<" "<<data1[i].name<<" "<<data1[i].borrowsum<<" "<<data1[i].number<<" "<<data1[i].borrowday;
            for(int j=0;j<10;j++)
            {
                fout<<" "<<data1[i].b[j];
            }
            fout<<endl;
        }
        fout.close();
    }
}
void Library::revisebook(string name,string author)//修改图书
{
    string Room,Kind;
    int num,k=0;
    printf("你要修改的内容是：\n");
    printf("                 1.图书总库存量修改\n");
    printf("                 2.图书所属种类修改\n");
    printf("                 3.退出\n");
    for (int i = 0; i <(int)data.size(); i++)
    {
        if (data[i].author==author&&data[i].name==name)
        {
            k=i;
            break;
        }
    }
    int cho;
    do
    {
        cin>>cho;
        switch(cho)
        {
        
        case 1:
        {
            cout<<"请输入新的总库存量：\n";
            cin>>num;
            data[k].sum=num;
            break;
        }
        case 2:
        {
            cout<<"请输入图书所属新种类：\n";
            cin>>Kind;
            data[k].kind=Kind;
            break;
        }
        }
    }
    while(cho<3);
}

int main()
{
	system("color 80");
    cout.setf(ios::left);//左对齐
    Library mybook;
    char mm[6];//密码数组
    cout<<"                                                   欢迎使用图书管理系统"<<endl;
    cout<<"请选择你的登录选项：\n";
    cout<<"                  1.管理员登录"<<endl;
    cout<<"                  2.读者登录"<<endl;
    int cho,start,kk=1;
    cin>>start;
    switch(start)
    {
        case 1:
       {
        char password[]="802377";
        cout<<"请输入管理员口令:";
        do{
            kk=1;
           for(int i=0;i<6;i++)
           {
            mm[i]=getch();
            cout<<"*";
           }
           cout<<endl;
           for(int i=0;i<6;i++)
           {
           if (mm[i]!=password[i])
           {
            cout<<"请输入正确密码！"<<endl;
            kk=0;
            break;
           }
           }
        }while(!kk);
        do
        {
            cout<<"              1.所有图书目录 "<<endl;
            cout<<"              2.查询图书 "<<endl;
            cout<<"              3.增加图书 "<<endl;
            cout<<"              4.删除图书 "<<endl;
            cout<<"              5.借阅图书 "<<endl;
            cout<<"              6.归还图书 "<<endl;
            cout<<"              7.修改图书信息 "<<endl;
            cout<<"              8.本月借还统计 "<<endl;
            cout<<"              9.退出 "<<endl;
            cout<<"-------------------------------------------------------------------------------------------------------------------"<<endl;
            cout<<"              请选择功能,输入指令 "<<endl;
            cin>>cho;
            switch(cho)
            {
            case 1:
            {
                int cho2;
                do
                {
                    mybook.ShowAllBook();
                    cout<<"-------------------------------------------------------------------------------------------------------------------------------------------------------------"<<endl;
                    cout<<"0:按借阅量排序 , 1:按索引号排序 ,3:退出"<<endl;
                    cin>>cho2;
                    switch(cho2)
                    {
                    case 0:
                        mybook.SortBook(0);
                        break;
                    case 1:
                        mybook.SortBook(1);
                        break;
                    }
                }
                while (cho2<2);
                break;
            }
            case 2:
            {
                cout<<"              1.按书名查询 "<<endl;
                cout<<"              2.按作者查询 "<<endl;
                cout<<"              3.按种类查询 "<<endl;
                cout<<"              4.按书名和作者查询 "<<endl;
                cout<<"              5.退出 "<<endl;
                cout<<"              请选择功能,输入指令 "<<endl;
                int cho3;
                do
                {
                    string Name,AutHor,Kind;
                    cin>>cho3;
                    switch(cho3)
                    {
                    case 1:
                        cout<<"请输入书名！"<<endl;
                        cin>>Name;
                        mybook.SearchBookPosWithname(Name); //按书名查询
                        break;
                    case 2:
                        cout<<"请输入作者！"<<endl;
                        cin>>AutHor;
                        mybook.SearchBookPosWithAuthor(AutHor);//按作者查询
                        break;
                    case 3:
                        cout<<"请输入种类！"<<endl;
                        cin>>Kind;
                        mybook.SearchBookPosWithKind(Kind);//按种类查询
                        break;
                    case 4:
                        cout<<"请输入作者和书名！"<<endl;
                        cin>>AutHor>>Name;
                        mybook.SearchBookPosWithAB(AutHor,Name);//按作者和书名查询
                        break;
                    }
                }
                while(cho3<=4&&cho3>=1);
                break;
            }
            case 3:         //增加图书
            {
                book temp;
                cout<<"请输入检索号:";
                cin>>temp.idnum;
                while (mybook.SearchBookPosWithid(temp.idnum)>-1)
                {
                    cout<<"检索号重复啦！~~~~(>_<)~~~~ !"<<endl;
                    cin>>temp.idnum;
                }
                cin.clear();
                cin.sync();
                cout<<"书名:"<<endl;
                cin>>temp.name;
                cout<<"作者："<<endl;
                cin>>temp.author;

                cout<<"种类:"<<endl;
                cin>>temp.kind;

                cout<<"数量:"<<endl;
                cin>>temp.sum;
                temp.nowsum=temp.sum;
                temp.BorrowCount=0;
                temp.ok=true;
                temp.borrowdate="0000.00.00";
                temp.returndate="0000.00.00";
                mybook.AddBook(temp);
                mybook.Save();
                cout<<"信息保存成功"<<endl;
                break;
            }
            case 4:         //删除图书
            {
                string bookname,bookauthor;
                cout<<"请输入书名和作者:"<<endl;
                cin>>bookname>>bookauthor;
                mybook.DeleteBook(bookname,bookauthor);
                break;
            }
            case 5:         //借书
            {
                string bookname,bookauthor;
                cout<<"请输入要借的书名和作者："<<endl;
                cin>>bookname>>bookauthor;
                mybook.BorrowBook(bookname,bookauthor);
                mybook.Save();
                break;
            }
            case 6:         //还书
            {
                string bookname,bookauthor;
                cout<<"请输入要还的书名和作者："<<endl;
                cin>>bookname>>bookauthor;
                mybook.BackBook(bookname,bookauthor,-1);
                mybook.Save();
                break;
            }
            case 7:
            {
                string name,author;
                cout<<"请输入要修改的书名和作者："<<endl;
                cin>>name>>author;
                mybook.revisebook(name,author);
                break;
            }
            case 8:
            {
                  printf("                     本月借书%d次\n",mybook.getborrownum());
                  printf("                     本月还书%d次\n",mybook.getreturnnum());
                  break;
            }

            }
        }
        while (cho<9);
        break;
    }
    case 2:
    {
        int bh,cho,k;
        cout<<"请输入你的读者编号：\n";
        cin>>bh;
        if(mybook.SerchStudent(bh)==-1)
        {
            int n;
            cout<<"你不是此系统读者，是否注册？\n";
            cout<<"                    1.注册\n";
            cout<<"                    2.我再想想\n";
            cin>>n;
            student temp;
            if(n==1)
            {
                cout<<"请输入你的姓名:";
                cin>>temp.name;
                cin.clear();
                cin.sync();
                temp.id=mybook.GetStudent();
                temp.borrowsum=0;
                temp.number=0;
                temp.borrowday="0000.00.00";
                for(int i=0;i<10;i++)
                {
                    temp.b[i]=0;
                }
                mybook.AddStudent(temp);
                mybook.Save1();
                cout<<"                注册成功！请记住你的读者号，若忘记请联系管理员!\n";
                cout<<"                姓名:"<<temp.name<<endl;
                cout<<"                读者号："<<temp.id<<endl;
                k=temp.id;
            }
            else
                break;
        }
        else
        {
            k=bh;
        }
        do
        {

            cout<<"              1.所有图书目录 "<<endl;
            cout<<"              2.查询图书 "<<endl;
            cout<<"              3.查看我的借阅信息 "<<endl;
            cout<<"              4.退出 "<<endl;
            cout<<"              请选择功能,输入指令 "<<endl;
            cin>>cho;
            switch(cho)
            {
            case 1:
            {
                int cho2;
                do
                {
                    mybook.ShowAllBook();
                    cout<<"-------------------------------------------------------------------------------------------------------------------------------------------------------------"<<endl;
                    cout<<"0:按借阅量排序 , 1:按索引号排序 ,3:退出"<<endl;
                    cin>>cho2;
                    switch(cho2)
                    {
                    case 0:
                        mybook.SortBook(0);
                        break;
                    case 1:
                        mybook.SortBook(1);
                        break;
                    }
                }
                while (cho2<2);
                break;
            }
            case 2:
            {
                cout<<"              1.按书名查询 "<<endl;
                cout<<"              2.按作者查询 "<<endl;
                cout<<"              3.按种类查询 "<<endl;
                cout<<"              4.按书名和作者查询 "<<endl;
                cout<<"              5.退出 "<<endl;
                cout<<"              请选择功能,输入指令 "<<endl;
                int cho3;
                do
                {
                    string Name,AutHor,Kind;
                    cin>>cho3;
                    switch(cho3)
                    {
                    case 1:
                        cout<<"请输入书名！"<<endl;
                        cin>>Name;
                        mybook.SearchBookPosWithname(Name); //按书名查询
                        break;
                    case 2:
                        cout<<"请输入作者！"<<endl;
                        cin>>AutHor;
                        mybook.SearchBookPosWithAuthor(AutHor);//按作者查询
                        break;
                    case 3:
                        cout<<"请输入种类！"<<endl;
                        cin>>Kind;
                        mybook.SearchBookPosWithKind(Kind);//按种类查询
                        break;
                    case 4:
                        cout<<"请输入作者和书名！"<<endl;
                        cin>>AutHor>>Name;
                        mybook.SearchBookPosWithAB(AutHor,Name);//按作者和书名查询
                        break;
                    }
                }
                while(cho3<=4&&cho3>=1);
                break;
            }
           
            case 3://查阅个人信息
            {
                cout<<setw(8)<<"读者号"<<setw(8)<<"姓名"<<setw(14)<<"历史借阅次数"<<setw(18)<<"现在借阅书个数"<<setw(14)<<"上次还书时间"<<endl;
                mybook.PrintStudent(k);
                mybook.SortStudent(0);
                printf("你在所有读者中借阅量排名第 %d 位\n",mybook.SerchStudent(k)+1);
                printf("所有读者中的最高借阅量是：%d次\n",mybook.readsum(1));
                mybook.SortStudent(1);
                cout<<"-------------------------------------------------------------------------------------------------------------------"<<endl;
                break;
            }
            }
        }
        while (cho<4);
        break;
    }
   }
   mybook.Save();
   mybook.Save1();
   return 0;
}

