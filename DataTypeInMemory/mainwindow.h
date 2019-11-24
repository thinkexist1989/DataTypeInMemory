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
    QButtonGroup*   signBtnGrp;
    QButtonGroup*   archBtnGrp;
    QButtonGroup*   endianBtnGrp;


public slots:
    void informationUpdate(QString);
};

#endif // MAINWINDOW_H
