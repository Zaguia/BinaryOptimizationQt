#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QLineEdit>

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

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
