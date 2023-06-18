
#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include "heightindicatorwidget.h"

#include <QWidget>

QT_BEGIN_NAMESPACE
class QNetworkDatagram;
class QUdpSocket;
class QLabel;
QT_END_NAMESPACE

class UdpClient : public QWidget
{
    Q_OBJECT

public:
    explicit UdpClient(QWidget* parent = nullptr);

protected:
    virtual void closeEvent(QCloseEvent* event) override;

private slots:
    void readPendingDatagrams();

private:
    void closeSocket();
    void readConnectionData();
    void bindAddress();
    void sendMessageToServer();

private:
    HeightIndicatorWidget* m_heightIndicatorWidget;

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

#endif // UDPCLIENT_H
