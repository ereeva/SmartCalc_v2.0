#ifndef SMARTCALC_MODEL_DEPOSIT_H_
#define SMARTCALC_MODEL_DEPOSIT_H_

#include <QDate>

namespace s21 {
/// @brief deposit calculator model
class Deposit {
 public:
  Deposit() = default;
  ~Deposit() = default;
  /// @brief loads deposit data
  /// @param total total deposit amount
  /// @param tenure deposit term in months
  /// @param start_date start date of deposit
  /// @param rate interest rate
  /// @param tax_rate tax rate
  /// @param periodicity periodicity of payments
  /// @param capitalization capitalization of interests
  /// @param date_list vector of dates of account operations
  /// @param operations_list vector of account operations
  void LoadData(double total, int tenure, QDate start_date, double rate,
                double tax_rate, int periodicity, bool capitalization,
                std::vector<QDate> &date_list,
                std::vector<double> &operations_list);
  /// @brief calculates deposit info based on loaded data
  void Calculate();
  /// @brief returns accrued interest
  double Interest();
  /// @brief returns tax amount
  double Tax();
  /// @brief returns deposit amount by the end of the term
  double TotalReturn();

 private:
  double total_{};
  int tenure_{};
  QDate start_date_{};
  double rate_{};
  double tax_rate_{};
  int periodicity_{};
  bool capitalization_{};
  std::vector<QDate> date_list_{};
  std::vector<double> operations_list_{};

  double interest_{};
  double tax_{};
  double total_return_{};

  /// @brief calculates the next day of payment
  /// @param date previous payment day
  QDate NextPayday(QDate date);
  /// @brief calculates taxes
  /// @param gain yearly gain from deposit
  /// @return tax amount
  double CalculateTax(double gain);
};

}  // namespace s21
#endif  // SMARTCALC_MODEL_DEPOSIT_H_