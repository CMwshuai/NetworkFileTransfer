#include "myserver.h"

Myserver::Myserver(QObject *parent)
{
    socketNum = 0;
    /*获得当前widget对象*/
    widget = dynamic_cast<Widget *>(parent);
}

Myserver::~Myserver()
{

}

void Myserver::incomingConnection(qintptr socketDescriptor)
{
#if defined __DEBUG__
    qDebug()<<"Enter incomingConnection funcation!";
#endif
    /*添加连接的客户端到链表中*/
    socketList.append(socketDescriptor);
#if defined __DEBUG__
    qDebug()<<"socketDescriptor is"<<socketDescriptor;
#endif
    /*每建立一个客户端请求客户端数量加一*/
    socketNum++;
    if(socketNum>3)
        socketNum = 1;
#if defined __DEBUG__
    qDebug()<<"客户端编号为"<<socketNum;
#endif
    /*创建一个线程用于执行与客户端的工作*/
    Mythread *thread = new Mythread(socketDescriptor,socketNum,0);
    threadList.append(thread);
    /*关联线程断开客户端连接信号和GUI更新界面槽函数*/
    connect(thread,&Mythread::closeClientConnectSignal,widget,&Widget::closeClientConnectSlot);
    /*添加客户端IP到GUI中进行显示*/
    connect(thread,&Mythread::addClientIPToGUISignal,widget,&Widget::addClientIPTGUISlot);
    /*获得文件列表信号和槽关联*/
    connect(widget,&Widget::getFileListSignal,thread,&Mythread::getFileListSlot);
    /*发送文件信号发送至线程*/
    connect(widget,&Widget::sendFileSignal,thread,&Mythread::sendFileSlot);
    /*服务器端主动断开连接*/
    connect(widget,&Widget::clientDiconnectSignal,this,&Myserver::clientDisconnectSlot);
    /*启动线程*/
    thread->start();
}

void Myserver::clientDisconnectSlot(int ID)
{
    /*服务器端主动断开客户端的连接根据断开的客户端编号与其线程关联槽函数*/
    QObject::connect(this,&Myserver::clientDisconnectSignal,
                     threadList[ID-1],&Mythread::clientDisconnectSlot);
    emit clientDisconnectSignal();
}
