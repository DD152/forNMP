#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QSpinBox>
#include <QDialog>
#include <QTextBrowser>
#include <QtCharts>

using namespace QtCharts;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void hideFile();
    void showFile();
    void hideInterface();
    void showInterface();
    bool isFileExist(const QString &fileFullName);
    bool createGraph(QString Gcol,QLineSeries *Gvalue);
    void clearGraph(QLayout *layout);

private slots:
    void on_comboBox_currentIndexChanged(const QString &arg1);
    void selectFile();
    bool fileStart();
    bool interfaceStart();

    void on_actionNetwork_Latency_triggered();

    void on_actionApplication_Latency_triggered();

    void on_actionnDPI_triggered();

    void on_actionTCP_triggered();

    void on_actionDNS_triggered();

    void on_actionGraph_triggered();

    void on_actionHTTP_User_Agent_triggered();

    void on_actionHTTP_Packet_triggered();

private:
    Ui::MainWindow *ui;

    QString curFile;
    QString interface;
    int time;

    QPushButton *fileBtn;
    QLineEdit *fileLine;
    QPushButton *fileStartBtn;

    QLabel *interfaceLable;
    QLineEdit *interfaceLine;
    QPushButton *interfaceStartBtn;
    QLabel *interfaceTimeLable;
    QSpinBox *interfaceSpinBox;

    QDialog *NetworkLatencyDlg;
    QDialog *ApplicationLatencyDlg;
    QDialog *ndpiDlg;
    QDialog *TCPDlg;
    QDialog *DNSDlg;
    QDialog *HTTPDlg;
    QDialog *HTTPUADlg;
    QDialog *GraphDlg;
    QTextBrowser *NetworkLatencyText;
    QTextBrowser *ApplicationLatencyText;
    QTextBrowser *ndpiText;
    QTextBrowser *TCPText;
    QTextBrowser *DNSText;
    QTextBrowser *HTTPText;
    QTextBrowser *HTTPUAText;

    QWidget  *graph;
    QGridLayout *graphLayout;
    QScrollArea *Garea;
};
#endif // MAINWINDOW_H
