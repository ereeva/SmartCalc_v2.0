#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDate>
#include <QMainWindow>

#include "../libs/qcustomplot.h"
#include "dialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

namespace s21 {
class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 signals:
  void SignalEq(const QString &expr, double x_value);
  void SignalGraph(const QString &expr, double x_min, double x_max);
  void SignalCredit(double total, int term, double rate, bool type);
  void SignalDeposit(double total, int tenure, QDate date, double rate,
                     double tax, int periodicity, bool capitalization,
                     std::vector<QDate> dates, std::vector<double> ops);

 public slots:
  void SlotCalcResult(QString res);
  void SlotBuildGraph(double x_min, double x_max, QVector<double> x,
                      QVector<double> y);
  void SlotCreditResult(std::vector<double> monthly, double total,
                        double overpay);
  void SlotDepositResult(double interest, double tax, double total);

 private:
  Ui::MainWindow *ui;
  Dialog *dialog;

  void ConnectInputButtons();

 private slots:
  void SlotInput();
  void SlotDel();
  void SlotCalculate();
  void SlotCalcGraph();
  void SlotCalcCredit();
  void SlotCalcDeposit();

  void SlotAxisChanged(QCPRange range);
  void on_pushButton_rep_plus_clicked();
  void on_pushButton_rep_minus_clicked();
  void on_pushButton_wit_plus_clicked();
  void on_pushButton_wit_minus_clicked();

  void AddRep(QString date, QString amount);
  void AddWit(QString date, QString amount);
};
}  // namespace s21
#endif  // MAINWINDOW_H
