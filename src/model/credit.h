#ifndef SMARTCALC_MODEL_CREDIT_H_
#define SMARTCALC_MODEL_CREDIT_H_

#include <vector>

namespace s21 {
class Credit {
 public:
  Credit() = default;
  ~Credit() = default;
  /// @brief loads credit data and calls function to calculate the credit info
  /// depending on the credit type
  /// @param total total credit amount
  /// @param term term of credit in months
  /// @param rate interest rate
  /// @param type 0 - differential, 1 - annuity
  void Calculate(double total, int term, double rate, bool type);

  /// @brief returns monthly payments
  /// @return vector of monthly payments
  std::vector<double> MonthlyPay();
  /// @brief returns total payment
  double TotalPay();
  /// @brief returns overpayment on credit
  double Overpay();

 private:
  double total_{};
  int term_{};
  double monthly_rate_{};
  std::vector<double> monthly_pay_{};
  double total_pay_{};

  /// @brief calculates annuity credit for loaded data
  void AnnuityCredit();
  /// @brief calculates differential credit for loaded data
  void DifferentialCredit();
};

}  // namespace s21

#endif  // SMARTCALC_MODEL_CREDIT_H_