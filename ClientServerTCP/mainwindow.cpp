#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lineEdit->installEventFilter(this);
    ui->pushButton_3->setDisabled(true);
    ui->pushButton_4->setDisabled(true);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete clientTCP;
}

Client::Client(QObject * parent)
{
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(socket, SIGNAL(disconnect()), this, SLOT(disconnected()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(error(QAbstractSocket::SocketError)));

    connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this, SIGNAL(stateChanged(QAbstractSocket::SocketState)));

}

Client::~Client()
{
    socket->close();
}


//tworzenie ramki xml
QByteArray Client::drawXmlFile(const QString & textToSend)
{
    QByteArray data;
    QXmlStreamWriter stream(&data);
    stream.setAutoFormatting(true);

    stream.writeStartDocument();
    stream.writeStartElement("frame");
    stream.writeStartElement("message");
    stream.writeAttribute("type", "txt");
    stream.writeTextElement("context", textToSend);
    stream.writeEndElement();
    stream.writeEndDocument();
    return data;

}

void Client::sendXmlFile(const QByteArray &xmlString)
{
    socket->write(xmlString);
}

void Client::connected(const QString &getIp, const QString &getPort)
{
    qint16 intPort;
    QByteArray bytePort;
    bytePort.append(getPort);
    intPort = getPort.toInt();

    socket->connectToHost(getIp, intPort);
}

void Client::readyRead()
{
   QString msgString;
   QByteArray msgByte;
   msgByte = socket->readAll();
   msgString = msgByte;
   resultReady(msgString);
}


void Client::disconnected()
{
    socket->disconnectFromHost();
    if (socket->state() == QAbstractSocket::UnconnectedState
            || socket->waitForDisconnected(5000))
    {
        socket->close();
        resultReady("Rozłączono pomyślnie");
    }

}

void Client::error(QAbstractSocket::SocketError socketError)
{
    switch (socketError)
    {
        case QAbstractSocket::RemoteHostClosedError:
            break;
        case QAbstractSocket::NetworkError:
            resultReady("Network Error");
            break;
        case QAbstractSocket::HostNotFoundError:
            resultReady("Nie znaleziono hosta");
            break;
        case QAbstractSocket::ConnectionRefusedError:
            resultReady("Komunikacja przerwana, serwer nie odpowiada");
            break;
        default:
           resultReady("Inny nieznany rodzaj błędu");
     }
}

void MainWindow::socketState(QAbstractSocket::SocketState socketState)
{

    switch (socketState)
    {
        case QAbstractSocket::UnconnectedState:
            ui->pushButton_3->setDisabled(true);
            ui->pushButton_4->setDisabled(true);
            break;
        case QAbstractSocket::ConnectedState:
            ui->pushButton_4->setEnabled(true);
            ui->pushButton_3->setEnabled(true);
            break;
        case QAbstractSocket::ClosingState:
            on_textBrowser_windowIconTextChanged("Utracono połączenie z serwerem.");
            break;
        defualt:
            ui->pushButton_4->setEnabled(true);
            ui->pushButton_3->setEnabled(true);

    }
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    // wpisywanie i pobieranie danych z wejscia - wiadomość
}


void MainWindow::on_lineEdit_returnPressed()
{
    //Enter
    QByteArray get_string;
    QString getString;
    QString getIp;
    QString getPort;


    getString = ui->lineEdit->text();

    if (getString.isEmpty())
    {
        on_textBrowser_windowIconTextChanged("Pusta wiadomość!\nWpisz treść w polu tekstowym");

    }
    else
    {
        QByteArray frame = clientTCP->drawXmlFile(getString);
        clientTCP->sendXmlFile(frame);
    }
}



void MainWindow::on_pushButton_2_clicked()
{
    this->close();
}

void MainWindow::on_pushButton_clicked()
{
    //Button Połącz
    QString getIp;
    QString getPort;

    clientTCP = new Client();


    connect(clientTCP, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
        this, SLOT(socketState(QAbstractSocket::SocketState)));



    connect(clientTCP, &Client::resultReady, this, &MainWindow::on_textBrowser_windowIconTextChanged);

    getIp = ui->lineEdit_2->text();
    getPort = ui->lineEdit_3->text();


    if (getPort.isEmpty() || getIp.isEmpty())
    {
        on_textBrowser_windowIconTextChanged("Błąd!\nNie podano adresu IP lub numeru portu!");
        ui->pushButton_3->setDisabled(true);
        ui->pushButton_4->setDisabled(true);
    }
    else
    {
        clientTCP->connected(getIp, getPort);
        ui->pushButton_4->setEnabled(true);
        ui->pushButton_3->setEnabled(true);
    }
}

void MainWindow::on_textBrowser_windowIconTextChanged(const QString &iconText)
{
    ui->textBrowser->append(iconText);
}

bool MainWindow::eventFilter(QObject * obj, QEvent * event)
{
    if (obj == ui->lineEdit && event->type() == Qt::Key_Enter)
    {
        QKeyEvent * event;
        if(Qt::Key_Enter == event->key()){qDebug(".");}
    }
}


//ip i port - pobranie wartości i ustawienie jej w konstruktorze clienta;

void MainWindow::on_lineEdit_2_textChanged(const QString &arg1)
{
    //wczytywanie i pobieranie danych z wejścia - numer ip
}

void MainWindow::on_lineEdit_3_textChanged(const QString &arg1)
{
    //wczytywanie i pobieranie danych z wejścia - port
}


void MainWindow::on_pushButton_3_clicked()
{
    clientTCP->disconnected();
    ui->pushButton_3->setDisabled(true);
    ui->pushButton_4->setDisabled(true);
}

void MainWindow::on_pushButton_4_clicked()
{
    //Button wyślij
    QString getString;
    getString = ui->lineEdit->text();
    if (getString.isEmpty())
        on_textBrowser_windowIconTextChanged("Pusta wiadomość!\nWpisz treść w polu tekstowym");
    else
    {
        QByteArray frame = clientTCP->drawXmlFile(getString);
        clientTCP->sendXmlFile(frame);
    }

}
