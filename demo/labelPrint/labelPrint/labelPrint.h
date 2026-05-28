#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_labelPrint.h"

QT_BEGIN_NAMESPACE
namespace Ui { class labelPrintClass; };
QT_END_NAMESPACE

class labelPrint : public QMainWindow
{
    Q_OBJECT

public:
    labelPrint(QWidget *parent = nullptr);
    ~labelPrint();

private:
    Ui::labelPrintClass *ui;
};

