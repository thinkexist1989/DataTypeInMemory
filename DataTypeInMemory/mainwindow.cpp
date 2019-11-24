#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QListWidget>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->showMemTableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //information update
    connect(ui->dataTypeList, &QListWidget::currentTextChanged, this, &MainWindow::informationUpdate);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::informationUpdate(QString type)
{
    qDebug() << type;
}
