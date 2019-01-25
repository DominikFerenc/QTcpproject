#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete serverTCP;
}

Server::Server(QObject * parent)
{

    server = new QTcpServer(this);
    qDebug() << "Server: " << server;
    if (server->listen(QHostAddress::Any, 1024))
        connect(server, SIGNAL(newConnection()), this, SLOT(acceptNewConnection()));

}

Server::~Server()
{
    server->close();
}

void Server::acceptNewConnection()
{
    if (server->hasPendingConnections())
    {
        socket = server->nextPendingConnection();
        connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    }

    _sockets.push_back(socket);



    for (QTcpSocket* socket : _sockets) {
        socket->write(QByteArray::fromStdString(socket->localAddress().toString().toStdString() + " Połączono z serwerem"));
    }
}



void Server::readyRead()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(QObject::sender());
    QByteArray dataXml;

    if (socket == nullptr)
        return;


    dataXml = socket->readAll();

    if (!dataXml.isEmpty())
    {
        result_Read("Wiadomość jest dostępna");
        socket->write("Serwer odebrał wiadomość");
        _sockets.removeAll(socket);
    }

    QXmlStreamReader xml(dataXml);


    while (xml.readNextStartElement())
    {
        if (xml.name() == "frame")
        {
            while (xml.readNextStartElement())
            {
                if (xml.name() == "message")
                {
                    while (xml.readNextStartElement())
                    {
                        if (xml.name() == "context")
                        {
                            QString xmlToString = xml.readElementText();
                            resultRead(xmlToString);
                        }
                    }
                }
            }
        }
    }
}

void MainWindow::on_pushButton_clicked()
{
    serverTCP = new Server();

    on_textBrowser_2_windowIconTextChanged("Uruchomiono");

    connect(serverTCP, &Server::resultRead, this, &MainWindow::on_textBrowser_windowIconTextChanged);
    connect(serverTCP, &Server::result_Read, this, &MainWindow::on_textBrowser_2_windowIconTextChanged);
}

void MainWindow::on_pushButton_2_clicked()
{
    this->close();
}

void MainWindow::on_textBrowser_windowIconTextChanged(const QString &iconText)
{
    ui->textBrowser->append(iconText);
}

void MainWindow::on_textBrowser_2_windowIconTextChanged(const QString &iconText)
{
    ui->textBrowser_2->append(iconText);
}
