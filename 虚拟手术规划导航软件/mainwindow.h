#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QTcpServer;//前向声明
class QTcpSocket;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_5_clicked();
    void test();
	void on_pushButton_4_clicked();
	void test2();
	void on_pushButton_3_clicked();
	void test3();
	void test5();
	void on_pushButton_2_clicked();
	void test4();
	void on_pushButton_6_clicked();
	void brain1();
	void on_pushButton_7_clicked();
	void brain2();
	void on_pushButton_8_clicked();
	void brain3();
	void on_pushButton_9_clicked();
	void on_pushButton_clicked();
	void brain4();
	void load();
	void change1();
	void change2();

	void SocketConnet();
	void Listen();
	void SocketReceive();
	void conn();

private:
    Ui::MainWindow *ui;
	QString statusText; //状态信息
	QTcpServer *tcpServer; //服务器
	QTcpSocket *clientTcpSocket; //客户端socket
	void SocketSend(QString sendStr);

	/*int dist;*/


	//float legdata[2][6];
};
#endif // MAINWINDOW_H
