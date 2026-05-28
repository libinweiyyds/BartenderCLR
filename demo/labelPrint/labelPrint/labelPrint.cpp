#include "labelPrint.h"

labelPrint::labelPrint(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::labelPrintClass())
{
    ui->setupUi(this);
}

labelPrint::~labelPrint()
{
    delete ui;
}

