#include "credit.h"

#include <cmath>
#include <vector>

namespace s21 {
void Credit::AnnuityCredit() {
  double rate_pow = pow(1 + monthly_rate_, term_);
  double res = total_ * monthly_rate_ * rate_pow / (rate_pow - 1);
  monthly_pay_ = std::vector<double>(term_, res);
  total_pay_ = term_ * res;
}

void Credit::DifferentialCredit() {
  double diff = total_ / term_;
  monthly_pay_.clear();
  total_pay_ = 0;
  for (int i = 0; i < term_; ++i) {
    monthly_pay_.push_back(monthly_rate_ * (total_ - diff * i) + diff);
    total_pay_ += monthly_pay_[i];
  }
}

void Credit::Calculate(double total, int term, double rate, bool type) {
  total_ = total;
  term_ = term;
  monthly_rate_ = rate / 1200;
  if (type)
    AnnuityCredit();
  else
    DifferentialCredit();
}

std::vector<double> Credit::MonthlyPay() { return monthly_pay_; }
double Credit::TotalPay() { return total_pay_; }
double Credit::Overpay() { return total_pay_ - total_; }

}  // namespace s21
