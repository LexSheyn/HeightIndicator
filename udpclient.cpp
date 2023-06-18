
#include "udpclient.h"

#include "data.h"

#include <QtWidgets>
#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QHostAddress>
#include <QDebug>

#include <fstream>
#include <sstream>

UdpClient::UdpClient(QWidget* parent)
    :QWidget(parent)
{
    m_heightIndicatorWidget = new HeightIndicatorWidget();

    readConnectionData();

    m_socket = new QUdpSocket(this);

    connect(m_socket, &QUdpSocket::readyRead, this, &UdpClient::readPendingDatagrams);

    m_connectionLabel = new QLabel(tr("Соединение с сервером:"));
    m_connectionStatusLabel = new QLabel(tr("Нет"));

    m_connectionLabel->setBuddy(m_connectionStatusLabel);

    m_heightLabel = new QLabel(tr("Текущая высота:"));
    m_heightValueLabel = new QLabel(QString::number(m_heightIndicatorWidget->getHeight()));
    m_heightUnitLabel = new QLabel(tr("м"));

    m_heightLabel->setBuddy(m_heightValueLabel);
    m_heightValueLabel->setBuddy(m_heightUnitLabel);

    QGridLayout* gridLayout = new QGridLayout();

    gridLayout->addWidget(m_connectionLabel, 0, 0, 1, 1);
    gridLayout->addWidget(m_connectionStatusLabel, 0, 1, 1, 1);
    gridLayout->addWidget(m_heightLabel, 1, 0, 1, 1);
    gridLayout->addWidget(m_heightValueLabel, 1, 1, 1, 1);
    gridLayout->addWidget(m_heightUnitLabel, 1, 2, 1, 1);
    gridLayout->addWidget(m_heightIndicatorWidget, 2, 0, 10, 10);

    setLayout(gridLayout);

    setWindowTitle(tr("Клиент"));
    resize(m_heightIndicatorWidget->width(), m_heightIndicatorWidget->height());
    move(200, 200);

    bindAddress();

    m_timer = new QTimer(this);

    connect(m_timer, &QTimer::timeout, this, &UdpClient::sendMessageToServer);

    //m_timer->start(2000); // 2 Hz
    m_timer->start(200);
}

void UdpClient::closeEvent(QCloseEvent *event)
{
    closeSocket();
}

void UdpClient::readPendingDatagrams()
{
    bool connectedToServer = false;

    while (m_socket->hasPendingDatagrams())
    {
        QByteArray datagram;

        datagram.resize(m_socket->pendingDatagramSize());

        QHostAddress sender;
        quint16 senderPort;

        m_socket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        if (!datagram.isEmpty())
        {
            Message1* message = (Message1*)datagram.data();

            if (message->header == UDP_HEADER_SERVER)
            {
                qDebug() << "Client received message";

                connectedToServer = true;

                m_heightIndicatorWidget->setHeight((qint32)message->height);
                m_heightValueLabel->setText(QString::number(m_heightIndicatorWidget->getHeight()));
            }
        }
    }

    if (connectedToServer)
    {
        m_connectionStatusLabel->setText(tr("Да"));
    }
    else
    {
        m_connectionStatusLabel->setText(tr("Нет"));
    }
}

void UdpClient::closeSocket()
{
    m_timer->stop();

    m_socket->close();
}

void UdpClient::readConnectionData()
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

    qDebug() << "Client: " << UDP_TOKEN_ADDRESS << '\t' << hostAddress;
    qDebug() << "Client: " << UDP_TOKEN_PORT << '\t' << m_clientPort;

    m_address = QString(hostAddress.c_str());
}

void UdpClient::bindAddress()
{
    bool binded = m_socket->bind(QHostAddress(m_address), m_clientPort);

    if (binded)
    {
        qDebug() << "Client: " << "Bound socket to [" << m_address << "][" << m_clientPort << "]";
    }
    else
    {
        qDebug() << "Client: " << "Failed to bind socket to [" << m_address << "][" << m_clientPort << "]";
    }
}

void UdpClient::sendMessageToServer()
{
    //qDebug() << "Client sent message";

    Message2 message;

    QByteArray data;

    data.append((const char*)&message, sizeof(Message2));

    m_socket->writeDatagram(data, QHostAddress(m_address), m_serverPort);
}

