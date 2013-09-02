#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QMessageBox>
#include <QImage>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include "m68kvirtualmachine.h"
#include "m68kassembler.h"
#include "m68kmacroprocessor.h"
#include <vector>

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:

    bool compile();
    void firstState();
    void nextStep();
    void run();
    void reset();
    
private:

    Ui::MainWindow *ui;

    QString logoFile;
    QImage *logoImage;
    QGraphicsScene scene;
    QGraphicsPixmapItem *pItem;

    M68KVirtualMachine MVM;
    M68KAssembler MASS;
    M68KMacroProcessor MMP;

    void configState();
};

#endif // MAINWINDOW_H
