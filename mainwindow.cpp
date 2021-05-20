#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pagesSW->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Funciones

void MainWindow::setProcess(const int &totalProcess)
{
    srand(time(NULL));
    for(int i(0); i<totalProcess; ++i)
    {
        Proceso p;
        int op = rand() % 5;
        switch (op) {
            case 0:
                p.setOperation('+');
            break;
            case 1:
                p.setOperation('-');
            break;
            case 2:
                p.setOperation('*');
            break;
            case 3:
                p.setOperation('/');
            break;
            case 4:
                p.setOperation('%');
            break;
        }

        p.setDigit1(rand() % 100);
        if(op==3 || op==4)
            p.setDigit2(rand() % 99+1);
        else
            p.setDigit2(rand() % 99+1);
        p.setId(processList.size()+1);
        p.setResult(0);
        p.setTimeMax(rand() % 10+6);
        p.setTt(0);
        p.setLlegada(0);
        p.setFinal(0);
        p.setRetorno(0);
        p.setRespuesta(0);
        p.setEspera(0);
        p.setServicio(0);
        p.setFirst(false);
        p.setBlocked(false);
        p.setError(false);
        p.setExec(false);
        p.setContBlocked(5);
        ui->bcpTW->insertRow(processList.size());
        processList.push_back(p);
    }
}

void MainWindow::startProcess()
{
    int topMemory=0, contBlocked=0;

    ui->globalContLB->setText("Contador: "+QString::number(globalCont));
    ui->quantumLB->setText("Quantum: "+QString::number(quantum));

    for(int i(0); i<processList.size(); ++i)
    {
        QLayoutItem* child;
        while((child = ui->pendientProcessGL->takeAt(0))!=0)
        {
            delete child->widget();
        }

        int five=0;
        int col=0;
        for(int j(i); j<processList.size(); ++j)
        {
            if(processList.at(j).getBlocked()==true)
            {
                BlockedProcess* bproc = new BlockedProcess();
                bproc->setBlockedProcess(processList.at(j).getId(), processList.at(j).getContBlocked());
                ui->blockedGL->addWidget(bproc, 0, col,Qt::AlignLeft);
                ++col;
            }
            else
            {
                PendientProcess* pproc = new PendientProcess();
                pproc->setData(processList.at(j).getId(), processList.at(j).getTimeMax(), processList.at(j).getTt());
                ui->pendientProcessGL->addWidget(pproc);

                if(processList.at(j).getFirst()==false)
                {
                    processList[j].setLlegada(globalCont);
                    processList[j].setFirst(true);
                }
            }

            topMemory=j+1;

            ++five;
            if(five==5)
                break;
        }
        ui->contOfBatchesLB->setText("Procesos Pendientes: "+QString::number(contOfProcess));

        if(contBlocked<5 && contBlocked<(processList.size()-i))
        {
            child=ui->pendientProcessGL->takeAt(0);
            delete child->widget();

            if(processList.at(i).getExec()==false)
            {
                processList[i].setRespuesta(globalCont-processList.at(i).getLlegada());
                processList[i].setExec(true);
            }

            int tr=processList.at(i).getTimeMax()-processList.at(i).getTt();

            ui->nameLB->setText("ID: "+QString::number(processList.at(i).getId()));
            ui->opLB->setText("Operacion: "+QString::number(processList.at(i).getDigit1())+processList.at(i).getOperation()+QString::number(processList.at(i).getDigit2()));
            ui->tmLB->setText("Tiempo Maximo: "+QString::number(processList.at(i).getTimeMax()));
            ui->ttLB->setText("Tiempo Transcurrido: "+QString::number(processList.at(i).getTt()));
            ui->trLB->setText("Tiempo Restante: "+QString::number(tr));
            ui->quantumContLB->setText("Quantum Cont: "+QString::number(0));

            int k;
            for(k=0; k<quantum; ++k)
            {
                delay(1000);

                processList[i].setTt(processList.at(i).getTt()+1);
                ui->ttLB->setText("Tiempo Transcurrido: "+QString::number(processList.at(i).getTt()));
                ui->trLB->setText("Tiempo Restante: "+QString::number(--tr));


                ++globalCont;
                ui->globalContLB->setText("Contador: "+QString::number(globalCont));
                ui->quantumContLB->setText("Quantum Cont: "+QString::number(k+1));

                for(int n(i); n<topMemory; ++n)
                {
                    if(processList.at(n).getBlocked())
                    {
                        processList[n].setContBlocked(processList.at(n).getContBlocked()-1);
                        if(processList.at(n).getContBlocked()==0)
                        {
                            processList[n].setContBlocked(5);
                            processList[n].setBlocked(false);
                            PendientProcess* pproc = new PendientProcess();
                            pproc->setData(processList.at(n).getId(), processList.at(n).getTimeMax(), processList.at(n).getTt());
                            ui->pendientProcessGL->addWidget(pproc);
                            --contBlocked;
                        }
                    }
                }

                while((child = ui->blockedGL->takeAt(0))!=0)
                {
                    delete child->widget();
                }
                int column=0;
                for(int n(i); n<topMemory; ++n)
                {
                    if(processList.at(n).getBlocked())
                    {
                        BlockedProcess* bproc = new BlockedProcess();
                        bproc->setBlockedProcess(processList.at(n).getId(), processList.at(n).getContBlocked());
                        ui->blockedGL->addWidget(bproc, 0, column, Qt::AlignLeft);
                        ++column;
                    }
                }

                if(interFlag||errorFlag)
                    break;

                if(pauseFlag)
                {
                    while(true)
                    {
                        delay(100);
                        if(continueFlag)
                            break;
                    }
                    pauseFlag=false;
                    continueFlag=false;
                }

                if(bcpFlag)
                {
                    setBCP();
                    ui->pagesSW->setCurrentIndex(3);
                    while(true)
                    {
                        delay(100);
                        if(continueFlag)
                        {
                            ui->pagesSW->setCurrentIndex(1);
                            break;
                        }
                    }
                    bcpFlag=false;
                    continueFlag=false;
                }

                if(newFlag)
                {
                    setProcess(1);
                    if((processList.size()-i-1)<5)
                    {
                        PendientProcess* pproc = new PendientProcess();
                        pproc->setData(processList.at(processList.size()-1).getId(), processList.at(processList.size()-1).getTimeMax(), processList.at(processList.size()-1).getTt());
                        ui->pendientProcessGL->addWidget(pproc);
                        processList[processList.size()-1].setLlegada(globalCont);
                        processList[processList.size()-1].setFirst(true);
                        processList.move(processList.size()-1,processList.size()-1-contBlocked);
                        ++topMemory;
                    }
                    else
                    {
                        ++contOfProcess;
                        ui->contOfBatchesLB->setText("Procesos Pendientes: "+QString::number(contOfProcess));
                    }
                    newFlag=false;
                }

                if(processList.at(i).getTt()==processList.at(i).getTimeMax())
                    break;
            }

            if(k<quantum)
            {
                if(!interFlag)
                {

                    FinishProcess* fproc = new FinishProcess();
                    if(!errorFlag)
                    {
                        processList[i].setResult(resolveProcess(processList.at(i).getDigit1(), processList.at(i).getDigit2(), processList.at(i).getOperation()));
                        fproc->setData(processList.at(i).getId(), QString::number(processList.at(i).getDigit1())+processList.at(i).getOperation()+QString::number(processList.at(i).getDigit2()), QString::number(processList.at(i).getResult()));
                    }
                    else
                    {
                        fproc->setData(processList.at(i).getId(), QString::number(processList.at(i).getDigit1())+processList.at(i).getOperation()+QString::number(processList.at(i).getDigit2()), "Error");
                        errorFlag=false;
                        processList[i].setError(true);
                    }

                    processList[i].setFinal(globalCont);
                    processList[i].setRetorno(processList.at(i).getFinal()-processList.at(i).getLlegada());
                    processList[i].setServicio(processList.at(i).getTt());
                    processList[i].setEspera(processList.at(i).getRetorno()-processList.at(i).getServicio());

                    ui->finishProcessGL->addWidget(fproc);
                    if(contOfProcess>0)
                        --contOfProcess;
                }
                else
                {
                    processList[i].setBlocked(true);
                    processList.insert(topMemory,processList.at(i));
                    interFlag=false;
                    processList.removeAt(i);
                    --i;
                    ++contBlocked;
                }
            }
            else
            {
                PendientProcess* pproc = new PendientProcess();
                pproc->setData(processList.at(i).getId(), processList.at(i).getTimeMax(), processList.at(i).getTt());
                ui->pendientProcessGL->addWidget(pproc);
                processList.move(i,topMemory-1);
                --i;
            }
        }

        else
        {
            ui->nameLB->setText("ID: -");
            ui->opLB->setText("Operacion: -");
            ui->tmLB->setText("Tiempo Maximo: -");
            ui->ttLB->setText("Tiempo Transcurrido: -");
            ui->trLB->setText("Tiempo Restante: -");


            delay(1000);
            ++globalCont;
            ui->globalContLB->setText("Contador: "+QString::number(globalCont));

            for(int n(i); n<topMemory; ++n)
            {

                processList[n].setContBlocked(processList.at(n).getContBlocked()-1);
                if(processList.at(n).getContBlocked()==0)
                {
                    processList[n].setContBlocked(5);
                    processList[n].setBlocked(false);
                    PendientProcess* pproc = new PendientProcess();
                    pproc->setData(processList.at(n).getId(), processList.at(n).getTimeMax(), processList.at(n).getTt());
                    ui->pendientProcessGL->addWidget(pproc);
                    --contBlocked;
                }

            }

            while((child = ui->blockedGL->takeAt(0))!=0)
            {
                delete child->widget();
            }

            int column=0;
            for(int n(i); n<topMemory; ++n)
            {
                if(processList.at(n).getBlocked())
                {
                    BlockedProcess* bproc = new BlockedProcess();
                    bproc->setBlockedProcess(processList.at(n).getId(), processList.at(n).getContBlocked());
                    ui->blockedGL->addWidget(bproc, 0, column, Qt::AlignLeft);
                    ++column;
                }
            }

            if(pauseFlag)
            {
                while(true)
                {
                    delay(100);
                    if(continueFlag)
                        break;
                }
                pauseFlag=false;
                continueFlag=false;
            }

            if(bcpFlag)
            {
                setBCP();
                ui->pagesSW->setCurrentIndex(3);
                while(true)
                {
                    delay(100);
                    if(continueFlag)
                    {
                        ui->pagesSW->setCurrentIndex(1);
                        break;
                    }
                }
                bcpFlag=false;
                continueFlag=false;
            }

            if(newFlag)
            {
                setProcess(1);
                if((processList.size()-i-1)<5)
                {
                    PendientProcess* pproc = new PendientProcess();
                    pproc->setData(processList.at(processList.size()-1).getId(), processList.at(processList.size()-1).getTimeMax(), processList.at(processList.size()-1).getTt());
                    ui->pendientProcessGL->addWidget(pproc);
                    processList[processList.size()-1].setLlegada(globalCont);
                    processList[processList.size()-1].setFirst(true);
                    processList.move(processList.size()-1,i);
                    ++topMemory;
                }
                else
                {
                    ++contOfProcess;
                    ui->contOfBatchesLB->setText("Procesos Pendientes: "+QString::number(contOfProcess));
                }
                newFlag=false;
            }
            --i;
        }
    }
    ui->nameLB->setText("ID: -");
    ui->opLB->setText("Operacion: -");
    ui->tmLB->setText("Tiempo Maximo: -");
    ui->ttLB->setText("Tiempo Transcurrido: -");
    ui->trLB->setText("Tiempo Restante: -");
    QMessageBox msg;
    msg.setText("Simulacion Terminada!");
    msg.exec();
    ui->finishPB->setEnabled(true);
}

void MainWindow::delay(const int &mSeconds)
{
    QTime dieTime=QTime::currentTime().addMSecs(mSeconds);
    while(QTime::currentTime()<dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

int MainWindow::resolveProcess(const int &d1, const int &d2, const char op)
{
    int res;
    switch (op) {
        case '+':
            res = d1+d2;
            break;

        case '-':
            res = d1-d2;
            break;

        case '*':
            res = d1*d2;
            break;

        case '/':
            res = d1/d2;
            break;

        case '%':
            res = d1%d2;
            break;
    }
    return res;
}

void MainWindow::printResults()
{
    for(int i(0);i<processList.size();++i)
    {
        ResultProcess* rproc = new ResultProcess();
        rproc->setResultProcess(processList.at(i));
        ui->resultsGL->addWidget(rproc, i/5, i%5,Qt::AlignCenter);
    }
}

void MainWindow::setBCP()
{
    for(int b(0);b<processList.size();++b)
    {
        ui->bcpTW->setItem(b,0,new QTableWidgetItem(QString::number(processList.at(b).getId())));

        if(processList.at(b).getFirst())
        {
            ui->bcpTW->setItem(b,2,new QTableWidgetItem(QString::number(processList.at(b).getDigit1())+processList.at(b).getOperation()+QString::number(processList.at(b).getDigit2())));
            ui->bcpTW->setItem(b,4,new QTableWidgetItem(QString::number(processList.at(b).getLlegada())));

            if(processList.at(b).getFinal()>0)
            {
                ui->bcpTW->setItem(b,1,new QTableWidgetItem("Finalizado"));

                if(processList.at(b).getError())
                    ui->bcpTW->setItem(b,3,new QTableWidgetItem("Error"));
                else
                    ui->bcpTW->setItem(b,3,new QTableWidgetItem(QString::number(processList.at(b).getResult())));

                ui->bcpTW->setItem(b,5,new QTableWidgetItem(QString::number(processList.at(b).getFinal())));
                ui->bcpTW->setItem(b,6,new QTableWidgetItem(QString::number(processList.at(b).getRetorno())));
                ui->bcpTW->setItem(b,7,new QTableWidgetItem(QString::number(processList.at(b).getEspera())));
                ui->bcpTW->setItem(b,9,new QTableWidgetItem("0"));
                ui->bcpTW->setItem(b,11,new QTableWidgetItem("-"));

            }
            else
            {
                ui->bcpTW->setItem(b,3,new QTableWidgetItem("-"));
                ui->bcpTW->setItem(b,5,new QTableWidgetItem("-"));
                ui->bcpTW->setItem(b,6,new QTableWidgetItem("-"));
                ui->bcpTW->setItem(b,7,new QTableWidgetItem(QString::number(globalCont-processList.at(b).getLlegada()-processList.at(b).getTt())));
                ui->bcpTW->setItem(b,9,new QTableWidgetItem(QString::number(processList.at(b).getTimeMax()-processList.at(b).getTt())));

                if(processList.at(b).getBlocked())
                {
                    ui->bcpTW->setItem(b,1,new QTableWidgetItem("Bloqueado"));
                    ui->bcpTW->setItem(b,11,new QTableWidgetItem(QString::number(processList.at(b).getContBlocked())));
                }
                else
                {
                    ui->bcpTW->setItem(b,1,new QTableWidgetItem("Listo"));
                    ui->bcpTW->setItem(b,11,new QTableWidgetItem("-"));
                }
            }
            ui->bcpTW->setItem(b,8,new QTableWidgetItem(QString::number(processList.at(b).getTt())));

            if(processList.at(b).getExec())
                ui->bcpTW->setItem(b,10,new QTableWidgetItem(QString::number(processList.at(b).getRespuesta())));
            else
                ui->bcpTW->setItem(b,10,new QTableWidgetItem("-"));
        }

        else
        {
            ui->bcpTW->setItem(b,1,new QTableWidgetItem("Pendiente"));
            ui->bcpTW->setItem(b,2,new QTableWidgetItem("-"));
            ui->bcpTW->setItem(b,3,new QTableWidgetItem("-"));
            ui->bcpTW->setItem(b,4,new QTableWidgetItem("-"));
            ui->bcpTW->setItem(b,5,new QTableWidgetItem("-"));
            ui->bcpTW->setItem(b,6,new QTableWidgetItem("-"));
            ui->bcpTW->setItem(b,7,new QTableWidgetItem("-"));
            ui->bcpTW->setItem(b,8,new QTableWidgetItem("-"));
            ui->bcpTW->setItem(b,9,new QTableWidgetItem("-"));
            ui->bcpTW->setItem(b,10,new QTableWidgetItem("-"));
            ui->bcpTW->setItem(b,11,new QTableWidgetItem("-"));
        }
    }
    ui->bcpTW->resizeColumnsToContents();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(!ui->startPB->isEnabled())
    {
        if(event->key()==Qt::Key_N && pauseFlag==false && bcpFlag==false)
            newFlag=true;
        else if(event->key()==Qt::Key_I && pauseFlag==false && bcpFlag==false)
            interFlag=true;
        else if(event->key()==Qt::Key_E && pauseFlag==false && bcpFlag==false)
            errorFlag=true;
        else if(event->key()==Qt::Key_P && bcpFlag==false)
            pauseFlag=true;
        else if(event->key()==Qt::Key_B && pauseFlag==false)
            bcpFlag=true;
        else if(event->key()==Qt::Key_C && (pauseFlag==true || bcpFlag==true))
            continueFlag=true;

    }
}

//Slots

void MainWindow::on_numOfProcessPB_clicked()
{
    contOfProcess=ui->numOfProcessLE->text().toInt();
    quantum=ui->insertQuantumLE->text().toInt();
    setProcess(contOfProcess);
    ui->pagesSW->setCurrentIndex(1);

    contOfProcess-=5;
    if(contOfProcess<0)
        contOfProcess=0;
}

void MainWindow::on_numOfProcessLE_textChanged(const QString &arg1)
{
    if(arg1.toInt()>0 && ui->insertQuantumLE->text().toInt()>0)
        ui->numOfProcessPB->setEnabled(true);
    else
        ui->numOfProcessPB->setEnabled(false);
}

void MainWindow::on_insertQuantumLE_textChanged(const QString &arg1)
{
    if(arg1.toInt()>0 && ui->numOfProcessLE->text().toInt()>0)
        ui->numOfProcessPB->setEnabled(true);
    else
        ui->numOfProcessPB->setEnabled(false);
}

void MainWindow::on_startPB_clicked()
{
    ui->startPB->setEnabled(false);
    startProcess();
}

void MainWindow::on_finishPB_clicked()
{
    printResults();
    ui->pagesSW->setCurrentIndex(2);
}
