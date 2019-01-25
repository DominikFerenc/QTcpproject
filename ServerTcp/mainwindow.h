#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTcpServer>
#include <QtXml/QtXml>
namespace Ui {
class MainWindow;
}

class Server : public QObject
{
    Q_OBJECT

    public:
        Server(QObject * parent = nullptr);
        ~Server();

    public slots:
        void readyRead();
        void acceptNewConnection();
    signals:
        void resultRead(const QString &s);
        void result_Read(const QString &s);
    private:
        QTcpSocket * socket;
        QTcpServer * server;
        QList<QTcpSocket*>  _sockets;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    private slots:
        void on_pushButton_clicked();
        void on_pushButton_2_clicked();
        void on_textBrowser_windowIconTextChanged(const QString &iconText);
        void on_textBrowser_2_windowIconTextChanged(const QString &iconText);

    private:
        Ui::MainWindow *ui;
        Server * serverTCP;
};




#endif // MAINWINDOW_H
