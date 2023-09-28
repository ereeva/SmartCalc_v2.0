#include "dialog.h"

#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) : QDialog(parent), ui(new Ui::Dialog) {
  ui->setupUi(this);
}

Dialog::~Dialog() { delete ui; }

void Dialog::on_buttonBox_accepted() {
  QString date = ui->dateEdit->date().toString("dd.MM.yyyy");
  QString amount = ui->lineEdit->text();
  if (amount != "") emit signal_data(date, amount);
}
