#include<stdio.h>
#include<string.h>
#include<WinSock2.h>
#include<time.h>
#include<string>
#include<iostream>
#pragma comment (lib,"ws2_32.lib")
using namespace std;
SOCKET sockClient;//ȫ�ֱ���������ͨ�ŵ�socket
//�����߳����ڽ��պͷ�����Ϣ
void sendFun();//������Ϣ�߳�
void recvFun();//������Ϣ�߳�
char name[16] = "Ĭ������2";//��ʼ������
int main()
{
    HANDLE h1, h2;//�߳̾������ʵ����һ������������ʶ�̶߳���
    SOCKADDR_IN addr;//�����������socket��ַ��Ϣ
    int info;//�жϳ�ʼ����Ϣ
    WSADATA data;//�洢��WSAStartup�������ú󷵻ص�Windows Sockets����
    WORD version;//socket�汾

    //�趨�汾�����ʼ��
    version = MAKEWORD(2, 2);//�汾�趨
    info = WSAStartup(version, &data);


    //1.�����׽���
    sockClient = socket(AF_INET, SOCK_STREAM, 0);
    //Ҫ���ӵķ�������ip,��Ϊ���ڷ������˾��Ǳ���������д����ip                                           
    //127.0.0.1һ�������IP��ַ����ʾ�Ǳ�����IP��ַ                                               
    addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    //�˿�Ҫ���������ͬ
    addr.sin_port = htons(11111);
    //��IPV4��ַ
    addr.sin_family = AF_INET;


    cout << "����������û���:";
    cin >> name;
    //�������ӷ�����
    cout << "������Է���������" << endl;
    while (1) {
        //2.����ָ��������˿�
        if (connect(sockClient, (SOCKADDR*)&addr, sizeof(SOCKADDR)) == 0) {
            cout << "������!" << endl;
            //�����̺߳�����ִ��
            //��socket�з���/������Ϣ
            h1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)sendFun, NULL, 0, NULL);
            h2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)recvFun, NULL, 0, NULL);
            WaitForSingleObject(h1, INFINITE);//��������ֱ���߳����н���
            WaitForSingleObject(h2, INFINITE);
        }
        else
            ;
    }
    //�ر�
    shutdown(sockClient, 2);
    closesocket(sockClient);//�ر��׽���
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
        cout << name << "��������Ϣ:";

        cin >> buf1;


        if (strcmp(buf1, "bye!") == 0) {
            cout << "�����ѽ���!" << endl;
            send(sockClient, "End", 4, 0);
            break;
        }//�ж��������
        cout << "������Ϣ,��Ϣ����ϸ������:"<<endl;

        strftime(tmp, sizeof(tmp), "%Y/%m/%d %X %A ", localtime(&t));
        strcpy(buf, "����ʱ��:");
        strcat(buf, tmp);
        strcat(buf, "\n������:");
        strcat(buf, name);
        strcat(buf, "\n��Ϣ:");
        //strcat(buf, "\n");
        strcat(buf, buf1);
        cout << buf << endl;
        //��������
        send(sockClient, buf, strlen(buf) + 1, 0);
    }
}


void recvFun()
{
    char buf[256];
    //���շ����͵�����
    while (1)
    {
        int n;
        if (recv(sockClient, buf, 256, 0) > 0) {
            if (strcmp(buf, "End") == 0) {
                cout << "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
                cout << "�Է��ѽ�������" << endl;
                break;
            }
            else {
                cout << "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
                cout << "������Ϣ,��Ϣ����ϸ������:" << endl;
                cout << buf << endl;
                cout << endl;
                cout << name << "��������Ϣ:";
            }
        }
        else {
            break;
        }
    }
}
