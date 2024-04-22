#include "controller.h"

#include <QVector>

namespace s21 {
Controller::Controller(QObject *parent)
    : QObject(parent),
      view_(new MainWindow()),
      calc_model_(new Calc()),
      credit_model_(new Credit()),
      deposit_model_(new Deposit()) {
  connect(view_, &MainWindow::SignalEq, this, &Controller::SlotCalc);
  connect(this, &Controller::SignalResult, view_, &MainWindow::SlotCalcResult);

  connect(view_, &MainWindow::SignalGraph, this, &Controller::SlotGraph);
  connect(this, &Controller::SignalGraph, view_, &MainWindow::SlotBuildGraph);

  connect(view_, &MainWindow::SignalCredit, this, &Controller::SlotCredit);
  connect(this, &Controller::SignalCredit, view_,
          &MainWindow::SlotCreditResult);

  connect(view_, &MainWindow::SignalDeposit, this, &Controller::SlotDeposit);
  connect(this, &Controller::SignalDeposit, view_,
          &MainWindow::SlotDepositResult);
}

Controller::~Controller() {
  delete calc_model_;
  delete credit_model_;
  delete deposit_model_;
  delete view_;
}

MainWindow *Controller::View() const { return view_; }

void Controller::SlotCalc(const QString &expr, double x_value) {
  try {
    calc_model_->LoadExpression(expr.toStdString());
    double res = calc_model_->Calculate(x_value);
    emit SignalResult(res == res ? QString::number(res, 'g', 13) : "Error");
  } catch (std::logic_error) {
    emit SignalResult("Error");
  }
}

void Controller::SlotGraph(const QString &expr, double x_min, double x_max) {
  try {
    calc_model_->LoadExpression(expr.toStdString());
    if (x_min > x_max) std::swap(x_min, x_max);
    Graph graph = calc_model_->CaculateGraph(x_min, x_max);
    emit SignalGraph(x_min, x_max,
                     QVector<double>(graph.first.begin(), graph.first.end()),
                     QVector<double>(graph.second.begin(), graph.second.end()));
  } catch (std::logic_error) {
    emit SignalGraph(x_min, x_max, QVector<double>{}, QVector<double>{});
  }
}

void Controller::SlotCredit(double total, int term, double rate, bool type) {
  credit_model_->Calculate(total, term, rate, type);
  std::vector res = credit_model_->MonthlyPay();
  emit SignalCredit(std::vector<double>(res.begin(), res.end()),
                    credit_model_->TotalPay(), credit_model_->Overpay());
}

void Controller::SlotDeposit(double total, int tenure, QDate date, double rate,
                             double tax, int periodicity, bool capitalization,
                             std::vector<QDate> dates,
                             std::vector<double> ops) {
  deposit_model_->LoadData(total, tenure, date, rate, tax, periodicity,
                           capitalization, dates, ops);
  deposit_model_->Calculate();
  emit SignalDeposit(deposit_model_->Interest(), deposit_model_->Tax(),
                     deposit_model_->TotalReturn());
}
}  // namespace s21
