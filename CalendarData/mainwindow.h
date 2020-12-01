#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <DWidget>
#include <DMainWindow>
#include <QNetworkReply>
#include <QMap>
DWIDGET_BEGIN_NAMESPACE
typedef  struct Cl{
    QList<int>Year;
    QMap<int,int>monthOfYear;

}Calendar;
class MainWindow : public DMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

signals:

public slots:
    void slotReplyrequest(QNetworkReply * reply);
};
DWIDGET_END_NAMESPACE
#endif // MAINWINDOW_H
