#include<stdio.h>
#include<string.h>
#include<WinSock2.h>
#include<time.h>
#include<string>
#include<iostream>
#pragma comment (lib,"ws2_32.lib")
using namespace std;
SOCKET sockConn[100];//ȫ�ֱ���������ͨ�ŵ�socket
void hThread();
int flag = 0;
char name[16] = "Ĭ������1";//��ʼ������
HANDLE h0[100],h1, h2;//�߳̾��
int u = 1;
int main()
{

    SOCKET serverSocket;//���ӵ��׽���
    SOCKADDR_IN newAddr;//����ͻ��˵�socket��ַ��Ϣ
    SOCKADDR_IN addr;//��ַ�ṹ�壬����ip port(�˿�)
    WSADATA data;//�洢��WSAStartup�������ú󷵻ص�Windows Sockets����
    WORD version;//socket�汾
    int info;

    //��ʹ��socket֮ǰҪ���а汾���趨�ͳ�ʼ��
    version = MAKEWORD(2, 2);//�趨�汾
    info = WSAStartup(version, &data);
    //Ӧ�ó����DLLֻ����һ�γɹ���WSAStartup()����֮����ܵ��ý�һ����Windows Sockets API������
    //���׽��ֵĽӿڲ��ܽ���ͨ��


    //1.����socket
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//AF_INETʹ��IPV4��ַ��SOCK_STREAMʹ�������䣬IPPROTO_TCPʹ��TCPЭ��
    addr.sin_addr.S_un.S_addr = htonl(ADDR_ANY);//��ʾ�κε�ip�������Ӷ�����
    addr.sin_family = AF_INET;//ʹ��ipv4�ĵ�ַ
    addr.sin_port = htons(11111);//�趨Ӧ��ռ�õĶ˿�


    //cout << "����������û���:";
    //cin >> name;
    //2.��socket�˿ں�
    bind(serverSocket, (SOCKADDR*)&addr, sizeof(SOCKADDR));//���׽���serverSocket��˿ڽ��յ�ip��
    //3.��ʼ�������Ƿ��пͷ�����������,���������Ϊ100
    listen(serverSocket, 100);
    cout << "��ʼ���죬�ȴ��Է�����.........." << endl;
    int len = sizeof(SOCKADDR);
    //accept��һ���������������û�пͻ����������ӻ�һֱ�ȴ�������
    //�ú����᷵��һ���µ��׽��֣�����µ��׽�����������ͻ���ͨ�ŵ��׽��֣�֮ǰ�Ǹ��׽����Ǽ������׽���
    while (1) {
        //4.�������Կͻ��˵���������
        sockConn[u] = accept(serverSocket, (SOCKADDR*)&newAddr, &len);//���ܿͻ��˵�����
        cout << "���ӳɹ�......" << endl;
        u += 1;
         for (int i = 1; i <=u; i++) {
             h0[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)hThread, NULL, 0, NULL);
             //WaitForSingleObject(h0[i], INFINITE);
      }
        //�����̺߳���������
        //��һ��������ʾ�߳��ں˶���İ�ȫ���ԣ��ڶ���������ʾ�߳�ջ�ռ��С��������������ʾ���߳���ִ�е��̺߳�����ַ�����������֣�������߳̿���ʹ��ͬһ��������ַ
        //���ĸ������Ǵ��ݸ��̺߳����Ĳ��������������ָ��ʲôʱ������̣߳�Ϊ0��ʾ�̴߳���֮��Ϳ��Խ��е��ã����������������̵߳�ID�ţ�����NULL��ʾ����Ҫ���ظ��߳�ID��
        //5.��socket�ж�ȡ/д����Ϣ

    }

    //6.�ر�
    closesocket(serverSocket);//�ر��׽���
    return 0;
}
void hThread() {
        for (int i = 1; i <= u; i++) {
            char buf[512];
            if (recv(sockConn[i], buf, 512, 0) > 0) {
                if (strcmp(buf, "End") == 0) {
                    cout << "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
                    cout << "�ѽ�������" << endl;
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
                    cout << "������Ϣ,��Ϣ����ϸ������:" << endl;
                    cout << buf << endl;
                    cout << endl;
                    cout << name << "��������Ϣ:";
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
