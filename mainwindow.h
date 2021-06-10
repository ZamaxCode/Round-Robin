#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRegExp>
#include <QMessageBox>
#include <iostream>
#include <QTime>
#include "proceso.h"
#include "pendientprocess.h"
#include "finishprocess.h"
#include <QKeyEvent>
#include <time.h>
#include <resultprocess.h>
#include "blockedprocess.h"

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event);

private slots:
    void on_numOfProcessPB_clicked();

    void on_numOfProcessLE_textChanged(const QString &arg1);

    void on_startPB_clicked();

    void on_finishPB_clicked();

    void on_insertQuantumLE_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    int contOfProcess;
    int quantum;
    int globalCont = 0;
    int contActualProcess = 1;
    bool interFlag=false;
    bool errorFlag=false;
    bool pauseFlag=false;
    bool continueFlag=false;
    bool newFlag=false;
    bool bcpFlag=false;
    bool fisrtBCP=false;

    QList<Proceso> pendientList;
    QList<Proceso> processList;
    QList<Proceso> blockedList;
    QList<Proceso> finishedList;

    void setProcess(const int &totalProcess);
    void startProcess();
    void delay(const int &mSeconds);
    int resolveProcess(const int &d1, const int &d2, const char op);
    void printResults();
    void setBCP();
};
#endif // MAINWINDOW_H
