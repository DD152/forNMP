#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <QFile>
#include <QScreen>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(QString("NPM"));
    move(QGuiApplication::screens().at(0)->geometry().center() - frameGeometry().center());
    ui->comboBox->setCurrentIndex(-1);
    curFile="";
    interface="";
    time=0;
    //select file
    fileBtn=new QPushButton(QString("Select File"));
    fileLine=new QLineEdit ();
    fileStartBtn=new QPushButton (QString("Start"));
    ui->horizontalLayout_2->addWidget(fileBtn);
    ui->horizontalLayout_2->addWidget(fileLine);
    ui->verticalLayout->addWidget(fileStartBtn);
    connect(fileBtn,SIGNAL(clicked()),this,SLOT(selectFile()));
    connect(fileStartBtn,SIGNAL(clicked()),this,SLOT(fileStart()));
    fileBtn->hide();
    fileLine->hide();
    fileStartBtn->hide();

    //select interface
    interfaceLable=new QLabel (QString("Interface"));
    interfaceLine=new QLineEdit();
    interfaceLine->setMaximumSize(55,25);
    interfaceTimeLable=new QLabel (QString("duration(s)"));
    interfaceSpinBox=new QSpinBox ();
    interfaceSpinBox->setMaximum(99999);
    interfaceStartBtn=new QPushButton (QString("Start"));
    ui->horizontalLayout_2->addWidget(interfaceLable);
    ui->horizontalLayout_2->addWidget(interfaceLine);
    ui->horizontalLayout_2->addWidget(interfaceTimeLable);
    ui->horizontalLayout_2->addWidget(interfaceSpinBox);
    ui->verticalLayout->addWidget(interfaceStartBtn);
    connect(interfaceStartBtn,SIGNAL(clicked()),this,SLOT(interfaceStart()));
    interfaceLable->hide();
    interfaceLine->hide();
    interfaceTimeLable->hide();
    interfaceSpinBox->hide();
    interfaceStartBtn->hide();

    //Network & Application
    NetworkLatencyDlg=new QDialog ();
    ApplicationLatencyDlg=new QDialog ();
    ndpiDlg=new QDialog ();
    TCPDlg=new QDialog();
    DNSDlg=new QDialog();
    GraphDlg=new QDialog();
    HTTPDlg=new QDialog();
    HTTPUADlg=new QDialog();

    QSize size(600,371);
    NetworkLatencyDlg->resize(size);
    ApplicationLatencyDlg->resize(size);
    ndpiDlg->resize(size);
    TCPDlg->resize(size);
    DNSDlg->resize(size);
    HTTPDlg->resize(800,471);
    HTTPUADlg->resize(size);
    GraphDlg->resize(1630,810);

    NetworkLatencyText=new QTextBrowser (NetworkLatencyDlg);
    ApplicationLatencyText=new QTextBrowser (ApplicationLatencyDlg);
    ndpiText=new QTextBrowser (ndpiDlg);
    TCPText=new QTextBrowser (TCPDlg);
    DNSText=new QTextBrowser (DNSDlg);
    HTTPText=new QTextBrowser (HTTPDlg);
    HTTPUAText=new QTextBrowser (HTTPUADlg);

    NetworkLatencyDlg->setWindowTitle(QString("Network Latency"));
    ApplicationLatencyDlg->setWindowTitle(QString("Application Latency"));
    ndpiDlg->setWindowTitle(QString("nDPI"));
    TCPDlg->setWindowTitle(QString("TCP"));
    DNSDlg->setWindowTitle(QString("DNS"));
    GraphDlg->setWindowTitle(QString("Graph"));
    HTTPDlg->setWindowTitle(QString("HTTP Packet"));
    HTTPUADlg->setWindowTitle(QString("HTTP User Agent"));

    QGridLayout *networkLayout=new QGridLayout(NetworkLatencyDlg);
    QGridLayout *applicationLayout=new QGridLayout(ApplicationLatencyDlg);
    QGridLayout *ndpiLayout=new QGridLayout(ndpiDlg);
    QGridLayout *TCPLayout=new QGridLayout(TCPDlg);
    QGridLayout *DNSLayout=new QGridLayout(DNSDlg);
    QGridLayout *HTTPLayout=new QGridLayout(HTTPDlg);
    QGridLayout *HTTPUALayout=new QGridLayout(HTTPUADlg);

    networkLayout->addWidget(NetworkLatencyText);
    applicationLayout->addWidget(ApplicationLatencyText);
    ndpiLayout->addWidget(ndpiText);
    TCPLayout->addWidget(TCPText);
    DNSLayout->addWidget(DNSText);
    HTTPLayout->addWidget(HTTPText);
    HTTPUALayout->addWidget(HTTPUAText);

    //graph
    Garea=new QScrollArea(GraphDlg);
    Garea->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
    Garea->setWidgetResizable( true );
    Garea->setGeometry( 10, 10, 1600, 800 );
    graph=new QWidget ();
    Garea->setWidget(graph);
    graphLayout=new QGridLayout();
    graph->setLayout(graphLayout);
    graphLayout->setSizeConstraint(QLayout::SetMinimumSize);
    }

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::hideFile()
{
    fileBtn->hide();
    fileLine->hide();
    fileStartBtn->hide();

}

void MainWindow::showFile()
{
    fileBtn->show();
    fileLine->show();
    fileStartBtn->show();
}

void MainWindow::hideInterface()
{
    interfaceLable->hide();
    interfaceLine->hide();
    interfaceTimeLable->hide();
    interfaceSpinBox->hide();
    interfaceStartBtn->hide();
}

void MainWindow::showInterface()
{
    interfaceLable->show();
    interfaceLine->show();
    interfaceTimeLable->show();
    interfaceSpinBox->show();
    interfaceStartBtn->show();
}


bool MainWindow::isFileExist(const QString &fileFullName)
{
    QFileInfo fileInfo(fileFullName);
    if(fileInfo.isFile())
    {
        qDebug()<<"true";
        return true;
    }
    return false;
}


void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
    curFile="";
    interface="";
    time=0;

    if(arg1==QString("File")){
        hideInterface();
        showFile();
    }
    else if(arg1==QString("Interface")){
        hideFile();
        showInterface();
        interfaceLine->setText(interface);
        interfaceSpinBox->setValue(time);
    }
}

void MainWindow::selectFile()
{
    QString fileName=QFileDialog::getOpenFileName(this,"Open","/home/ddan/QtProject",QString("Wireshark files (*.pcap *.pcapng)"));
    curFile=fileName;
    fileLine->setText(curFile);

}

//v1
/*
bool MainWindow::fileStart()
{
    NetworkLatencyText->clear();
    ApplicationLatencyText->clear();
    ndpiText->clear();
    TCPText->clear();
    DNSText->clear();
    HTTPUAText->clear();
    HTTPText->clear();
    clearGraph(graphLayout);
    ui->textBrowser->clear();
    qApp->processEvents();
    if(isFileExist(fileLine->text())){
        std::string  cc = std::string("tshark -i ndpi -q -z http,tree ")
                //tcp
                +std::string("-z io,stat,1,\"AVG(tcp.analysis.ack_rtt)tcp.analysis.ack_rtt\" ")
                +std::string("-z io,stat,1,\"COUNT(tcp.analysis.retransmission) tcp.analysis.retransmission\" ")
                +std::string("-z io,stat,1,\"COUNT(tcp.analysis.fast_retransmission) tcp.analysis.fast_retransmission\" ")
                +std::string("-z io,stat,1,\"COUNT(tcp.analysis.duplicate_ack) tcp.analysis.duplicate_ack\" ")
                +std::string("-z io,stat,1,\"COUNT(tcp.analysis.lost_segment) tcp.analysis.lost_segment\" ")
                +std::string("-z io,stat,1,\"MIN(tcp.window_size)tcp.window_size\" ")
                +std::string("-z io,stat,1,\"COUNT(tcp.analysis.bytes_in_flight)tcp.analysis.bytes_in_flight\" ")
                +std::string("-z io,stat,1,\"COUNT(tcp.connection.synack)tcp.connection.synack\" ")
                //udp
                +std::string("-z io,stat,1,\"MAX(udp.pdu.size)udp.pdu.size\" ")
                //http
                +std::string("-z io,stat,1,\"COUNT(http.request)http.request\" ")
                +std::string("-z io,stat,1,\"COUNT(http.response)http.response\" ")
                //ntp
                +std::string("-z io,stat,0.01,\"AVG(ntp.delta_time)ntp.delta_time\" ")

                +std::string("-o extcap.ndpi.i:")
                +std::string((fileLine->text()).toLocal8Bit().data())
                +std::string(" -o extcap.ndpi.9:-1");
        char const *command=cc.c_str();
        std::cout<<command;
        std::cout.flush();
        QFile popenFile;

        if((popenFile.open(popen(command, "r"), QIODevice::ReadOnly)))
        {
            QTextStream in(&popenFile);
            QString s=QString("Iuput file %1 \n\n").arg(fileLine->text());
            while(!in.atEnd())
            {
                if(s.contains(QString("=========="))){
                    s=in.readLine();
                }

                else if(s.contains(QString("| IO Statistics "))){
                    int x=0;
                    int interval;
                    QString col;
                    QLineSeries  *value = new QLineSeries();
                    qDebug()<<"come in";
                    while(!s.contains("| Interval:")){
                        s=in.readLine();
                    }
                    interval=s.midRef(12,s.indexOf(QString("secs"))-12).toInt();
                    qDebug()<<interval;
                    while(!s.contains("| Col 1:")){
                        s=in.readLine();
                    }
                    col=s.mid(s.indexOf(QString("("))+1,s.indexOf(QString(")"))-s.indexOf(QString("("))-1);
                    qDebug()<<col;
                    while(!s.contains("| Interval")){
                        s=in.readLine();
                    }
                    s=in.readLine();
                    s=in.readLine();
                    while(!s.contains("==========")){
                        value ->append(x,s.split(QLatin1Char('|'), Qt::SkipEmptyParts)[1].toFloat());
                        x+=interval;
                        s=in.readLine();
                    }
                    s=in.readLine();
                    createGraph(col,value);
                }

                else if(s==QString("HTTP/Packet Counter:")){
                    s=in.readLine();
                    while(s!=QString("Network Latency")){
                        HTTPText->insertPlainText(s+QString("\n"));
                        s=in.readLine();
                    }
                }

                else if(s==QString("Network Latency")){
                    s=in.readLine();
                    while(s!=QString("Application Latency")){
                        NetworkLatencyText->insertPlainText(s+QString("\n"));
                        s=in.readLine();
                    }
                }
                else if(s==QString("Application Latency")){
                    s=in.readLine();
                    while(s!=QString("nDPI")){
                        ApplicationLatencyText->insertPlainText(s+QString("\n"));
                        s=in.readLine();
                    }
                }
                else if (s==QString("nDPI")) {
                    s=in.readLine();
                    while(s!=QString("tcp_information")){
                        ndpiText->insertPlainText(s+QString("\n"));
                        s=in.readLine();
                    }
                }
                else if (s==QString("tcp_information")) {
                    s=in.readLine();
                    while(s!=QString("DNS Server")){
                        TCPText->insertPlainText(s+QString("\n"));
                        s=in.readLine();
                    }
                }
                else if (s==QString("DNS Server")) {
                    s=in.readLine();
                    while(s!=QString("httpua")){
                        DNSText->insertPlainText(s+QString("\n"));
                        s=in.readLine();
                    }
                }
                else if (s==QString("httpua")) {
                    s=in.readLine();
                    while(!in.atEnd()){
                        HTTPUAText->insertPlainText(s+QString("\n"));
                        s=in.readLine();
                    }
                }
                else{
                    ui->textBrowser->insertPlainText(s+QString("\n"));
                    qApp->processEvents();
                    s=in.readLine();
                }

            }
            ui->textBrowser->moveCursor(QTextCursor::End);
            qApp->processEvents();
        }
        popenFile.close();
        return true;
    }
    QMessageBox::warning(this,QString("Error"),QString("%1 doesn't exist!").arg(fileLine->text()));
    return false;
}
*/
/*
bool MainWindow::interfaceStart()
{
    NetworkLatencyText->clear();
    ApplicationLatencyText->clear();
    ndpiText->clear();
    TCPText->clear();
    DNSText->clear();
    HTTPUAText->clear();
    clearGraph(graphLayout);
    ui->textBrowser->clear();
    qApp->processEvents();
    if(interfaceLine->text()!=QString("") && interfaceSpinBox->text()!="0"){
        ui->textBrowser->moveCursor(QTextCursor::End);
        qApp->processEvents();
        std::string  cc = std::string("echo \"fgh152..\" | sudo -S tshark -i ndpi -z http,tree ")
                //tcp
                +std::string("-z io,stat,1,\"AVG(tcp.analysis.ack_rtt)tcp.analysis.ack_rtt\" ")
                +std::string("-z io,stat,1,\"COUNT(tcp.analysis.retransmission) tcp.analysis.retransmission\" ")
                +std::string("-z io,stat,1,\"COUNT(tcp.analysis.fast_retransmission) tcp.analysis.fast_retransmission\" ")
                +std::string("-z io,stat,1,\"COUNT(tcp.analysis.duplicate_ack) tcp.analysis.duplicate_ack\" ")
                +std::string("-z io,stat,1,\"COUNT(tcp.analysis.lost_segment) tcp.analysis.lost_segment\" ")
                +std::string("-z io,stat,1,\"MIN(tcp.window_size)tcp.window_size\" ")
                +std::string("-z io,stat,1,\"COUNT(tcp.analysis.bytes_in_flight)tcp.analysis.bytes_in_flight\" ")
                +std::string("-z io,stat,1,\"COUNT(tcp.connection.synack)tcp.connection.synack\" ")
                //udp
                +std::string("-z io,stat,1,\"MAX(udp.pdu.size)udp.pdu.size\" ")
                //http
                +std::string("-z io,stat,1,\"COUNT(http.request)http.request\" ")
                +std::string("-z io,stat,1,\"COUNT(http.response)http.response\" ")
                //ntp
                +std::string("-z io,stat,1,\"MAX(ntp.delta_time)ntp.delta_time\" ")
                +std::string("-o extcap.ndpi.i:")
                +std::string((interfaceLine->text()).toLocal8Bit().data())
                +std::string(" -o extcap.ndpi.9:-1 -a duration:")
                +std::string(interfaceSpinBox->text().toLocal8Bit().data());

        char const *command=cc.c_str();
        std::cout<<command;
        std::cout.flush();
        QFile popenFile;

        if((popenFile.open(popen(command, "r"), QIODevice::ReadOnly)))
        {
            QTextStream in(&popenFile);
            QString s=QString("Starting capture interface %1 \n\n").arg(interfaceLine->text());
            ui->textBrowser->insertPlainText(s+QString("\n"));
            qApp->processEvents();
            s="";
            while(!in.atEnd())
            {
                if(s.contains(QString("=========="))){
                    s=in.readLine();
                }

                else if(s.contains(QString("| IO Statistics "))){
                    int x=0;
                    int interval;
                    QString col;
                    QLineSeries  *value = new QLineSeries();
                    qDebug()<<"come in";
                    while(!s.contains("| Interval:")){
                        s=in.readLine();
                    }
                    interval=s.midRef(12,s.indexOf(QString("secs"))-12).toInt();
                    qDebug()<<interval;
                    while(!s.contains("| Col 1:")){
                        s=in.readLine();
                    }
                    col=s.mid(s.indexOf(QString("("))+1,s.indexOf(QString(")"))-s.indexOf(QString("("))-1);
                    qDebug()<<col;
                    while(!s.contains("| Interval")){
                        s=in.readLine();
                    }
                    s=in.readLine();
                    s=in.readLine();
                    while(!s.contains("==========")){
                        value ->append(x,s.split(QLatin1Char('|'), Qt::SkipEmptyParts)[1].toFloat());
                        x+=interval;
                        s=in.readLine();
                    }
                    s=in.readLine();
                    createGraph(col,value);
                }

                else if(s==QString("HTTP/Packet Counter:")){
                    s=in.readLine();
                    while(s!=QString("Network Latency")){
                        HTTPText->insertPlainText(s+QString("\n"));
                        s=in.readLine();
                    }
                }

                else if(s==QString("Network Latency")){
                    s=in.readLine();
                    while(s!=QString("Application Latency")){
                        NetworkLatencyText->insertPlainText(s+QString("\n"));
                        s=in.readLine();
                    }
                }
                else if(s==QString("Application Latency")){
                    s=in.readLine();
                    while(s!=QString("nDPI")){
                        ApplicationLatencyText->insertPlainText(s+QString("\n"));
                        s=in.readLine();
                    }
                }
                else if (s==QString("nDPI")) {
                    s=in.readLine();
                    while(s!=QString("tcp_information")){
                        ndpiText->insertPlainText(s+QString("\n"));
                        s=in.readLine();
                    }
                }
                else if (s==QString("tcp_information")) {
                    s=in.readLine();
                    while(s!=QString("DNS Server")){
                        TCPText->insertPlainText(s+QString("\n"));
                        s=in.readLine();
                    }
                }
                else if (s==QString("DNS Server")) {
                    s=in.readLine();
                    while(s!=QString("httpua")){
                        DNSText->insertPlainText(s+QString("\n"));
                        s=in.readLine();
                    }
                }
                else if (s==QString("httpua")) {
                    s=in.readLine();
                    while(!in.atEnd()){
                        HTTPUAText->insertPlainText(s+QString("\n"));
                        s=in.readLine();
                    }
                }
                else{
                    ui->textBrowser->insertPlainText(s+QString("\n"));
                    s=in.readLine();
                }

            }
            ui->textBrowser->moveCursor(QTextCursor::End);
            qApp->processEvents();
        }
        popenFile.close();
        return true;
    }
    QMessageBox::warning(this,QString("Error"),QString("Interface %1 doesn't exist or time is 0!").arg(interfaceLine->text()));
    return false;

}

*/

//v2
bool MainWindow::fileStart(){
    NetworkLatencyText->clear();
    ApplicationLatencyText->clear();
    ndpiText->clear();
    TCPText->clear();
    DNSText->clear();
    HTTPUAText->clear();
    HTTPText->clear();
    clearGraph(graphLayout);
    ui->textBrowser->clear();
    qApp->processEvents();
    if(isFileExist(fileLine->text())){
        std::string  cc = std::string("tshark -i ndpi  -z http,tree ")
                //tcp
                +std::string("-z io,stat,1,\"AVG(tcp.analysis.ack_rtt)tcp.analysis.ack_rtt\" ")
                +std::string("-z io,stat,1,\"COUNT(tcp.analysis.retransmission) tcp.analysis.retransmission\" ")
                +std::string("-z io,stat,1,\"COUNT(tcp.analysis.fast_retransmission) tcp.analysis.fast_retransmission\" ")
                +std::string("-z io,stat,1,\"COUNT(tcp.analysis.duplicate_ack) tcp.analysis.duplicate_ack\" ")
                +std::string("-z io,stat,1,\"COUNT(tcp.analysis.lost_segment) tcp.analysis.lost_segment\" ")
                +std::string("-z io,stat,1,\"MIN(tcp.window_size)tcp.window_size\" ")
                +std::string("-z io,stat,1,\"COUNT(tcp.analysis.bytes_in_flight)tcp.analysis.bytes_in_flight\" ")
                +std::string("-z io,stat,1,\"COUNT(tcp.connection.synack)tcp.connection.synack\" ")
                //udp
                +std::string("-z io,stat,1,\"MAX(udp.pdu.size)udp.pdu.size\" ")
                //http
                +std::string("-z io,stat,1,\"COUNT(http.request)http.request\" ")
                +std::string("-z io,stat,1,\"COUNT(http.response)http.response\" ")
                //ntp
                +std::string("-z io,stat,0.01,\"AVG(ntp.delta_time)ntp.delta_time\" ")

                +std::string("-o extcap.ndpi.i:")
                +std::string((fileLine->text()).toLocal8Bit().data())
                +std::string(" -o extcap.ndpi.9:-1");
        char const *command=cc.c_str();
        std::cout<<command;
        std::cout.flush();
        QFile popenFile;

        if((popenFile.open(popen(command, "r"), QIODevice::ReadOnly)))
        {
            //QTextStream in(&popenFile);
            QString s=QString("Iuput file %1 \n\n").arg(fileLine->text());
            while(!popenFile.atEnd())
            {
                if(s.contains(QString("=========="))){
                    s=popenFile.readLine();
                }

                else if(s.contains(QString("| IO Statistics "))){
                    int x=0;
                    int interval;
                    QString col;
                    QLineSeries  *value = new QLineSeries();
                    qDebug()<<"come in";
                    while(!s.contains("| Interval:")){
                        s=popenFile.readLine();
                    }
                    interval=s.midRef(12,s.indexOf(QString("secs"))-12).toInt();
                    qDebug()<<interval;
                    while(!s.contains("| Col 1:")){
                        s=popenFile.readLine();
                    }
                    col=s.mid(s.indexOf(QString("("))+1,s.indexOf(QString(")"))-s.indexOf(QString("("))-1);
                    qDebug()<<col;
                    while(!s.contains("| Interval")){
                        s=popenFile.readLine();
                    }
                    s=popenFile.readLine();
                    s=popenFile.readLine();
                    while(!s.contains("==========")){
                        value ->append(x,s.split(QLatin1Char('|'), Qt::SkipEmptyParts)[1].toFloat());
                        x+=interval;
                        s=popenFile.readLine();
                    }
                    s=popenFile.readLine();
                    createGraph(col,value);
                }

                else if(s==(QString("HTTP/Packet Counter:\n"))){
                    s=popenFile.readLine();
                    while(s!=(QString("Network Latency\n"))){
                        HTTPText->insertPlainText(s);
                        s=popenFile.readLine();
                    }
                }

                else if(s==(QString("Network Latency\n"))){
                    s=popenFile.readLine();
                    while(s!=(QString("Application Latency\n"))){
                        NetworkLatencyText->insertPlainText(s);
                        s=popenFile.readLine();
                    }
                }
                else if(s==(QString("Application Latency\n"))){
                    s=popenFile.readLine();
                    while(s!=(QString("nDPI_v2\n"))){
                        ApplicationLatencyText->insertPlainText(s);
                        s=popenFile.readLine();
                    }
                }
                else if (s==(QString("nDPI_v2\n"))) {
                    s=popenFile.readLine();
                    while(s!=(QString("tcp_information\n"))){
                        ndpiText->insertPlainText(s);
                        s=popenFile.readLine();
                    }
                }
                else if (s==(QString("tcp_information\n"))) {
                    s=popenFile.readLine();
                    while(s!=(QString("DNS Server_v2\n"))){
                        TCPText->insertPlainText(s);
                        s=popenFile.readLine();
                    }
                }
                else if (s==(QString("DNS Server_v2\n"))) {
                    s=popenFile.readLine();
                    while(s!=(QString("httpua\n"))){
                        DNSText->insertPlainText(s);
                        s=popenFile.readLine();
                    }
                }
                else if (s==(QString("httpua\n"))) {
                    s=popenFile.readLine();
                    while(!popenFile.atEnd()){
                        HTTPUAText->insertPlainText(s);
                        s=popenFile.readLine();
                    }
                }
                else{
                    ui->textBrowser->insertPlainText(s);
                    ui->textBrowser->moveCursor(QTextCursor::End);
                    qApp->processEvents();
                    s=popenFile.readLine();
                }

            }
            ui->textBrowser->moveCursor(QTextCursor::End);
            qApp->processEvents();
        }
        popenFile.close();
        return true;
    }
    QMessageBox::warning(this,QString("Error"),QString("%1 doesn't exist!").arg(fileLine->text()));
    return false;
}

bool MainWindow::interfaceStart()
{
    NetworkLatencyText->clear();
    ApplicationLatencyText->clear();
    ndpiText->clear();
    TCPText->clear();
    DNSText->clear();
    HTTPUAText->clear();
    clearGraph(graphLayout);
    ui->textBrowser->clear();
    qApp->processEvents();
    if(interfaceLine->text()!=QString("") && interfaceSpinBox->text()!="0"){
        ui->textBrowser->moveCursor(QTextCursor::End);
        qApp->processEvents();
        std::string  cc = std::string("echo \"fgh152..\" | sudo -S tshark -i ndpi -z http,tree ")
                //tcp
                +std::string("-z io,stat,1,\"AVG(tcp.analysis.ack_rtt)tcp.analysis.ack_rtt\" ")
                +std::string("-z io,stat,1,\"COUNT(tcp.analysis.retransmission) tcp.analysis.retransmission\" ")
                +std::string("-z io,stat,1,\"COUNT(tcp.analysis.fast_retransmission) tcp.analysis.fast_retransmission\" ")
                +std::string("-z io,stat,1,\"COUNT(tcp.analysis.duplicate_ack) tcp.analysis.duplicate_ack\" ")
                +std::string("-z io,stat,1,\"COUNT(tcp.analysis.lost_segment) tcp.analysis.lost_segment\" ")
                +std::string("-z io,stat,1,\"MIN(tcp.window_size)tcp.window_size\" ")
                +std::string("-z io,stat,1,\"COUNT(tcp.analysis.bytes_in_flight)tcp.analysis.bytes_in_flight\" ")
                +std::string("-z io,stat,1,\"COUNT(tcp.connection.synack)tcp.connection.synack\" ")
                //udp
                +std::string("-z io,stat,1,\"MAX(udp.pdu.size)udp.pdu.size\" ")
                //http
                +std::string("-z io,stat,1,\"COUNT(http.request)http.request\" ")
                +std::string("-z io,stat,1,\"COUNT(http.response)http.response\" ")
                //ntp
                +std::string("-z io,stat,1,\"MAX(ntp.delta_time)ntp.delta_time\" ")
                +std::string("-o extcap.ndpi.i:")
                +std::string((interfaceLine->text()).toLocal8Bit().data())
                +std::string(" -o extcap.ndpi.9:-1 -a duration:")
                +std::string(interfaceSpinBox->text().toLocal8Bit().data());

        char const *command=cc.c_str();
        std::cout<<command;
        std::cout.flush();
        QFile popenFile;

        if((popenFile.open(popen(command, "r"), QIODevice::ReadOnly)))
        {
            //QTextStream in(&popenFile);
            QString s=QString("Starting capture interface %1 \n\n").arg(interfaceLine->text());
            ui->textBrowser->insertPlainText(s);
            qApp->processEvents();
            s="";
            while(!popenFile.atEnd())
            {
                if(s.contains(QString("=========="))){
                    s=popenFile.readLine();
                }

                else if(s.contains(QString("| IO Statistics "))){
                    int x=0;
                    int interval;
                    QString col;
                    QLineSeries  *value = new QLineSeries();
                    qDebug()<<"come in";
                    while(!s.contains("| Interval:")){
                        s=popenFile.readLine();
                    }
                    interval=s.midRef(12,s.indexOf(QString("secs"))-12).toInt();
                    qDebug()<<interval;
                    while(!s.contains("| Col 1:")){
                        s=popenFile.readLine();
                    }
                    col=s.mid(s.indexOf(QString("("))+1,s.indexOf(QString(")"))-s.indexOf(QString("("))-1);
                    qDebug()<<col;
                    while(!s.contains("| Interval")){
                        s=popenFile.readLine();
                    }
                    s=popenFile.readLine();
                    s=popenFile.readLine();
                    while(!s.contains("==========")){
                        value ->append(x,s.split(QLatin1Char('|'), Qt::SkipEmptyParts)[1].toFloat());
                        x+=interval;
                        s=popenFile.readLine();
                    }
                    s=popenFile.readLine();
                    createGraph(col,value);
                }

                else if(s==QString("HTTP/Packet Counter:\n")){
                    s=popenFile.readLine();
                    while(s!=QString("Network Latency\n")){
                        HTTPText->insertPlainText(s);
                        s=popenFile.readLine();
                    }
                }

                else if(s==QString("Network Latency\n")){
                    s=popenFile.readLine();
                    while(s!=QString("Application Latency\n")){
                        NetworkLatencyText->insertPlainText(s);
                        s=popenFile.readLine();
                    }
                }
                else if(s==QString("Application Latency\n")){
                    s=popenFile.readLine();
                    while(s!=QString("nDPI_v2\n")){
                        ApplicationLatencyText->insertPlainText(s);
                        s=popenFile.readLine();
                    }
                }
                else if (s==QString("nDPI_v2\n")) {
                    s=popenFile.readLine();
                    while(s!=QString("tcp_information\n")){
                        ndpiText->insertPlainText(s);
                        s=popenFile.readLine();
                    }
                }
                else if (s==QString("tcp_information\n")) {
                    s=popenFile.readLine();
                    while(s!=QString("DNS Server_v2\n")){
                        TCPText->insertPlainText(s);
                        s=popenFile.readLine();
                    }
                }
                else if (s==QString("DNS Server_v2\n")) {
                    s=popenFile.readLine();
                    while(s!=QString("httpua\n")){
                        DNSText->insertPlainText(s);
                        s=popenFile.readLine();
                    }
                }
                else if (s==QString("httpua\n")) {
                    s=popenFile.readLine();
                    while(!popenFile.atEnd()){
                        HTTPUAText->insertPlainText(s);
                        s=popenFile.readLine();
                    }
                }
                else{
                    ui->textBrowser->insertPlainText(s);
                    ui->textBrowser->moveCursor(QTextCursor::End);
                    qApp->processEvents();
                    s=popenFile.readLine();
                }

            }
            ui->textBrowser->moveCursor(QTextCursor::End);
            qApp->processEvents();
        }
        popenFile.close();
        return true;
    }
    QMessageBox::warning(this,QString("Error"),QString("Interface %1 doesn't exist or time is 0!").arg(interfaceLine->text()));
    return false;

}

bool MainWindow::createGraph(QString Gcol,QLineSeries *Gvalue){

    QChart *chart=new QChart();
    chart->setTitle(Gcol);
    QChartView *cv =new QChartView(chart);
    chart->legend()->hide();
    chart->addSeries(Gvalue);
    chart->createDefaultAxes();
    cv->setRenderHint(QPainter::Antialiasing);
    cv->setFixedSize(800,570);
    graphLayout->addWidget(cv);
    return true;
}

void MainWindow::clearGraph(QLayout *layout){

        QLayoutItem *item;
        while((item = layout->takeAt(0))) {
            if (item->layout()) {
                clearGraph(item->layout());
                delete item->layout();
            }
            if (item->widget()) {
               delete item->widget();
            }
            delete item;
        }

}

void MainWindow::on_actionNetwork_Latency_triggered()
{
    NetworkLatencyDlg->show();
}

void MainWindow::on_actionApplication_Latency_triggered()
{
    ApplicationLatencyDlg->show();
}

void MainWindow::on_actionnDPI_triggered()
{
    ndpiDlg->show();
}

void MainWindow::on_actionTCP_triggered()
{
    TCPDlg->show();
}

void MainWindow::on_actionDNS_triggered()
{
    DNSDlg->show();
}

void MainWindow::on_actionGraph_triggered()
{
    GraphDlg->show();
}

void MainWindow::on_actionHTTP_User_Agent_triggered()
{
    HTTPUADlg->show();
}

void MainWindow::on_actionHTTP_Packet_triggered()
{
    HTTPDlg->show();
}
