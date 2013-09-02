#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    logoFile = QDir::currentPath();
    logoFile.append("/motorola_logo.jpg");

    logoImage = new QImage(logoFile);

    if(logoImage->isNull())
    {
        QMessageBox::information(this,"Image Viewer","Error Displaying image");
        return;
    }

    //QGraphicsScene scene;
    pItem = new QGraphicsPixmapItem(QPixmap::fromImage(*logoImage));
    scene.addItem(pItem);

    ui->logo->setScene(&scene);
    ui->logo->show();

    connect(ui->stepButton, SIGNAL(clicked()), this, SLOT(nextStep()));
    connect(ui->resetButton, SIGNAL(clicked()), this, SLOT(reset()));
    connect(ui->runButton, SIGNAL(clicked()), this, SLOT(run()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::compile()
{
    vector<unsigned short> program;
    string code;
    bool _return = true;

    try
    {
        code = ui->codeEditor->toPlainText().toStdString();
        MMP.setCode(code);
        MMP.run();

        MASS.setCode(MMP.getCode());
        MASS.run();
        program = MASS.getProgram();
        MVM.setProgram(program);

        ui->programView->clear();

        for (unsigned int i = 0; i < program.size(); i++)
        {
            ui->programView->appendPlainText(QString("%1\t%2").arg(QString::number(i), QString::number(program[i])));
        }
    }
    catch (...)//M68KException &e)
    {

    }

    ui->runButton->setEnabled(true);
    ui->stepButton->setEnabled(true);
    ui->resetButton->setEnabled(true);

    return _return;
}

void MainWindow::firstState()
{
    ui->runButton->setEnabled(false);
    ui->stepButton->setEnabled(false);
    ui->resetButton->setEnabled(false);
}

void MainWindow::nextStep()
{
    MVM.nextStep();

    configState();
}

void MainWindow::configState()
{
    vector<unsigned short> As;
    vector<short> Ds;
    vector<pair<USHORT, short> > dataMemory;
    vector<pair<USHORT, short> >::iterator it;
    vector<bool> CCs;
    QString line;

    As = MVM.getAs();
    Ds = MVM.getDs();

    ui->Alcd_0->display((int)As[0]);
    ui->Alcd_1->display((int)As[1]);
    ui->Alcd_2->display((int)As[2]);
    ui->Alcd_3->display((int)As[3]);
    ui->Alcd_4->display((int)As[4]);
    ui->Alcd_5->display((int)As[5]);
    ui->Alcd_6->display((int)As[6]);
    ui->Alcd_7->display((int)As[7]);

    ui->Dlcd_0->display((int)Ds[0]);
    ui->Dlcd_1->display((int)Ds[1]);
    ui->Dlcd_2->display((int)Ds[2]);
    ui->Dlcd_3->display((int)Ds[3]);
    ui->Dlcd_4->display((int)Ds[4]);
    ui->Dlcd_5->display((int)Ds[5]);
    ui->Dlcd_6->display((int)Ds[6]);
    ui->Dlcd_7->display((int)Ds[7]);

    dataMemory = MVM.getMemory();

    ui->memoryView->clear();

    for (it = dataMemory.begin(); it != dataMemory.end(); it++)
    {
        line = QString("%1\t%2").arg(QString::number(it->first), QString::number(it->second));
        ui->memoryView->appendPlainText(line);
    }

    ui->PClcd->display((int)MVM.getPC());

    CCs = MVM.getCCs();

    ui->CClcd_0->display((int)CCs[0]);
    ui->CClcd_1->display((int)CCs[1]);
    ui->CClcd_2->display((int)CCs[2]);
    ui->CClcd_3->display((int)CCs[3]);
    ui->CClcd_4->display((int)CCs[4]);
}

void MainWindow::run()
{
    MVM.run();

    configState();
}

void MainWindow::reset()
{
    MVM.reset();

    configState();
}



















