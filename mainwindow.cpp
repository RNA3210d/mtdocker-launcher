#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QProcess>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <QString>
#include <unistd.h>
#include <ios>
#include <fstream>
#include <string>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
using namespace std;
  QObject *parent;
  QProcess* ping_process = new QProcess(parent);

std::string com1()
{
    string cmd="docker ps";
    string data;
    FILE * stream;
    const int max_buffer = 256;
    char buffer[max_buffer];
    cmd.append(" 2>&1");

    stream = popen(cmd.c_str(), "r");
    if (stream) {
      while (!feof(stream))
        if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
      pclose(stream);
    }
  cout << "LS: " << data << endl;

    const std::string s = data;
    return s;
}



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    const QString s1 = QString::fromStdString( com1() );
    ui -> textBrowser_2->append(s1);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    system("notify-send 'Methlab - Launcher' \"Starting container in VNC mode...\"");
    connect(ping_process, &QProcess::readyReadStandardOutput, [=] {
        ui->textBrowser->append(ping_process->readAllStandardOutput());
    });
    ui->statusbar->showMessage("Container started...");
    ping_process->QProcess::start("docker", QStringList() << "run -it --rm -p 5901:5901 -p 6080:6080 --shm-size=2G mathworks/matlab:r2022a -vnc");
}


void MainWindow::on_pushButton_3_clicked()
{
    ping_process->terminate();
}

