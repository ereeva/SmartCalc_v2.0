#include <QApplication>

#include "../controller/controller.h"
#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  s21::Controller c;
  c.View()->show();
  return a.exec();
}
