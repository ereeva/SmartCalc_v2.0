#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog {
  Q_OBJECT

 public:
  explicit Dialog(QWidget *parent = nullptr);
  ~Dialog();

 private:
  Ui::Dialog *ui;

 private slots:
  void on_buttonBox_accepted();
  //    void on_lineEdit_textEdited(const QString &arg1);

 signals:
  void signal_data(QString date, QString amount);
};

#endif  // DIALOG_H
