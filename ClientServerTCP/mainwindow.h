#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTcpServer>
#include <QKeyEvent>
#include <QFile>
#include <QtXml/QtXml>
#include <QXmlSimpleReader>
#include <QTimer>


namespace Ui {
class MainWindow;
}


class Client : public QMainWindow
{
    Q_OBJECT

    public:
        Client(QObject * parent = nullptr);
        ~Client();
        QByteArray drawXmlFile(const QString & textToSend);
        void sendXmlFile(const QByteArray &get_String);
    public slots:
        void connected(const QString &getIp, const QString &getPort);
        void readyRead();
        void disconnected();
        void error(QAbstractSocket::SocketError socketError);
    signals:
        void resultReady(const QString &s);
        void stateChanged(QAbstractSocket::SocketState socketState);
    private:
        QTcpSocket * socket = nullptr;
        QByteArray get_String;
        QFile xmlFile;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    protected:
        bool eventFilter(QObject * obj, QEvent * event);
    private slots:
        void on_lineEdit_textChanged(const QString &arg1);
        void on_lineEdit_returnPressed();
        void on_pushButton_2_clicked();
        void on_pushButton_clicked();
        void on_textBrowser_windowIconTextChanged(const QString &iconText);
        void on_lineEdit_2_textChanged(const QString &arg1);
        void on_lineEdit_3_textChanged(const QString &arg1);
        void on_pushButton_3_clicked();
        void on_pushButton_4_clicked();
        void socketState(QAbstractSocket::SocketState socketState);


    private:
        Ui::MainWindow *ui;
        Client * clientTCP;
        QTcpSocket * socket = nullptr;
        QKeyEvent * event = nullptr;
};





#endif // MAINWINDOW_H
