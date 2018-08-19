#ifndef MYSOCKET_H
#define MYSOCKET_H

#include <QTcpSocket>
#include "debugonoroff.h"
#include <QFile>


class Mysocket : public QTcpSocket
{
    Q_OBJECT
public:
    Mysocket(int socket,int ID,QObject *parent = 0);
    ~Mysocket();
    /*发送文件*/
    void sendFile(QString path);
    /*发送消息*/
    void sendMSG(QString msg,qint64 cmd);
    /*初始化变量*/
    void clearVariation();
    /*查找要下载的文件*/
    QString findDownloadFile(QString path,QString fileName);
public slots:
    /*数据接收槽函数*/
    void reveiveData();
    /*客户端被动断开连接*/
    void clientDisconnectSlot();
private:
    /*客户端编号*/
    int socketID;
    int socketDescriptor;
    /*数据协议包*/
    struct rcvOrSendDataPack{
        /*共用部分*/
        QFile *localFile;           //存放本地文件
        qint64 totalBytes;          //存放总大小信息
        QString fileName;           //存放文件名
        QByteArray inOrOutBlock;    //数据缓冲区
        /*发送相关*/
        qint64 bytesWritten;        //已经发送数据大小
        qint64 bytesToWrite;        //剩余数据的大小
        qint64 payloadSize;         //每次发送数据的大小
        /*接收相关*/
        qint64 bytesReceived;       //已收到数据的大小
        qint64 fileNameSize;        //文件名的大小信息
        int command;            //接收的命令
    };
    struct rcvOrSendDataPack transferData; //数据结构体
};

#endif // MYSOCKET_H
