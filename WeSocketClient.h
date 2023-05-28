#ifndef WESOCKETCLIENT_H
#define WESOCKETCLIENT_H

#include <QObject>
#include <QWebSocket>

class WeSocketClient : public QObject
{
    Q_OBJECT
public:
    explicit WeSocketClient(QObject *parent = nullptr);
    ~WeSocketClient();
    bool connectedstate = false;
    void sendMessage(const QString &message);//发送信息函数
    void open();
    void close();
    void flush();
    QString message;

private slots:
    void onConnected();//连接槽函数
    void onDisconnected();//断开连接槽函数
    void onTextMessageReceived(const QString &message);//接收信息槽函数

private:
    QWebSocket m_webSocket;
signals:
    void SendConnectedState(bool state);
    void SendReceiveMsg(const QString &message);

};

#endif // WESOCKETCLIENT_H
