
#include "udpserver.h"

#include "data.h"

#include <QtWidgets>
#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QHostAddress>
#include <QDebug>

#include <fstream>
#include <sstream>

UdpServer::UdpServer(QWidget* parent)
    : QWidget(parent)
{
    m_heightSlider = new QSlider(Qt::Horizontal);

    m_heightSlider->setMinimum(0);
    m_heightSlider->setMaximum(9999);

    readConnectionData();

    m_socket = new QUdpSocket(this);

    connect(m_socket, &QUdpSocket::readyRead, this, &UdpServer::readPendingDatagrams);

    m_connectionLabel = new QLabel(tr("Соединение с клиентом:"));
    m_connectionStatusLabel = new QLabel(tr("Нет"));

    m_connectionLabel->setBuddy(m_connectionStatusLabel);

    m_heightLabel = new QLabel(tr("Текущая высота:"));
    m_heightValueLabel = new QLabel(QString::number(m_heightSlider->value()));
    m_heightUnitLabel = new QLabel(tr("м"));

    m_heightLabel->setBuddy(m_heightValueLabel);
    m_heightValueLabel->setBuddy(m_heightUnitLabel);

    connect(m_heightSlider, &QAbstractSlider::valueChanged, this, &UdpServer::setHeightValueLabelText);

    QGridLayout* gridLayout = new QGridLayout();

    gridLayout->addWidget(m_connectionLabel, 0, 0, 1, 1);
    gridLayout->addWidget(m_connectionStatusLabel, 0, 1, 1, 1);
    gridLayout->addWidget(m_heightLabel, 1, 0, 1, 1);
    gridLayout->addWidget(m_heightValueLabel, 1, 1, 1, 1);
    gridLayout->addWidget(m_heightUnitLabel, 1, 2, 1, 1);
    gridLayout->addWidget(m_heightSlider, 2, 0, 10, 10);

    setLayout(gridLayout);

    setWindowTitle(tr("Сервер"));
    resize(450, 80);
    move(450 + 200 + 100, 200);

    bindAddress();

    m_timer = new QTimer(this);

    connect(m_timer, &QTimer::timeout, this, &UdpServer::sendMessageToClient);

    //m_timer->start(25000); // 25 Hz
    m_timer->start(2500);
}

void UdpServer::closeEvent(QCloseEvent *event)
{
    closeSocket();
}

void UdpServer::readPendingDatagrams()
{
    bool connectedToClient = false;

    while (m_socket->hasPendingDatagrams())
    {
        QByteArray datagram;

        datagram.resize(m_socket->pendingDatagramSize());

        QHostAddress sender;
        quint16 senderPort;

        m_socket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        if (!datagram.isEmpty())
        {
            Message2* message = (Message2*)datagram.data();

            if (message->header == UDP_HEADER_CLIENT)
            {
                qDebug() << "Server received message";

                connectedToClient = true;;
            }
        }
    }

    if (connectedToClient)
    {
        m_connectionStatusLabel->setText(tr("Да"));
    }
    else
    {
        m_connectionStatusLabel->setText(tr("Нет"));
    }
}

void UdpServer::setHeightValueLabelText(qint32 value)
{
    m_heightValueLabel->setText(QString::number(value));
}

void UdpServer::closeSocket()
{
    m_timer->stop();

    m_socket->close();
}

void UdpServer::readConnectionData()
{
    QString directory = QDir::currentPath();

    std::string filePath = directory.toStdString() + '/' + "settings.ini";

    std::ifstream inputFileStream;
    std::istringstream inputStringStream;
    std::string line;
    std::string firstToken;
    std::string secondToken;
    std::string hostAddress;

    inputFileStream.open(filePath);

    if (!inputFileStream.is_open())
    {
        qDebug() << "Failed to open file: " << filePath;
    }

    while (std::getline(inputFileStream, line))
    {
        inputStringStream.clear();
        inputStringStream.str(line);
        inputStringStream >> firstToken;

        if (firstToken == UDP_TOKEN_ADDRESS)
        {
            inputStringStream >> hostAddress;
        }
        else if (firstToken == UDP_TOKEN_PORT)
        {
            inputStringStream >> secondToken;

            if (secondToken == UDP_TOKEN_SERVER)
            {
                inputStringStream >> m_serverPort;
            }
            else if (secondToken == UDP_TOKEN_CLIENT)
            {
                inputStringStream >> m_clientPort;
            }
        }
    }

    inputFileStream.close();

    qDebug() << "Server: " << UDP_TOKEN_ADDRESS << '\t' << hostAddress;
    qDebug() << "Server: " << UDP_TOKEN_PORT << '\t' << m_serverPort;

    m_address = QString(hostAddress.c_str());
}

void UdpServer::bindAddress()
{
    bool binded = m_socket->bind(QHostAddress(m_address), m_serverPort);

    if (binded)
    {
        qDebug() << "Server: " << "Bound socket to [" << m_address << "][" << m_serverPort << "]";
    }
    else
    {
        qDebug() << "Server: " << "Failed to bind socket to [" << m_address << "][" << m_serverPort << "]";
    }
}

void UdpServer::sendMessageToClient()
{
    //qDebug() << "Server sent message";

    Message1 message;

    message.height = (qint16)m_heightSlider->value();

    QByteArray data;

    data.append((const char*)&message, sizeof(Message1));

    m_socket->writeDatagram(data, QHostAddress(m_address), m_clientPort);
}

