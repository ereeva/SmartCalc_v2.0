#include "deposit.h"

namespace s21 {

double Deposit::Interest() { return interest_; }

double Deposit::Tax() { return tax_; }

double Deposit::TotalReturn() { return total_return_; }

void Deposit::LoadData(double total, int tenure, QDate start_date, double rate,
                       double tax_rate, int periodicity, bool capitalization,
                       std::vector<QDate> &date_list,
                       std::vector<double> &operations_list) {
  total_ = total;
  tenure_ = tenure;
  start_date_ = start_date;
  rate_ = rate / 100;
  tax_rate_ = tax_rate / 100;
  periodicity_ = periodicity;
  capitalization_ = capitalization;
  date_list_ = date_list;
  operations_list_ = operations_list;
}

QDate Deposit::NextPayday(QDate date) {
  if (periodicity_ == 0)
    date = date.addDays(1);
  else if (periodicity_ == 1)
    date = date.addMonths(1);
  else if (periodicity_ == 2)
    date = date.addMonths(3);
  else if (periodicity_ == 3)
    date = date.addMonths(6);
  else if (periodicity_ == 4)
    date = date.addYears(1);
  return date;
}

double Deposit::CalculateTax(double gain) {
  double res = 0;
  if (gain > 1e+6 * 0.13) res = tax_rate_ * (gain - 1e+6 * 0.13);
  return res;
}

void Deposit::Calculate() {
  const QDate end_date = start_date_.addMonths(tenure_);
  QDate payday = NextPayday(start_date_);
  QDate date = start_date_;
  double daily_rate = rate_ / date.daysInYear();
  double yearly_gain = 0;
  double gain = 0;
  total_return_ = total_;
  interest_ = 0;
  tax_ = 0;

  while (date != end_date) {
    date = date.addDays(1);
    if (date.month() == 1 && date.day() == 1)
      daily_rate = rate_ / date.daysInYear();
    gain += daily_rate * total_return_;
    if (capitalization_) {
      if (date == payday) {
        total_return_ += gain;
        interest_ += gain;
        yearly_gain += gain;
        gain = 0;
        payday = NextPayday(date);
      }
    } else {
      interest_ += gain;
      yearly_gain += gain;
      gain = 0;
    }
    if ((date.month() == start_date_.month() && date.day() == start_date_.day()) || date == end_date) {
      tax_ += CalculateTax(yearly_gain);
      yearly_gain = 0;
    }
    for (size_t i = 0; i != date_list_.size(); ++i)
      if (date == date_list_[i] && total_return_ + operations_list_[i] >= 0)
        total_return_ += operations_list_[i];
  }
}

}  // namespace s21
