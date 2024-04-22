#include "mainwindow.h"

#include "dialog.h"
#include "ui_mainwindow.h"

namespace s21 {
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  ConnectInputButtons();
  connect(ui->pushButton_AC, SIGNAL(clicked()), ui->lineEdit, SLOT(clear()));
  connect(ui->pushButton_del, SIGNAL(clicked()), this, SLOT(SlotDel()));
  connect(ui->lineEdit, SIGNAL(textChanged(QString)), ui->lineEdit_graph,
          SLOT(setText(QString)));

  connect(ui->pushButton_eq, SIGNAL(clicked()), this, SLOT(SlotCalculate()));
  connect(ui->pushButton_build, SIGNAL(clicked()), this, SLOT(SlotCalcGraph()));
  connect(ui->plot->xAxis, SIGNAL(rangeChanged(QCPRange)), this,
          SLOT(SlotAxisChanged(QCPRange)));

  connect(ui->pushButton_credit, SIGNAL(clicked()), this,
          SLOT(SlotCalcCredit()));

  connect(ui->pushButton_deposit, SIGNAL(clicked()), this,
          SLOT(SlotCalcDeposit()));
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::ConnectInputButtons() {
  QVector<QPushButton *> buttons{
      ui->pushButton_0,    ui->pushButton_1,    ui->pushButton_2,
      ui->pushButton_3,    ui->pushButton_4,    ui->pushButton_5,
      ui->pushButton_6,    ui->pushButton_7,    ui->pushButton_8,
      ui->pushButton_9,    ui->pushButton_dot,  ui->pushButton_x,
      ui->pushButton_add,  ui->pushButton_sub,  ui->pushButton_mul,
      ui->pushButton_div,  ui->pushButton_mod,  ui->pushButton_pow,
      ui->pushButton_acos, ui->pushButton_asin, ui->pushButton_atan,
      ui->pushButton_cos,  ui->pushButton_sin,  ui->pushButton_tan,
      ui->pushButton_ln,   ui->pushButton_log,  ui->pushButton_sqrt,
      ui->pushButton_br_l, ui->pushButton_br_r};
  for (auto i = buttons.begin(); i != buttons.end(); ++i)
    connect(*i, SIGNAL(clicked()), this, SLOT(SlotInput()));
}

void MainWindow::SlotInput() {
  if (ui->lineEdit->text() == "Error") ui->lineEdit->clear();
  QPushButton *button = (QPushButton *)sender();
  ui->lineEdit->setText((ui->lineEdit->text()) + button->text());
}

void MainWindow::SlotDel() {
  if (ui->lineEdit->text() == "Error")
    ui->lineEdit->clear();
  else
    ui->lineEdit->backspace();
}

void MainWindow::SlotCalculate() {
  QString str = ui->lineEdit->text();
  emit SignalEq(str == "" ? "0" : str, ui->doubleSpinBox->value());
}

void MainWindow::SlotCalcGraph() {
  QString str = ui->lineEdit_graph->text();
  double x_min = ui->doubleSpinBox_minx->value();
  double x_max = ui->doubleSpinBox_maxx->value();
  ui->plot->xAxis->setRange(x_min, x_max);
  ui->plot->yAxis->setRange(x_min * 0.76, x_max * 0.76);
  emit SignalGraph(str == "" ? "x" : str, x_min, x_max);
}

void MainWindow::SlotAxisChanged(QCPRange range) {
  ui->doubleSpinBox_minx->setValue(qRound(range.lower));
  ui->doubleSpinBox_maxx->setValue(qRound(range.upper));
  QString str = ui->lineEdit_graph->text();
  emit SignalGraph(str == "" ? "x" : str, qRound(range.lower),
                   qRound(range.upper));
}

void MainWindow::SlotCalcResult(QString res) { ui->lineEdit->setText(res); }

void MainWindow::SlotBuildGraph(double x_min, double x_max, QVector<double> x,
                                QVector<double> y) {
  ui->doubleSpinBox_minx->setValue(x_min);
  ui->doubleSpinBox_maxx->setValue(x_max);
  ui->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom |
                            QCP::iSelectPlottables);
  ui->plot->addGraph();
  ui->plot->graph(0)->setData(x, y);
  ui->plot->replot();
}

//≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈

void MainWindow::SlotCalcCredit() {
  emit SignalCredit(ui->doubleSpinBox_total->value(), ui->spinBox_term->value(),
                    ui->doubleSpinBox_rate->value(),
                    ui->radioButton_annuity->isChecked());
}

void MainWindow::SlotCreditResult(std::vector<double> monthly, double total,
                                  double overpay) {
  ui->list_res_monthly->clear();
  for (size_t i = 0; i < monthly.size(); ++i) {
    ui->list_res_monthly->insertItem(
        i, QString::number(i + 1) + "\t" + QString::number(monthly[i], 'f', 2));
  }
  ui->label_res_total->setText("  " + QString::number(total, 'f', 2));
  ui->label_res_overpayment->setText("  " + QString::number(overpay, 'f', 2));
}

//≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈

void MainWindow::SlotCalcDeposit() {
  std::vector<QDate> dates{};
  std::vector<double> ops{};
  for (int i = 0; i != ui->tableWidget_rep->rowCount(); ++i) {
    dates.push_back(QDate::fromString(ui->tableWidget_rep->item(i, 0)->text(),
                                      "dd.MM.yyyy"));
    ops.push_back(ui->tableWidget_rep->item(i, 1)->text().toDouble());
  }
  for (int i = 0; i != ui->tableWidget_wit->rowCount(); ++i) {
    dates.push_back(QDate::fromString(ui->tableWidget_wit->item(i, 0)->text(),
                                      "dd.MM.yyyy"));
    ops.push_back(-ui->tableWidget_wit->item(i, 1)->text().toDouble());
  }
  emit SignalDeposit(ui->doubleSpinBox_total_d->value(),
                     ui->spinBox_term_d->value(), ui->dateEdit_date->date(),
                     ui->doubleSpinBox_rate_d->value(),
                     ui->doubleSpinBox_tax_d->value(),
                     ui->comboBox_periodicity->currentIndex(),
                     ui->checkBox_capitalization->isChecked(), dates, ops);
}

void MainWindow::SlotDepositResult(double interest, double tax, double total) {
  ui->label_res_interest->setText(QString::number(interest, 'f', 2));
  ui->label_res_tax->setText(QString::number(tax, 'f', 2));
  ui->label_res_total_d->setText(QString::number(total, 'f', 2));
}

void MainWindow::on_pushButton_rep_plus_clicked() {
  dialog = new Dialog(this);
  dialog->setModal(true);
  dialog->setWindowTitle("Replenishment");
  connect(dialog, SIGNAL(signal_data(QString, QString)), this,
          SLOT(AddRep(QString, QString)));
  dialog->exec();
}

void MainWindow::on_pushButton_wit_plus_clicked() {
  dialog = new Dialog(this);
  dialog->setModal(true);
  dialog->setWindowTitle("Withdrawal");
  connect(dialog, SIGNAL(signal_data(QString, QString)), this,
          SLOT(AddWit(QString, QString)));
  dialog->exec();
}

void MainWindow::on_pushButton_rep_minus_clicked() {
  ui->tableWidget_rep->removeRow(0);
}

void MainWindow::on_pushButton_wit_minus_clicked() {
  ui->tableWidget_wit->removeRow(0);
}

void MainWindow::AddRep(QString date, QString amount) {
  ui->tableWidget_rep->insertRow(0);
  ui->tableWidget_rep->setItem(0, 0, new QTableWidgetItem(date));
  ui->tableWidget_rep->setItem(0, 1, new QTableWidgetItem(amount));
}

void MainWindow::AddWit(QString date, QString amount) {
  ui->tableWidget_wit->insertRow(0);
  ui->tableWidget_wit->setItem(0, 0, new QTableWidgetItem(date));
  ui->tableWidget_wit->setItem(0, 1, new QTableWidgetItem(amount));
}
}  // namespace s21
