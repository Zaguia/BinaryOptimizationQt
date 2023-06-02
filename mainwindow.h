#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QLineEdit>
#include "balas_1959.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void new_function(QLineEdit *bl,QLineEdit *bn,QListWidget *lt);

private slots:
    void on_eAdd_clicked();
    void on_rAdd_clicked();
    void on_solve_clicked();
    void on_stop_clicked();

public slots :
    void a_better_feasible_is_found(QList<int> *current_feasable,double *current_feasable_objective_value);
    void an_optimum_is_found(QList<int> *current_feasable,double *current_feasable_objective_value);

 signals :
    void on_stop();

private:
    Ui::MainWindow *ui;
    balas_1959 alg;
    QList<QList<double>> model;
    bool first_model;
};
#endif // MAINWINDOW_H
