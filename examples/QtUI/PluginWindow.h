#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class ExamplePluginWindow; }
QT_END_NAMESPACE

class ExamplePluginWindow : public QMainWindow
{
    Q_OBJECT

public:
    ExamplePluginWindow(QWidget *parent = nullptr);
    ~ExamplePluginWindow();

public slots:
    void on_btnClickMe_click();
    void on_btnAboutQt_click();

private:
    Ui::ExamplePluginWindow *ui;
};
#endif // MAINWINDOW_H
