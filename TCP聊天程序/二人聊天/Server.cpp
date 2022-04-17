#include<stdio.h>
#include<string.h>
#include<WinSock2.h>
#include<time.h>
#include<string>
#include<iostream>
#pragma comment (lib,"ws2_32.lib")
using namespace std;
SOCKET sockConn;//全局变量，用来通信的socket
void recvFun();//接收信息线程
void sendFun();//发送信息线程
int flag = 0;
char name[16]="默认姓名1";//初始化姓名
HANDLE h1, h2;//线程句柄
int main()
{

    SOCKET serverSocket;//监视的套接字
    SOCKADDR_IN newAddr;//保存客户端的socket地址信息
    SOCKADDR_IN addr;//地址结构体，包括ip port(端口)
    WSADATA data;//存储被WSAStartup函数调用后返回的Windows Sockets数据
    WORD version;//socket版本
    int info;

    //在使用socket之前要进行版本的设定和初始化
    version = MAKEWORD(2, 2);//设定版本
    info = WSAStartup(version, &data);
    //应用程序或DLL只能在一次成功的WSAStartup()调用之后才能调用进一步的Windows Sockets API函数。
    //有套接字的接口才能进行通信


    //1.创建socket
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//AF_INET使用IPV4地址，SOCK_STREAM使用流传输，IPPROTO_TCP使用TCP协议
    addr.sin_addr.S_un.S_addr = htonl(ADDR_ANY);//表示任何的ip过来连接都接受
    addr.sin_family = AF_INET;//使用ipv4的地址
    addr.sin_port = htons(11111);//设定应用占用的端口


    cout << "请输入你的用户名:";
    cin >> name;
    //2.绑定socket端口号
    bind(serverSocket, (SOCKADDR*)&addr, sizeof(SOCKADDR));//将套接字serverSocket与端口接收的ip绑定
    //3.开始监听，是否有客服端请求连接,最大连接数为3
    listen(serverSocket, 3);
    cout<<"开始聊天，等待对方连接.........."<<endl;
    int len = sizeof(SOCKADDR);
    //accept是一个阻塞函数，如果没有客户端请求，连接会一直等待在这里
    //该函数会返回一个新的套接字，这个新的套接字是用来与客户端通信的套接字，之前那个套接字是监听的套接字
    while (1) {
        //4.接受来自客户端的连接请求
        sockConn = accept(serverSocket, (SOCKADDR*)&newAddr, &len);//接受客户端的请求
        cout<<"连接成功......"<<endl;
        //创建线程后立即运行
        //第一个参数表示线程内核对象的安全属性；第二个参数表示线程栈空间大小；第三个参数表示新线程所执行的线程函数地址（函数的名字），多个线程可以使用同一个函数地址
        //第四个参数是传递给线程函数的参数；第五个参数指定什么时候调用线程，为0表示线程创建之后就可以进行调用；第六个参数返回线程的ID号，传入NULL表示不需要返回该线程ID号
        //5.向socket中读取/写入信息
        h1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)sendFun, NULL, 0, NULL);//用于发送的线程
        h2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)recvFun, NULL, 0, NULL);//用于接收的线程
    }
    //6.关闭
    closesocket(sockConn);//关闭套接字
    return 0;
}
void sendFun()
{
    char buf[512];//定义报文
    while (1)
    {        
        char buf1[512];//输入信息
        time_t t = time(0);
        char tmp[64];
        cout << endl;
        cout << name << "请输入消息:";

        cin >> buf1;//输入聊天信息

        if (strcmp(buf1,"bye!")==0) {
            cout << "聊天已结束!" << endl;
            send(sockConn, "End", 4, 0);//发送信息
            break;
        }//判断聊天结束

        cout << "发送消息,消息具体细节如下:"<<endl;

        strftime(tmp, sizeof(tmp), "%Y/%m/%d %X %A ", localtime(&t));//拼接整合完整报文
        strcpy(buf, "发送时间:");
        strcat(buf, tmp);
        strcat(buf, "\n发送人:");
        strcat(buf, name);
        strcat(buf, "\n消息:");
        //strcat(buf, "\n");
        strcat(buf, buf1);
        cout << buf << endl;
        //sockConn为用来通信的套接字，buf为发送数据的缓冲区，strlen(buf)+1为发送数据的长度，0位flags标志
        send(sockConn, buf, strlen(buf) + 1, 0);//发送信息
    }
}

void recvFun()
{
    char buf[512];
    while (1)
    {
        int Ret = recv(sockConn, buf, 256, 0);//接收信息
        if (Ret < 0) {
            cout<<"\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
            cout<<"对方已退出!"<<endl;
            flag = flag % 2 + 1;
            break;
        }
        else if (Ret == 0) {
            cout<<"ERROR_RECV";
        }
        else if(strcmp(buf, "End") == 0) {//若收到send发出的End,则结束聊天
            cout << "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
            cout << "对方已结束聊天" << endl;
            break;
        }
        else{
            cout << "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
            cout << "接收消息,消息具体细节如下:" << endl;
            cout << buf << endl;
            cout << endl;
            cout << name << "请输入消息:";
        
        }
    }
}

