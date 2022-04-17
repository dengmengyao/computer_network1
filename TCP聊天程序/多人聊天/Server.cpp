#include<stdio.h>
#include<string.h>
#include<WinSock2.h>
#include<time.h>
#include<string>
#include<iostream>
#pragma comment (lib,"ws2_32.lib")
using namespace std;
SOCKET sockConn[100];//全局变量，用来通信的socket
void hThread();
int flag = 0;
char name[16] = "默认姓名1";//初始化姓名
HANDLE h0[100],h1, h2;//线程句柄
int u = 1;
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


    //cout << "请输入你的用户名:";
    //cin >> name;
    //2.绑定socket端口号
    bind(serverSocket, (SOCKADDR*)&addr, sizeof(SOCKADDR));//将套接字serverSocket与端口接收的ip绑定
    //3.开始监听，是否有客服端请求连接,最大连接数为100
    listen(serverSocket, 100);
    cout << "开始聊天，等待对方连接.........." << endl;
    int len = sizeof(SOCKADDR);
    //accept是一个阻塞函数，如果没有客户端请求，连接会一直等待在这里
    //该函数会返回一个新的套接字，这个新的套接字是用来与客户端通信的套接字，之前那个套接字是监听的套接字
    while (1) {
        //4.接受来自客户端的连接请求
        sockConn[u] = accept(serverSocket, (SOCKADDR*)&newAddr, &len);//接受客户端的请求
        cout << "连接成功......" << endl;
        u += 1;
         for (int i = 1; i <=u; i++) {
             h0[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)hThread, NULL, 0, NULL);
             //WaitForSingleObject(h0[i], INFINITE);
      }
        //创建线程后立即运行
        //第一个参数表示线程内核对象的安全属性；第二个参数表示线程栈空间大小；第三个参数表示新线程所执行的线程函数地址（函数的名字），多个线程可以使用同一个函数地址
        //第四个参数是传递给线程函数的参数；第五个参数指定什么时候调用线程，为0表示线程创建之后就可以进行调用；第六个参数返回线程的ID号，传入NULL表示不需要返回该线程ID号
        //5.向socket中读取/写入信息

    }

    //6.关闭
    closesocket(serverSocket);//关闭套接字
    return 0;
}
void hThread() {
        for (int i = 1; i <= u; i++) {
            char buf[512];
            if (recv(sockConn[i], buf, 512, 0) > 0) {
                if (strcmp(buf, "End") == 0) {
                    cout << "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
                    cout << "已结束聊天" << endl;
                    for (int j = 1; j <= u; j++) {
                        if (j == i)
                            continue;
                        else
                            send(sockConn[j], buf, strlen(buf) + 1, 0);
                    }
                   // break;
                }
                else {
                    cout << "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
                    cout << "接收消息,消息具体细节如下:" << endl;
                    cout << buf << endl;
                    cout << endl;
                    cout << name << "请输入消息:";
                    for (int j = 1; j <= u; j++) {
                        if (j == i)
                            continue;
                        else
                            send(sockConn[j], buf, strlen(buf) + 1, 0);
                    }
                }
            }
            else
                continue;
        }
}
