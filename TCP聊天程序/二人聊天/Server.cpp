#include<stdio.h>
#include<string.h>
#include<WinSock2.h>
#include<time.h>
#include<string>
#include<iostream>
#pragma comment (lib,"ws2_32.lib")
using namespace std;
SOCKET sockConn;//ȫ�ֱ���������ͨ�ŵ�socket
void recvFun();//������Ϣ�߳�
void sendFun();//������Ϣ�߳�
int flag = 0;
char name[16]="Ĭ������1";//��ʼ������
HANDLE h1, h2;//�߳̾��
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


    cout << "����������û���:";
    cin >> name;
    //2.��socket�˿ں�
    bind(serverSocket, (SOCKADDR*)&addr, sizeof(SOCKADDR));//���׽���serverSocket��˿ڽ��յ�ip��
    //3.��ʼ�������Ƿ��пͷ�����������,���������Ϊ3
    listen(serverSocket, 3);
    cout<<"��ʼ���죬�ȴ��Է�����.........."<<endl;
    int len = sizeof(SOCKADDR);
    //accept��һ���������������û�пͻ����������ӻ�һֱ�ȴ�������
    //�ú����᷵��һ���µ��׽��֣�����µ��׽�����������ͻ���ͨ�ŵ��׽��֣�֮ǰ�Ǹ��׽����Ǽ������׽���
    while (1) {
        //4.�������Կͻ��˵���������
        sockConn = accept(serverSocket, (SOCKADDR*)&newAddr, &len);//���ܿͻ��˵�����
        cout<<"���ӳɹ�......"<<endl;
        //�����̺߳���������
        //��һ��������ʾ�߳��ں˶���İ�ȫ���ԣ��ڶ���������ʾ�߳�ջ�ռ��С��������������ʾ���߳���ִ�е��̺߳�����ַ�����������֣�������߳̿���ʹ��ͬһ��������ַ
        //���ĸ������Ǵ��ݸ��̺߳����Ĳ��������������ָ��ʲôʱ������̣߳�Ϊ0��ʾ�̴߳���֮��Ϳ��Խ��е��ã����������������̵߳�ID�ţ�����NULL��ʾ����Ҫ���ظ��߳�ID��
        //5.��socket�ж�ȡ/д����Ϣ
        h1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)sendFun, NULL, 0, NULL);//���ڷ��͵��߳�
        h2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)recvFun, NULL, 0, NULL);//���ڽ��յ��߳�
    }
    //6.�ر�
    closesocket(sockConn);//�ر��׽���
    return 0;
}
void sendFun()
{
    char buf[512];//���屨��
    while (1)
    {        
        char buf1[512];//������Ϣ
        time_t t = time(0);
        char tmp[64];
        cout << endl;
        cout << name << "��������Ϣ:";

        cin >> buf1;//����������Ϣ

        if (strcmp(buf1,"bye!")==0) {
            cout << "�����ѽ���!" << endl;
            send(sockConn, "End", 4, 0);//������Ϣ
            break;
        }//�ж��������

        cout << "������Ϣ,��Ϣ����ϸ������:"<<endl;

        strftime(tmp, sizeof(tmp), "%Y/%m/%d %X %A ", localtime(&t));//ƴ��������������
        strcpy(buf, "����ʱ��:");
        strcat(buf, tmp);
        strcat(buf, "\n������:");
        strcat(buf, name);
        strcat(buf, "\n��Ϣ:");
        //strcat(buf, "\n");
        strcat(buf, buf1);
        cout << buf << endl;
        //sockConnΪ����ͨ�ŵ��׽��֣�bufΪ�������ݵĻ�������strlen(buf)+1Ϊ�������ݵĳ��ȣ�0λflags��־
        send(sockConn, buf, strlen(buf) + 1, 0);//������Ϣ
    }
}

void recvFun()
{
    char buf[512];
    while (1)
    {
        int Ret = recv(sockConn, buf, 256, 0);//������Ϣ
        if (Ret < 0) {
            cout<<"\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
            cout<<"�Է����˳�!"<<endl;
            flag = flag % 2 + 1;
            break;
        }
        else if (Ret == 0) {
            cout<<"ERROR_RECV";
        }
        else if(strcmp(buf, "End") == 0) {//���յ�send������End,���������
            cout << "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
            cout << "�Է��ѽ�������" << endl;
            break;
        }
        else{
            cout << "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
            cout << "������Ϣ,��Ϣ����ϸ������:" << endl;
            cout << buf << endl;
            cout << endl;
            cout << name << "��������Ϣ:";
        
        }
    }
}

