
#include "udpserver.h"
#include "udpclient.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);

    UdpServer server;
    UdpClient client;

    server.show();
    client.show();

    return application.exec();
}
