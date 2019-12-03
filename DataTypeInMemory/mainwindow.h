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

    unsigned char* memArray;


    void updateBytesAndBitsInfo(QString type);
    void updateMemTableWidget(unsigned char *p, int lens);

    template<typename T> void toLittleEndian(T src, unsigned char *dest);
    template<typename T> void toBigEndian(T src, unsigned char *dest);

    void memSwapCopy(unsigned char* Dst, const unsigned char* src, int lens);
public slots:
    void informationUpdate(QString);
private slots:
    void on_toMemPushButton_clicked();
    void on_toValuePushButton_clicked();
    void on_showMarkerCheckBox_toggled(bool checked);
};

#endif // MAINWINDOW_H
