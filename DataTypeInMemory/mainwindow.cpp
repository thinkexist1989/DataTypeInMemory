#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QListWidget>
#include <QTableWidget>
#include <QDebug>
#include <QtEndian>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    isSigned(true),
    isX86(false),
    isLittle(true),
    show0x(false),
    type("bool")
{
    ui->setupUi(this);
    ui->showMemTableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->showMemTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
    ui->hostEndianLineEdit->setText("Little Endian");
#elif Q_BYTE_ORDER == Q_BIG_ENDIAN
    ui->hostEndianLineEdit->setText("Big Endian");
#endif

    //information update
    connect(ui->dataTypeList, &QListWidget::currentTextChanged, this, &MainWindow::informationUpdate);
    //init dataTypeList
    ui->dataTypeList->setCurrentRow(0);
    updateBytesAndBitsInfo("bool");

    //sign
    signBtnGrp = new QButtonGroup(this);
    signBtnGrp->addButton(ui->signedRadioButton,0);
    signBtnGrp->addButton(ui->unsignedRadioButton,1);
    connect(signBtnGrp, QOverload<int>::of(&QButtonGroup::buttonClicked), this, [=](int id){
        switch (id) {
        case 0:
            isSigned = true;
            qDebug() << tr("Signed");
            break;
        case 1:
            isSigned = false;
            qDebug() << tr("Unsigned");
            break;
        default:
            break;
        }
        on_toMemPushButton_clicked();
    });

    //architecture
    archBtnGrp = new QButtonGroup(this);
    archBtnGrp->addButton(ui->x86RadioButton,0);
    archBtnGrp->addButton(ui->x64RadioButton,1);
    connect(archBtnGrp, QOverload<int>::of(&QButtonGroup::buttonClicked), this, [=](int id){
        switch (id) {
        case 0:
            isX86 = true;
            qDebug() << tr("architecture X86");
            break;
        case 1:
            isX86 = false;
            qDebug() << tr("architecture X64");
            break;
        default:
            break;
        }
        on_toMemPushButton_clicked();
    });
    ui->x86RadioButton->setDisabled(true);


    //endian
    endianBtnGrp = new QButtonGroup(this);
    endianBtnGrp->addButton(ui->littleEndianRadioButton,0);
    endianBtnGrp->addButton(ui->bigEndianRadioButton,1);
    connect(endianBtnGrp, QOverload<int>::of(&QButtonGroup::buttonClicked), this, [=](int id){
        switch (id) {
        case 0:
            isLittle = true;
            qDebug() << tr("Little Endian");
            break;
        case 1:
            isLittle = false;
            qDebug() << tr("Big Endian");
            break;
        default:
            break;
        }
        on_toMemPushButton_clicked();
    });

    memArray = new unsigned char[8];
    on_toMemPushButton_clicked();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateBytesAndBitsInfo(QString type)
{
    int bytes = 0;
    if(type == "char")
        bytes = sizeof(char);
    else if(type == "int")
        bytes = sizeof(int);
    else if(type == "bool")
        bytes = sizeof(bool);
    else if(type == "long")
        bytes = sizeof(long);
    else if(type == "float")
        bytes = sizeof(float);
    else if(type == "short")
        bytes = sizeof(short);
    else if(type == "double")
        bytes = sizeof(double);


    ui->bytesInfoLineEdit->setText(QString::number(bytes));
    ui->bitsInfoLineEdit->setText(QString::number(bytes * 8));
}

void MainWindow::updateMemTableWidget(unsigned char *p, int lens)
{
    QString prefix;
    if(show0x)
        prefix = "0x";

    for(int i = 0; i < lens; i++){
        ui->showMemTableWidget->setItem(0,i,new QTableWidgetItem(prefix + QString("%1").arg(p[i],2,16,QLatin1Char('0')).toUpper()));
    }
}

void MainWindow::informationUpdate(QString type)
{
    this->type = type;
    updateBytesAndBitsInfo(type);
    qDebug() << tr("Data type changes to").toStdString().c_str() << type;
}

void MainWindow::on_toMemPushButton_clicked()
{
    ui->showMemTableWidget->clearContents(); //clear all contents except headers
    QString stringValue = ui->valueInputLineEdit->text();

    /********char***********/
    if(type == "char"){
        if(isSigned){ //signed
            char temp = static_cast<char>(stringValue.toInt());
            memArray = reinterpret_cast<unsigned char*>(&temp);
            updateMemTableWidget(memArray, sizeof(char));

        }
        else { //usigned
            unsigned char temp = static_cast<unsigned char>(stringValue.toInt());
            memArray = &temp;
            updateMemTableWidget(memArray, sizeof(unsigned char));
        }
    }
    /********bool***********/
    else if(type == "bool"){
            bool temp = static_cast<bool>(stringValue.toInt());
            memArray = reinterpret_cast<unsigned char*>(&temp);
            updateMemTableWidget(memArray, sizeof(bool));
    }
    /********short***********/
    else if(type == "short"){
        if(isSigned){ //signed
            short temp = stringValue.toShort();
            if(isLittle){
                toLittleEndian(temp, memArray);
                updateMemTableWidget(memArray, sizeof(short));
            }
            else {
                toBigEndian(temp, memArray);
                updateMemTableWidget(memArray, sizeof(short));
            }
        }
        else { //usigned
            unsigned short temp = stringValue.toUShort();
            if(isLittle){
                toLittleEndian(temp, memArray);
                updateMemTableWidget(memArray, sizeof(unsigned short));
            }
            else {
                toBigEndian(temp, memArray);
                updateMemTableWidget(memArray, sizeof(unsigned short));
            }
        }
    }
    /********int***********/
    else if(type == "int"){
        if(isSigned){ //signed
            int temp = stringValue.toInt();
            if(isLittle){
                toLittleEndian(temp, memArray);
                updateMemTableWidget(memArray, sizeof(int));
            }
            else {
                toBigEndian(temp, memArray);
                updateMemTableWidget(memArray, sizeof(int));
            }
        }
        else { //usigned
            unsigned int temp = stringValue.toUInt();
            if(isLittle){
                toLittleEndian(temp, memArray);
                updateMemTableWidget(memArray, sizeof(unsigned int));
            }
            else {
                toBigEndian(temp, memArray);
                updateMemTableWidget(memArray, sizeof(unsigned int));
            }
        }
    }
    /********long***********/
    else if(type == "long"){
        if(isSigned){ //signed
            qint64 temp = stringValue.toLong();
            if(isLittle){
                toLittleEndian(temp, memArray);
                updateMemTableWidget(memArray, sizeof(long));
            }
            else {
                toBigEndian(temp, memArray);
                updateMemTableWidget(memArray, sizeof(long));
            }
        }
        else { //usigned
            unsigned long temp = stringValue.toULong();
            if(isLittle){
                toLittleEndian(temp, memArray);
                updateMemTableWidget(memArray, sizeof(unsigned long));
            }
            else {
                toBigEndian(temp, memArray);
                updateMemTableWidget(memArray, sizeof(unsigned long));
            }
        }
    }
    /********float***********/
    else if(type == "float"){
        float temp = stringValue.toFloat();
        if(isLittle){
            toLittleEndian(temp, memArray);
            updateMemTableWidget(memArray, sizeof(float));
        }
        else {
            toBigEndian(temp, memArray);
            updateMemTableWidget(memArray, sizeof(float));
        }
    }
    /********double***********/
    else if(type == "double"){
        double temp = stringValue.toDouble();
        if(isLittle){
            toLittleEndian(temp, memArray);
            updateMemTableWidget(memArray, sizeof(double));
        }
        else {
            toBigEndian(temp, memArray);
            updateMemTableWidget(memArray, sizeof(double));
        }
    }

}

void MainWindow::on_toValuePushButton_clicked()
{

}

void MainWindow::on_showMarkerCheckBox_toggled(bool checked)
{
    show0x = checked;
    on_toMemPushButton_clicked();
}

template<typename T>
void MainWindow::toLittleEndian(T src, unsigned char *dest)
{
#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
    memcpy(dest, reinterpret_cast<unsigned char*>(&src), sizeof(T));
#else
    memSwapCopy(dest, reinterpret_cast<unsigned char*>(&src), sizeof(T));
#endif
}

template<typename T>
void MainWindow::toBigEndian(T src, unsigned char *dest)
{
#if Q_BYTE_ORDER == Q_BIG_ENDIAN
    memcpy(dest, reinterpret_cast<unsigned char*>(&src), sizeof(T));
#else
    memSwapCopy(dest, reinterpret_cast<unsigned char*>(&src), sizeof(T));
#endif
}

void MainWindow::memSwapCopy(unsigned char *Dst, const unsigned char *src, int lens)
{
    for(int i = 0; i < lens; i++){
        memcpy(&(Dst[i]), &(src[lens - 1 - i]), 1);
    }
}
