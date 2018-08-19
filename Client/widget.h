#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

/*类的前置声明*/
class QFileDialog;
class QFile;
class QTcpSocket;
class QTreeWidgetItem;
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void sleep(int ms);                     //延时函数
    void sendFile(QString path);            //文件头信息发送函数
    void clearVariation();                  //初始化变量
    void diaplayFileListForListWidget();
    //void displayFile(QTreeWidgetItem *parent,int num);                     //递归显示文件列表
private slots:
    void receiveFile();                       //文件接收函数
    void socketDisconnected();                //断开连接槽函数
    void on_ClientConnectPB_clicked();        //请求连接槽函数
    void on_SynFilePtn_clicked();             //文件同步槽函数
    void on_pushButton_clicked();             //上传文件槽函数
    void on_DownloadPB_clicked();             //下载文件按钮

private:
    Ui::Widget *ui;
    QTcpSocket *tcpSocket;          //套接字变量
    QString message;                //存放数据大小信息
    bool flag;                      //是否可以同步目录列表，true可是同步，false不可以
    int SYNFlag;                //请求文件同步标志位
    int DOWNFlag;               //下载文件标志位
    QString tempFileName;
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
        int command;                //发送命令
        /*接收相关*/
        qint64 bytesReceived;       //已收到数据的大小
        qint64 fileNameSize;        //文件名的大小信息
    };
    struct rcvOrSendDataPack transferData; //数据结构体

    struct DirInformation{
        char fileName[260];           //文件名字
        char dirName[260];            //目录名字
        unsigned int isDirectory;  //是否是目录
        unsigned int isFile;       //是否是文件
        unsigned int topDir;       //是否是顶层目录
    };
    struct DirInformation dirInfo;
};

#endif // WIDGET_H
