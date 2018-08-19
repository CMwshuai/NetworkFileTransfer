#include "mythread.h"
#include <QHostAddress>
#include <QMessageBox>
#include "debugonoroff.h"
#include "protocolcommand.h"
Mythread::Mythread(int socketDes,int ID,QObject *parent):
    QThread(parent), socketDescriptor(socketDes)
{
    clientID = ID;
    socketDescriptor = socketDes;
}

void Mythread::run()
{
    /*保存客户端IP地址*/
    QString clientIP;
    /*保存客户端状态*/
    int clientState;
    /*创建套接字对象*/
    Mysocket *socket = new Mysocket(socketDescriptor,clientID,0);
    /*设置服务器监听到来的客户端的套接字描述符*/
    if(!socket->setSocketDescriptor(socketDescriptor))
        return;

    QObject::connect(socket,&Mysocket::disconnected,this,&Mythread::closeClientConnectSlot);
    QObject::connect(this,&Mythread::sendFileSignal,socket,&Mysocket::sendFile);
    QObject::connect(this,&Mythread::clientDisconnectSignal,socket,&Mysocket::clientDisconnectSlot);
    clientIP = socket->peerAddress().toString();
    clientState = socket->state();
#if defined __DEBUG__
    qDebug()<<"Client IP address is"<<clientIP;
    qDebug()<<"Client State is"<<clientState;
#endif

    if(!clientIP.isEmpty())
    {
        /*只保留32位的点分十进制IP值*/
        clientIP = clientIP.right(clientIP.length()-7);
        /*回传客户端IP地址和ID到GUI显示信号*/
        emit addClientIPToGUISignal(clientIP,clientID,clientState);
    }
    else
    {
        qDebug()<< "Get client IP failed!";
    }

    exec();
}

void Mythread::closeClientConnectSlot()
{
#if defined __DEBUG__
    qDebug()<<"Get Mysocket signal!";
#endif
    /*向GUI发射套接字断开连接信号更新界面*/
    emit closeClientConnectSignal(clientID);
}

void Mythread::clientDisconnectSlot()
{
    emit clientDisconnectSignal();
}

void Mythread::getFileListSlot(QString filelist)
{

}

void Mythread::sendFileSlot(QString filename)
{
#if defined __DEBUG__
    qDebug()<<"Mythread receive file name is"<<filename;
#endif
    emit sendFileSignal(filename);
}
