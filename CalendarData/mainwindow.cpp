#include "mainwindow.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QDebug>
#include <QTextCodec>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonArray>
DWIDGET_USE_NAMESPACE
MainWindow::MainWindow(QWidget *parent) : DMainWindow(parent)
{
  QNetworkRequest m_request;
  int year,momth;
  for (int year=2000;year<=2020;year++) {
      for (int month=1;month<=12;month++) {
        m_request.setUrl(QUrl(QString("https://sp0.baidu.com/8aQDcjqpAAV3otqbppnN2DJv/api.php?query=%1年%2月&co=&resource_id=39043&t=1606725087695&ie=utf8&oe=gbk&cb=op_aladdin_callback&format=json&tn=wisetpl&cb=jQuery1102027574670252801425_1606724608608&_=1606724608627").arg(year).arg(month)));
      }
  }

 // http://opendata.baidu.com/api.php?query=202002&resource_id=6018&format=json
  QNetworkAccessManager *manager=new QNetworkAccessManager(this);
  connect(manager, SIGNAL(finished(QNetworkReply *)),this,SLOT(slotReplyrequest(QNetworkReply *)));
  manager->get(m_request);
}

void MainWindow::slotReplyrequest(QNetworkReply *reply)
{
    QTextCodec *tc=QTextCodec::codecForName("GBK");
    QString replyInfo= tc->toUnicode(reply->readAll());
    qInfo()<<replyInfo;
    QString replyJSon  =replyInfo.left(replyInfo.length()-4).remove(0,replyInfo.indexOf("{"));
    replyJSon.insert(replyJSon.length(),"]");
    replyJSon.insert(replyJSon.length(),"}");
    QJsonParseError jsonParseError;
    QJsonDocument jsonDocument=QJsonDocument::fromJson(replyJSon.toStdString().data(),&jsonParseError);
    qInfo()<<replyJSon.toStdString().data();
    qInfo()<<jsonDocument<<jsonParseError.error;
    if(jsonParseError.error==QJsonParseError::NoError){
        QJsonObject json = jsonDocument.object();
        QStringList keys = json.keys();
        for(int i = 0; i < keys.size(); i++)
         {
            QJsonValue value(keys.at(i));
            QString name=value.toString();
            if(keys.at(i)=="data"){
               QJsonArray dataArray=json[keys.at(i)].toArray();
               QJsonObject dataObject= dataArray.at(0).toObject();
                qInfo()<<dataObject.size()<<dataObject;
               //               qInfo()<<dataObject.size();
//               qInfo()<<dataArray;
               QStringList datakeys=dataObject.keys();
               for (int j=0;j<datakeys.size();j++) {
                QJsonValue dataValue=dataObject.take(datakeys.at(j));
                qInfo()<<dataValue.toString();

                if(datakeys.at(j)=="almanac"){
                     qInfo()<<datakeys.at(j)<<dataObject;
                     QJsonArray subsubArray=dataArray.at(0).toObject()[datakeys.at(j)].toArray();
                    qInfo()<<subsubArray.size();
                 for (int a=0;a<subsubArray.size();a++) {
                    QJsonObject subsubsubObject=subsubArray.at(a).toObject();
                    QStringList subsubsubStr=subsubsubObject.keys();
                    for (int b=0;b<subsubsubStr.size();b++) {
                       QJsonValue subvalue=subsubsubObject.take(subsubsubStr.at(b));
//                       qInfo()<<subvalue.toString();
                    }
                 }
                }
               }
            }

         }
//        for () {

//        }
//        qInfo()<<arraya<<arraya.at(0);

        QString resultcode = json["data.StdStl"].toString();
//        qInfo()<<json;
//        qInfo()<<resultcode;
         }

}




class Worker : public QObject
{
    Q_OBJECT

public slots:
    void doWork(const QString &parameter) {
        QString result;
        /* ... here is the expensive or blocking operation ... */
        emit resultReady(result);
    }

signals:
    void resultReady(const QString &result);
};

class Controller : public QObject
{
    Q_OBJECT
    QThread workerThread;
public:
    Controller() {
        Worker *worker = new Worker;
        worker->moveToThread(&workerThread);
        connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
        connect(this, &Controller::operate, worker, &Worker::doWork);
        connect(worker, &Worker::resultReady, this, &Controller::handleResults);
        workerThread.start();
    }
    ~Controller() {
        workerThread.quit();
        workerThread.wait();
    }
public slots:
    void handleResults(const QString &);
signals:
    void operate(const QString &);
};

