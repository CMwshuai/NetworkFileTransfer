#include "widget.h"
#include "ui_widget.h"
#include <QFileInfo>
#include <QTextStream>
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QEvent>
#include <stdio.h>
#include <string.h>
#include <io.h>
#include <iostream>
//#include "seekfile.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle(tr("局域网文件传输服务器"));
    /*清空列表文件*/
    if((fp = fopen("./FileList/FILELIST.TXT","w+"))==NULL)
    {
        qDebug()<<"Clear FILELIST.TXT error!";
    }
    fclose(fp);
    server = new Myserver(this);
    server->listen(QHostAddress::Any, 8888);
    closeCommand = 0;

    ui->ClientPB1->setEnabled(false);
    ui->ClientPB2->setEnabled(false);
    ui->ClientPB3->setEnabled(false);
    /*为三个按键安装事件过滤器*/
    ui->ClientPB1->installEventFilter(this);
    ui->ClientPB2->installEventFilter(this);
    ui->ClientPB3->installEventFilter(this);

    ui->LBstate1->setText(tr("断开"));
    ui->LBstate2->setText(tr("断开"));
    ui->LBstate3->setText(tr("断开"));

    connect(ui->pushButton,&QPushButton::clicked,this,&Widget::fileTranferButtonSlot);
    connect(ui->AddFilePB,&QPushButton::clicked,this,&Widget::addFilePBSlot);
    connect(ui->DelFilePB,&QPushButton::clicked,this,&Widget::deleteFileListItem);
    connect(ui->listWidget,&QAbstractItemView::doubleClicked,this,&Widget::doubleClickedItem);
}

Widget::~Widget()
{
    delete ui;
}



bool Widget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->ClientPB1)
    {
        if(event->type() == QEvent::MouseButtonRelease)
        {
            emit clientDiconnectSignal(1);
            ui->LBDisplayIP1->clear();
            ui->LBstate1->setText(tr("断开"));
            ui->ClientPB1->setEnabled(false);
        }
    }
    else if(watched == ui->ClientPB2)
    {
        if(event->type() == QEvent::MouseButtonRelease)
        {
            emit clientDiconnectSignal(2);
            ui->LBDisplayIP2->clear();
            ui->LBstate2->setText(tr("断开"));
            ui->ClientPB2->setEnabled(false);
        }
    }
    else if(watched == ui->ClientPB3)
    {
        if(event->type() == QEvent::MouseButtonRelease)
        {
            emit clientDiconnectSignal(3);
            ui->LBDisplayIP3->clear();
            ui->LBstate3->setText(tr("断开"));
            ui->ClientPB3->setEnabled(false);
        }
    }
}

void Widget::closeClientConnectSlot(int ID)
{
    /*判断是哪个客户端断开，并做相应的清除工作*/
    if(ID == 1)
    {
        ui->LBDisplayIP1->clear();
        ui->LBstate1->setText(tr("断开"));
        ui->ClientPB1->setEnabled(false);
    }
    else if(ID == 2)
    {
        ui->LBDisplayIP2->clear();;
        ui->LBstate2->setText(tr("断开"));
        ui->ClientPB2->setEnabled(false);
    }
    else if(ID == 3)
    {
        ui->LBDisplayIP3->clear();
        ui->LBstate3->setText(tr("断开"));
        ui->ClientPB3->setEnabled(false);
    }
}

void Widget::addFilePBSlot()
{
    /*打开目录获得文件目录*/
    dirPath = QFileDialog::getExistingDirectory(this,0,"./");
    QDir dir(dirPath);
#if defined __DEBUG__
    qDebug()<<"Open dir path"<<dirPath;
#endif
    if(dirPath.isEmpty())
        return;
//    /*得到目录名、标志为顶层目录*/
//    strcpy(dirInfo.dirName,dir.dirName().toLatin1());
//    dirInfo.topDir = 1;
//    strcpy(dirInfo.fileName," ");
//    dirInfo.isDirectory = 1;
//    dirInfo.isFile = 0;
//    fp = fopen("./FileList/FILELIST.TXT","ab+");
//    if(fp == NULL)
//    {
//        qDebug()<<"Open FILELIST.TXT failed!";
//        return;
//    }
//    fwrite(&dirInfo,sizeof(struct DirInformation),1,fp);
#if defined __DEBUG__
      qDebug()<<"Open directory name is :"<<dir.dirName();
#endif
      this->getSendFileList(dirPath);
    /*获取目录名字，并添加到ListWidget中*/
    ui->listWidget->addItem(dir.dirName());
    //std::string str = std::string((const char *)dirPath.toLocal8Bit());
    /*遍历目录以及子目录中的文件*/
    //this->DfsFolder(str,0);
//    fclose(fp);
}
void Widget::getSendFileList(QString path)
{
//    FILE *fp;
//    /*已追加的方式打开文件*/
//    fp = fopen("./FileList/FILELIST.TXT","at+");
//    if(fp == NULL)
//    {
//        qDebug()<<"Open FILELIST.TXT failed!";
//        return;
//    }
    QFile filelist("./FileList/FILELIST.TXT");
    QTextStream out(&filelist);
    if(!filelist.open(QFile::WriteOnly|QFile::Text))
    {
        qDebug()<<"write FILELIST.TXT error!"<<endl;
        return;
    }
    QDir dir(path);
    QStringList str;
    str << "*";
    QFileInfoList files = dir.entryInfoList((QStringList)"*",
                                            QDir::Files|QDir::Dirs,QDir::DirsFirst);
    for(int i=0;i<files.count();i++)        //把目录中的文件名写入filelist.txt文件中发送至客户端
    {
        QFileInfo tmpFileInfo = files.at(i);
        QString fileName = tmpFileInfo.fileName();
        if(fileName=="."||fileName=="..")   //过滤掉“.”和“..”目录
            continue;
        if(tmpFileInfo.isFile())
            out << fileName << endl;
        else
            continue;
//        if(tmpFileInfo.isDir())
//        {
//            ISDir();
//            strcpy(dirInfo.dirName,tmpFileInfo.fileName().toLatin1());
//        }
//        else if(tmpFileInfo.isFile())
//        {
//            ISFile();
//            strcpy(dirInfo.fileName,tmpFileInfo.fileName().toLatin1());
//        }
#ifdef __DEBUG__
        qDebug()<<tmpFileInfo.fileName();
#endif
//        if(dirInfo.isDirectory == 1)
//        {
//            /*获得的条目写入文件中*/
//            fwrite(&dirInfo,sizeof(struct DirInformation),1,fp);
//            QDir dir(tmpFileInfo.absoluteDir());
//        }
        /*获得的条目写入文件中*/
        //fwrite(&dirInfo,sizeof(struct DirInformation),1,fp);
     }
    //fclose(fp);
    filelist.close();
}

/*遍历当前目录下的文件夹和文件,默认是按字母顺序遍历*/
//bool Widget::TraverseFiles(std::string path,int &file_num)
//{
//    _finddata_t file_info;
//    std::string current_path=path+"/*.*"; //可以定义后面的后缀为*.exe，*.txt等来查找特定后缀的文件，*.*是通配符，匹配所有类型,路径连接符最好是左斜杠/，可跨平台
//    /*打开文件查找句柄*/
//    int handle=_findfirst(current_path.c_str(),&file_info);
//    /*返回值为-1则查找失败*/
//    if(-1==handle)
//        return false;
//    do
//    {
//        /*判断是否子目录*/
//        std::string attribute;
//        if(file_info.attrib==_A_SUBDIR) //是目录
//        {
//            if(!(strcmp(file_info.name,"."))||!(strcmp(file_info.name,"..")))
//                continue;
//            ISDir();
//            strcpy(dirInfo.dirName,file_info.name);
//#ifdef __DEBUG__
//           qDebug()<<dirInfo.dirName<<endl;
//#endif
//            fwrite(&dirInfo,sizeof(struct DirInformation),1,fp);
//        }
//        else
//        {
//            ISFile();
//            strcpy(dirInfo.fileName,file_info.name);
//#ifdef __DEBUG__
//            std::cout<<dirInfo.fileName;
//#endif
//            fwrite(&dirInfo,sizeof(struct DirInformation),1,fp);
//        }
//        /*输出文件信息并计数,文件名(带后缀)、文件最后修改时间、文件字节数(文件夹显示0)、文件是否目录*/
//        std::cout<<file_info.name<<' '<<file_info.time_write<<' '<<file_info.size<<' '<<attribute<<endl; //获得的最后修改时间是time_t格式的长整型，需要用其他方法转成正常时间显示
//        file_num++;
//    }while(!_findnext(handle,&file_info));  //返回0则遍历完
//    /*关闭文件句柄*/
//    _findclose(handle);
//    return true;
// }
void Widget::deleteFileListItem()
{
    /*获得当前选中项,并删除*/
    QListWidgetItem *item = ui->listWidget->currentItem();
    delete item;
}

void Widget::doubleClickedItem()
{
    /*获得双击项文本*/
    QString str = ui->listWidget->currentItem()->text();
#if defined __DEBUG__
    qDebug()<<"双击项文本内容str为"<<str;
#endif
    QDir dir;
    /*设置路径为添加文件的路径*/
    dir.setPath(dirPath);
    /*切换路径为双击项的路径*/
    dir.cd(str);
    dirPath = dir.absolutePath();
#if defined __DEBUG__
    qDebug()<<"当前文件绝对路径为"<<dirPath;
#endif
    /*更新列表文件内容*/
    this->getSendFileList(dirPath);
//   fp = fopen("./FileList/FILELIST.TXT","ab");
//    if(fp == NULL)
//    {
//        qDebug()<<"Open FILELIST.TXT failed!";
//        return;
//    }
//    this->DfsFolder(dirPath.toStdString(),0);
//    fclose(fp);
    QStringList nameFilters;
    nameFilters << "*";
    QFileInfoList files = dir.entryInfoList(nameFilters,QDir::AllEntries,QDir::DirsFirst);
    ui->listWidget->clear();
    if(str=="..")                       //当返回上一级目录时，只显示目录不显示文件
    {
        for(int i =0 ;i<files.count();i++)
        {
            QFileInfo tmpFileInfo = files.at(i);
            QString fileName = tmpFileInfo.fileName();
            QListWidgetItem *tmp =new QListWidgetItem(fileName);
            if(tmpFileInfo.isDir())         //显示目录
            {
                ui->listWidget->addItem(tmp);
            }
        }
    }
    else
    {
        for(int i =0 ;i<files.count();i++)
        {
            QFileInfo tmpFileInfo = files.at(i);
            QString fileName = tmpFileInfo.fileName();
            QListWidgetItem *tmp =new QListWidgetItem(fileName);
            if(tmpFileInfo.isDir())         //显示目录
            {
                ui->listWidget->addItem(tmp);
            }
            if(tmpFileInfo.isFile())        //显示文件
            {
                ui->listWidget->addItem(tmp);
            }
        }
    }
}

void Widget::fileTranferButtonSlot()
{
    QString fileName = QFileDialog::getOpenFileName(this);
#if defined __DEBUG__
    qDebug()<<"Open file's name is"<<fileName;
#endif
    if(!fileName.isEmpty())
    {
        qDebug()<<"Open file successed!";
    }
    emit sendFileSignal(fileName);
}

void Widget::addClientIPTGUISlot(QString CIP,int ID,int state)
{
    /*判断连接的客户端数量，并根据连接的客户端信息进行界面更新*/
    if((ID == 1)&&(state==QAbstractSocket::ConnectedState))
    {
        ui->LBDisplayIP1->setText(CIP);
        ui->LBstate1->setText(tr("连接"));
        ui->ClientPB1->setEnabled(true);
    }
    else if((ID == 2)&&(state==QAbstractSocket::ConnectedState))
    {
        ui->LBDisplayIP2->setText(CIP);
        ui->LBstate2->setText(tr("连接"));
        ui->ClientPB2->setEnabled(true);
    }
    else if((ID == 3)&&(state==QAbstractSocket::ConnectedState))
    {
        ui->LBDisplayIP3->setText(CIP);
        ui->LBstate3->setText(tr("连接"));
        ui->ClientPB3->setEnabled(true);
    }
}
/*深度优先递归遍历当前目录下文件夹和文件及子文件夹和文件*/
//void Widget::DfsFolder(std::string path,int layer)
//{
//    _finddata_t file_info;
//    std::string current_path=path+"/*.*"; //也可以用/*来匹配所有
//    int handle=_findfirst(current_path.c_str(),&file_info);
//    /*返回值为-1则查找失败*/
//    if(-1==handle)
//    {
//        std::cout<<"cannot match the path"<<endl;
//        return;
//    }
//    do
//    {
//        /*判断是否子目录*/
//        if(file_info.attrib==_A_SUBDIR)
//        {
//            if(!(strcmp(file_info.name,"."))||!(strcmp(file_info.name,"..")))
//                continue;
//            ISDir();
//            /*递归遍历子目录*/
//            strcpy(dirInfo.dirName,file_info.name);
//#ifdef __DEBUG__
//           qDebug()<<dirInfo.dirName<<endl;
//#endif
//            fwrite(&dirInfo,sizeof(struct DirInformation),1,fp);
//            int layer_tmp=layer;
//            if(strcmp(file_info.name,"..")!=0&&strcmp(file_info.name,".")!=0)  //.是当前目录，..是上层目录，必须排除掉这两种情况
//                DfsFolder(path+'/'+file_info.name,layer_tmp+1); //再windows下可以用\\转义分隔符，不推荐
//        }
//        else
//        {
//            ISFile();
//            strcpy(dirInfo.fileName,file_info.name);
//#ifdef __DEBUG__
//            std::cout<<dirInfo.fileName;
//#endif
//            fwrite(&dirInfo,sizeof(struct DirInformation),1,fp);
//        }
//    }while(!_findnext(handle,&file_info));  /*返回0则遍历完*/
//    /*关闭文件句柄*/
//    _findclose(handle);
//}

