#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <QObject>
#include <QTextEdit>
#include <QList>

class algorithms : QObject
{
    Q_OBJECT
public:
    algorithms();
    void test();
    void balas_1959(QTextEdit *qt);
    void feasible(QList<int> *selected,QList<QList<double>> *model, QList<int> *violated,QList<int> *current_feasable,double *current_feasable_objective_value, int const vars, int const constraints);
    void backtrack(QList<int> *selected,const int vars);
    bool backtrack_condition(QList<int> *selected,QList<int> *violated,QList<int> *helpfull,QList<QList<int>> *helpfull_per_constraint,QList<int> *preference , int const vars);
    bool termination_condition(QList<int> *selected, int const vars1);
};

#endif // ALGORITHMS_H
