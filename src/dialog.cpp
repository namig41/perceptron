#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    dist_index = 0;
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_buttonBox_accepted()
{
    max = ui->lineEdit->text().toInt();
    min = ui->lineEdit_2->text().toInt();
    n = ui->lineEdit_3->text().toInt();

    emit create_dist();
}

void Dialog::on_comboBox_currentIndexChanged(int index)
{
    dist_index = index;
}
