
#ifndef UDPSERVER_H
#define UDPSERVER_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QNetworkDatagram;
class QUdpSocket;
class QLabel;
class QSlider;
QT_END_NAMESPACE

class UdpServer : public QWidget
{
    Q_OBJECT

public:
    explicit UdpServer(QWidget* parent = nullptr);

protected:
    virtual void closeEvent(QCloseEvent* event) override;

private slots:
    void readPendingDatagrams();
    void setHeightValueLabelText(qint32 value);

private:
    void closeSocket();
    void readConnectionData();
    void bindAddress();
    void sendMessageToClient();

private:
    QSlider* m_heightSlider;

    QUdpSocket* m_socket;
    QString m_address;
    quint16 m_serverPort;
    quint16 m_clientPort;

    QLabel* m_connectionLabel;
    QLabel* m_connectionStatusLabel;
    QLabel* m_heightLabel;
    QLabel* m_heightValueLabel;
    QLabel* m_heightUnitLabel;

    QTimer* m_timer;
};

#endif // UDPSERVER_H
