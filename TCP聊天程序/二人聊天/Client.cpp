#include<stdio.h>
#include<string.h>
#include<WinSock2.h>
#include<time.h>
#include<string>
#include<iostream>
#pragma comment (lib,"ws2_32.lib")
using namespace std;
SOCKET sockClient;//全局变量，用于通信的socket
//两个线程用于接收和发送信息
void sendFun();//发送信息线程
void recvFun();//接收信息线程
char name[16] = "默认姓名2";//初始化姓名
int main()
{
    HANDLE h1, h2;//线程句柄，其实就是一串数字用来标识线程对象
    SOCKADDR_IN addr;//保存服务器的socket地址信息
    int info;//判断初始化信息
    WSADATA data;//存储被WSAStartup函数调用后返回的Windows Sockets数据
    WORD version;//socket版本

    //设定版本，与初始化
    version = MAKEWORD(2, 2);//版本设定
    info = WSAStartup(version, &data);


    //1.创建套接字
    sockClient = socket(AF_INET, SOCK_STREAM, 0);
    //要连接的服务器的ip,因为现在服务器端就是本机，所以写本机ip                                           
    //127.0.0.1一个特殊的IP地址，表示是本机的IP地址                                               
    addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    //端口要与服务器相同
    addr.sin_port = htons(11111);
    //用IPV4地址
    addr.sin_family = AF_INET;


    cout << "请输入你的用户名:";
    cin >> name;
    //主动连接服务器
    cout << "尝试与对方进行连接" << endl;
    while (1) {
        //2.连接指定计算机端口
        if (connect(sockClient, (SOCKADDR*)&addr, sizeof(SOCKADDR)) == 0) {
            cout << "已连接!" << endl;
            //创建线程后立即执行
            //向socket中发送/接受信息
            h1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)sendFun, NULL, 0, NULL);
            h2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)recvFun, NULL, 0, NULL);
            WaitForSingleObject(h1, INFINITE);//会阻塞，直到线程运行结束
            WaitForSingleObject(h2, INFINITE);
        }
        else
            ;
    }
    //关闭
    shutdown(sockClient, 2);
    closesocket(sockClient);//关闭套接字
    return 0;
}

void sendFun()
{
    char buf[256];
    while (1)
    {
        char buf1[256];
        time_t t = time(0);
        char tmp[64];

        cout << endl;
        cout << name << "请输入消息:";

        cin >> buf1;


        if (strcmp(buf1, "bye!") == 0) {
            cout << "聊天已结束!" << endl;
            send(sockClient, "End", 4, 0);
            break;
        }//判断聊天结束
        cout << "发送消息,消息具体细节如下:"<<endl;

        strftime(tmp, sizeof(tmp), "%Y/%m/%d %X %A ", localtime(&t));
        strcpy(buf, "发送时间:");
        strcat(buf, tmp);
        strcat(buf, "\n发送人:");
        strcat(buf, name);
        strcat(buf, "\n消息:");
        //strcat(buf, "\n");
        strcat(buf, buf1);
        cout << buf << endl;
        //发送数据
        send(sockClient, buf, strlen(buf) + 1, 0);
    }
}


void recvFun()
{
    char buf[256];
    //接收服务发送的数据
    while (1)
    {
        int n;
        if (recv(sockClient, buf, 256, 0) > 0) {
            if (strcmp(buf, "End") == 0) {
                cout << "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
                cout << "对方已结束聊天" << endl;
                break;
            }
            else {
                cout << "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
                cout << "接收消息,消息具体细节如下:" << endl;
                cout << buf << endl;
                cout << endl;
                cout << name << "请输入消息:";
            }
        }
        else {
            break;
        }
    }
}
