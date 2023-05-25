#ifndef BALAS_1959_H
#define BALAS_1959_H

#include <QThread>
#include <QObject>
#include <QTextEdit>
#include <QList>

class balas_1959 : public QObject
{
    Q_OBJECT
public:
    explicit balas_1959(QObject *parent = nullptr);
    void set_model(QList<QList<double>> *model_);
    void solve();
    void run();
private :

    void feasible(QList<int> *selected,QList<QList<double>> *model, QList<int> *violated,QList<int> *current_feasable,double *current_feasable_objective_value, int const vars, int const constraints, bool *terminate);
    void backtrack(QList<int> *selected,const int vars);
    bool backtrack_condition(QList<int> *selected,QList<int> *violated,QList<int> *helpfull,QList<QList<int>> *helpfull_per_constraint,QList<int> *preference , int const vars,bool *terminate);
    bool termination_condition(QList<int> *selected, int const vars1);
    void send_optimum(QList<int> *current_feasable,double *current_feasable_objective_value);
    bool stop_all;
    QList<QList<double>> *model;

signals :
    void a_better_feasible_is_found(QList<int> *current_feasable,double *current_feasable_objective_value);
    void an_optimum_is_found(QList<int> *current_feasable,double *current_feasable_objective_value);

public slots :
    void stop();
};

#endif // BALAS_1959_H
