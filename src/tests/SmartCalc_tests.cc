#include <gtest/gtest.h>

#include <iostream>

#include "../model/calc.h"
#include "../model/credit.h"
// #include "../model/deposit.h"

s21::Calc calc;

TEST(Calc, test1) {
  std::string expr = "sin5";
  calc.LoadExpression(expr);
  double res = calc.Calculate();
  ASSERT_NEAR(res, -0.95892427466, 1e-7);
}

TEST(Calc, test2) {
  std::string expr = "+2";
  calc.LoadExpression(expr);
  double res = calc.Calculate();
  ASSERT_NEAR(res, 2, 1e-7);
}

TEST(Calc, test3) {
  std::string expr = "x";
  calc.LoadExpression(expr);
  double res = calc.Calculate();
  ASSERT_NEAR(res, 0, 1e-7);
}

TEST(Calc, test4) {
  std::string expr = "(7)";
  calc.LoadExpression(expr);
  double res = calc.Calculate();
  ASSERT_NEAR(res, 7, 1e-7);
}

TEST(Calc, test5) {
  std::string expr = "sin(4) - sqrt(3)";
  calc.LoadExpression(expr);
  double res = calc.Calculate();
  ASSERT_NEAR(res, -2.48885330288, 1e-7);
}

TEST(Calc, test6) {
  std::string expr = "-2-2";
  calc.LoadExpression(expr);
  double res = calc.Calculate();
  ASSERT_NEAR(res, -4, 1e-7);
}

TEST(Calc, test7) {
  std::string expr = "( -2-2)";
  calc.LoadExpression(expr);
  double res = calc.Calculate();
  ASSERT_NEAR(res, -4, 1e-7);
}

TEST(Calc, test8) {
  std::string expr = "(2 + 3) * 5 ^ 2 ";
  calc.LoadExpression(expr);
  double res = calc.Calculate();
  ASSERT_NEAR(res, 125, 1e-7);
}

TEST(Calc, test9) {
  std::string expr = "1/2+(2+3)/(sin(9-2)^2-6/7)";
  calc.LoadExpression(expr);
  double res = calc.Calculate();
  ASSERT_NEAR(res, -11.2505646, 1e-7);
}

TEST(Calc, test10) {
  std::string expr = "(-12.3)+4";
  calc.LoadExpression(expr);
  double res = calc.Calculate();
  ASSERT_NEAR(res, -8.3, 1e-7);
}

TEST(Calc, test11) {
  std::string expr = "(12)+(-12.00006)";
  calc.LoadExpression(expr);
  double res = calc.Calculate();
  ASSERT_NEAR(res, -0.00006, 1e-7);
}

TEST(Calc, test12) {
  std::string expr = "(-(-(-10)))-(-(-10))";
  calc.LoadExpression(expr);
  double res = calc.Calculate();
  ASSERT_NEAR(res, -20, 1e-7);
}

TEST(Calc, test13) {
  std::string expr = "(-1.2)-1.2-2.4-4.8";
  calc.LoadExpression(expr);
  double res = calc.Calculate();
  ASSERT_NEAR(res, -9.6, 1e-7);
}

TEST(Calc, test14) {
  std::string expr = "(12)*(-1.0006)";
  calc.LoadExpression(expr);
  double res = calc.Calculate();
  ASSERT_NEAR(res, -12.0072, 1e-7);
}

TEST(Calc, test15)
{s21::Calc a;
  std::string expr = "53.5mod9";
  a.LoadExpression(expr);
  double res = a.Calculate();
  ASSERT_NEAR(res, 8.5, 1e-7);
}

TEST(Calc, test16)
{
  std::string expr = "1+simx";
  EXPECT_THROW(calc.LoadExpression(expr), std::logic_error);
}

TEST(Calc, test17)
{
  std::string expr = "1?2";
  EXPECT_THROW(calc.LoadExpression(expr), std::logic_error);
}

s21::Credit credit;

TEST(Credit, annuity) {
  credit.Calculate(1000000, 7, 12, 1);
  std::vector<double> res = credit.MonthlyPay();
  std::vector<double> cmp(7, 148628.28);
  ASSERT_EQ(res.size(), cmp.size());
  for (size_t i = 0; i < res.size(); ++i) ASSERT_NEAR(res[i], cmp[i], 1e-2);
  ASSERT_NEAR(credit.TotalPay(), 1040397.96, 0.1);
  ASSERT_NEAR(credit.Overpay(), 40397.98, 0.1);
}

TEST(Credit, differential) {
  credit.Calculate(1000000, 7, 12, 0);
  std::vector<double> res = credit.MonthlyPay();
  std::vector<double> cmp{152857.14, 151428.57, 150000.00, 148571.43,
                          147142.86, 145714.29, 144285.71};
  ASSERT_EQ(res.size(), cmp.size());
  for (size_t i = 0; i < res.size(); ++i) ASSERT_NEAR(res[i], cmp[i], 1);
  ASSERT_NEAR(credit.TotalPay(), 1040000.00, 0.1);
  ASSERT_NEAR(credit.Overpay(), 40000.00, 0.1);
}

TEST(Credit, test)
{
  s21::Credit a;
  a.Calculate(1000000, 7, 12, 0);
  std::vector<double> res = a.MonthlyPay();
  std::vector<double> cmp{152857.14, 151428.57, 150000.00, 148571.43,
                          147142.86, 145714.29, 144285.71};
  ASSERT_EQ(res.size(), cmp.size());
  for (size_t i = 0; i < res.size(); ++i)
    ASSERT_NEAR(res[i], cmp[i], 1e-2);
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}