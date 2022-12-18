#include "PluginWindow.h"
#include "ui_PluginWindow.h"

ExamplePluginWindow::ExamplePluginWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ExamplePluginWindow)
{
    ui->setupUi(this);

    QObject::connect(ui->btnClickMe, SIGNAL(clicked()), this, SLOT(on_btnClickMe_click()));
    QObject::connect(ui->btnAboutQt, SIGNAL(clicked()), this, SLOT(on_btnAboutQt_click()));
}

void ExamplePluginWindow::on_btnClickMe_click()
{
    QMessageBox::information(this, "DPF Example UI", "Hello DPF!");
}

void ExamplePluginWindow::on_btnAboutQt_click()
{
    QMessageBox::aboutQt(this);
}

ExamplePluginWindow::~ExamplePluginWindow()
{
    delete ui;
}

