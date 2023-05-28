#include "WeSocketClient.h"

WeSocketClient::WeSocketClient(QObject *parent)
    : QObject{parent}
{

}

void WeSocketClient::sendMessage(const QString &message)
{
    if(connectedstate)
        m_webSocket.sendTextMessage(message);
}
void WeSocketClient::open()
{
    m_webSocket.open(QUrl("ws://192.168.4.1/webcontrol"));
    connect(&m_webSocket, &QWebSocket::connected, this, &WeSocketClient::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &WeSocketClient::onDisconnected);
}
void WeSocketClient::close()
{
    m_webSocket.close();
    connectedstate = false;
}

void WeSocketClient::flush()
{
    m_webSocket.flush();
    connectedstate = false;
}


void WeSocketClient::onConnected()
{
   connectedstate = true;
   emit SendConnectedState(true);
   connect(&m_webSocket, &QWebSocket::textMessageReceived, this, &WeSocketClient::onTextMessageReceived);
}

void WeSocketClient::onDisconnected()
{
    connectedstate = false;
    emit SendConnectedState(false);
    disconnect(&m_webSocket, &QWebSocket::textMessageReceived, this, &WeSocketClient::onTextMessageReceived);
}

void WeSocketClient::onTextMessageReceived(const QString &message)
{
    this->message = message;
    emit SendReceiveMsg(message);
}

WeSocketClient::~WeSocketClient()
{

}
