#ifndef SMARTCALC_CONTROLLER_CONTROLLER_H_
#define SMARTCALC_CONTROLLER_CONTROLLER_H_

#include <QObject>

#include "../model/calc.h"
#include "../model/credit.h"
#include "../model/deposit.h"
#include "../view/mainwindow.h"

namespace s21 {
class Controller : public QObject {
  Q_OBJECT
 public:
  Controller(QObject *parent = nullptr);
  ~Controller();
  MainWindow *View() const;

 signals:
  void SignalResult(QString res);
  void SignalGraph(double x_min, double x_max, QVector<double> x,
                   QVector<double> y);
  void SignalCredit(std::vector<double> monthly, double total, double overpay);
  void SignalDeposit(double interest, double tax, double total);

 private:
  MainWindow *view_{};
  Calc *calc_model_{};
  Credit *credit_model_{};
  Deposit *deposit_model_{};

 private slots:
  void SlotCalc(const QString &expr, double x_value);
  void SlotGraph(const QString &expr, double x_min, double x_max);
  void SlotCredit(double total, int term, double rate, bool type);
  void SlotDeposit(double total, int tenure, QDate date, double rate,
                   double tax, int periodicity, bool capitalization,
                   std::vector<QDate> dates, std::vector<double> ops);
};

}  // namespace s21

#endif  // SMARTCALC_CONTROLLER_CONTROLLER_H_
