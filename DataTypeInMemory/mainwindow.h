#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    bool isSigned;
    bool isX86;
    bool isLittle;
    bool show0x;

    QString type;
    QButtonGroup*   signBtnGrp;
    QButtonGroup*   archBtnGrp;
    QButtonGroup*   endianBtnGrp;

    char* memArray;


    void updateBytesAndBitsInfo(QString type);
    void updateMemTableWidget(char* p, int lens);
public slots:
    void informationUpdate(QString);
private slots:
    void on_toMemPushButton_clicked();
    void on_toValuePushButton_clicked();
    void on_showMarkerCheckBox_toggled(bool checked);
};

#endif // MAINWINDOW_H
