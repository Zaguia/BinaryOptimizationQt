#include "mainwindow.h"
#include "balas_1959.h"
#include "./ui_mainwindow.h"
#include <cmath>
#include <QtConcurrent>

//balas_1959 *alg;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_eAdd_clicked()
{
    new_function(ui->eBarLength,ui->eBarNumber,ui->eBarList);

}


void MainWindow::on_rAdd_clicked()
{
    new_function(ui->rBarLength,ui->rBarNumber,ui->rBarList);
}



void MainWindow::on_solve_clicked()
{

    //The test problem is :

    //maximize requested bars first

    /*
        min:                                                         -3*rbar1-2*rbar2;

             2*bar1         -3*rbar11-2*rbar21                                        >=0;
                    5*bar2                    -3*rbar12-2*rbar22                      >=0;

                            + rbar11            +rbar12               -rbar1          =0;

                                        rbar21           +rbar22              -rbar2  =0;
    */

    /* becomes
        min:                                                         -3*(1-rbar1)-2*(1-rbar2);

             2*bar1         -3*rbar11-2*rbar21                                           >=0;
                    5*bar2                    -3*rbar12-2*rbar22                         >=0;

                            + rbar11            +rbar12               -(1-rbar1)          =0;

                                        rbar21           +rbar22              -(1-rbar2)  =0;
    */

    /* becomes
        min:                                                        +3*rbar1  + 2*rbar2 - 5;

             2*bar1         -3*rbar11-2*rbar21                                        >=0;
                    5*bar2                    -3*rbar12-2*rbar22                      >=0;

                            + rbar11            +rbar12               + rbar1         >=1;
                            + rbar11            +rbar12               + rbar1         <=1;

                                        rbar21           +rbar22               +rbar2 >=1;
                                        rbar21           +rbar22               +rbar2 <=1;
    */


    /* becomes
     *                                                                  0           0  solution
     *                                                                 (1           1) those two are 1-x
        min:                                                         +3*rbar1  + 2*rbar2 - 5;

             2*bar1         -3*rbar11-2*rbar21                                          >=0;
                    5*bar2                    -3*rbar12-2*rbar22                        >=0;

                            + rbar11            +rbar12               + rbar1           >=1;
                            - rbar11            -rbar12               - rbar1           >=-1;

                                        rbar21           +rbar22               +  rbar2 >=1;
                                       -rbar21           -rbar22               -  rbar2 >=-1;
    */

    // minimize available bars usage for the maximum requested bars possible (this model keeps small requested bars to the end)
    /*
        min: 2*bar1+5*bar2;

             2*bar1         -3*rbar11-2*rbar21                                        >=0;
                    5*bar2                    -3*rbar12-2*rbar22                      >=0;

                            + rbar11            +rbar12               -1              =0;

                                        rbar21           +rbar22              -1      =0;
    */

    /* becomes
     *            0      1
        min: 2*bar1+5*bar2;

             2*bar1         -3*rbar11-2*rbar21                                        >=0;
                    5*bar2                    -3*rbar12-2*rbar22                      >=0;

                            + rbar11            +rbar12                               =>1;
                            - rbar11            -rbar12                               =>-1;

                                        rbar21           +rbar22                      =>1;
                                       -rbar21           -rbar22                      =>-1;
    */
    /*first model
     *    here 1s are selected ==>(0  1  0  0  1  1)(0  0)<== 0s here are selected
    QList<QList<double>> model_ {{ 0, 0, 0, 0, 0, 0, 3, 2, 0}, // putting a requested bar to a 0 or to a 1 makes it become a constant

                                 { 2, 0,-3,-2, 0, 0, 0, 0, 0}, // possible cuts for existing bar1
                                 { 0, 5, 0, 0,-3,-2, 0, 0, 0}, // possible cuts for existing bar2

                                 { 0, 0, 1, 0, 1, 0, 1, 0, 1}, // an existing bar should be used only once (used = 0: not used = 1)
                                 { 0, 0,-1, 0,-1, 0, 0,-1,-1}, // an existing bar should be used only once (used = 0: not used = 1)

                                 { 0, 0, 0, 1, 0, 1, 1, 0, 1},  //<=0 for the not selected among requested bars
                                 { 0, 0, 0,-1, 0,-1, 0,-1,-1}}; //<=0 for the not selected among requested bars


    //second model                 0  1  0  0  1  1  0  0
    QList<QList<double>> model_ {{ 2, 5, 0, 0, 0, 0, 0, 0, 0}, // putting a requested bar to a 0 or to a 1 makes it become a constant

                                 { 2, 0,-3,-2, 0, 0, 0, 0, 0}, // possible cuts for existing bar1
                                 { 0, 5, 0, 0,-3,-2, 0, 0, 0}, // possible cuts for existing bar2

                                 { 0, 0, 1, 0, 1, 0, 0, 0, 1}, // an existing bar should be used only once (used = 0: not used = 1)
                                 { 0, 0,-1, 0,-1, 0, 0, 0,-1}, // an existing bar should be used only once (used = 0: not used = 1)

                                 { 0, 0, 0, 1, 0, 1, 0, 0, 1},  //<=0 for the not selected among requested bars
                                 { 0, 0, 0,-1, 0,-1, 0, 0,-1}}; //<=0 for the not selected among requested bars
*/

    //model = model_;
    first_model = true;
    model.clear();
    //read the ui
    QList<double> existing_bars;
    int existing_bars_total = ui->eBarList->count();
    for(int i =0 ; i < existing_bars_total; i++){
        existing_bars.append(ui->eBarList->item(i)->text().toInt());
    }

    QList<double> requested_bars;
    int requested_bars_total = ui->rBarList->count();
    for(int i =0 ; i < requested_bars_total; i++){
        requested_bars.append(ui->rBarList->item(i)->text().toInt());
    }

    //formulate an objective (both objectives together - later on we would differentiate the model to 2 objectives)
    QList<double> objective;
    //add existing bars
    for(int i =0 ; i < existing_bars.size(); i++){
        objective.append(existing_bars.at(i));
    }
    //add all the zeros (possible cuts)
    for(int i =0 ; i < existing_bars.size()*requested_bars.size(); i++){
        objective.append(0);
    }
    //add the requested bars
    for(int i =0 ; i < requested_bars.size(); i++){
        objective.append(requested_bars.at(i));
    }
    //add the not used inequality (0)
    objective.append(0);
    model.append(objective);


    //add possible cuts
    for(int k =0 ; k < existing_bars.size(); k++){
        QList<double> possible_cuts;
        for(int i =0 ; i < existing_bars.size(); i++){
            if(k==i){
                possible_cuts.append(existing_bars.at(i));
            }else{
                possible_cuts.append(0);
            }

        }
        for(int i =0 ; i < existing_bars.size(); i++){
            for(int j =0 ; j < requested_bars.size(); j++){
                if(k==i){
                    possible_cuts.append(-1*requested_bars.at(j));
                }else{
                    possible_cuts.append(0);
                }
            }
        }
        for(int j =0 ; j < requested_bars.size(); j++){
            possible_cuts.append(0);
        }
        possible_cuts.append(0);
        model.append(possible_cuts);
    }

    // an existing bar should be used only once (used = 0: not used = 1)
    for(int requested_id=0;requested_id<requested_bars.size();requested_id++){
        int n=0;
        QList<double> upper_equality_constraint;
        QList<double> lower_equality_constraint;
        for(int i=0;i<existing_bars.size()+(existing_bars.size()+1)*requested_bars.size();i++){//existing.size + n*requested.size + requested.id < existing.size*(requested.size+1)
            //int possible_cuts_biggest_id = existing_bars.size()*(requested_bars.size()+1);
            int the_requested_id_next_accurance = existing_bars.size() + (n*requested_bars.size()) + requested_id;
            if(i==the_requested_id_next_accurance ){//&& i<possible_cuts_biggest_id
                upper_equality_constraint.append(1);
                lower_equality_constraint.append(-1);
                n++;
            }else{
                upper_equality_constraint.append(0);
                lower_equality_constraint.append(0);
            }

        }
        upper_equality_constraint.append(1);
        lower_equality_constraint.append(-1);
        model.append(upper_equality_constraint);
        model.append(lower_equality_constraint);
    }


/*
    QString r="";
    for(int i=0;i<model.size();i++){
        for(int j=0;j<model.at(0).size();j++){
            r+= QString::number(model.at(i).at(j))+"\t";
        }
        r+="\n";
    }
    ui->solution->setText(r);
*/
    //model = model_;

    connect(&alg,&balas_1959::a_better_feasible_is_found,this,&MainWindow::a_better_feasible_is_found);
    connect(&alg,&balas_1959::an_optimum_is_found,this,&MainWindow::an_optimum_is_found);
    connect(this,&MainWindow::on_stop,&alg,&balas_1959::stop);

    //set the objective to maximizing requested bars
    int first_part = existing_bars.size();
    int middle_part = existing_bars.size()*requested_bars.size();
    int last_part = requested_bars.size();
    for(int i=0;i<first_part; i++){
        model[0][i] = 0;
    }

    alg.set_model(&model);
    QFuture<void> solve = QtConcurrent::run(&balas_1959::solve,&this->alg);//,add model

    //set the objective to minimizing used bars
    //first_model = false;



}

void MainWindow::new_function(QLineEdit *bl,QLineEdit *bn,QListWidget *tl){
    QString l = bl->text();
    QString n = bn->text();
    int n1=0;
    try {
        n1 = n.toInt();
    }catch (const char* e) {

    }
    for(int i=0;i<n1;i++){
        tl->addItem(l);
    }
}


void MainWindow::on_stop_clicked()
{
    emit on_stop();
}

void MainWindow::a_better_feasible_is_found(QList<int> *current_feasable,double *current_feasable_objective_value){
    QString r = "";
    //r += "Current solution : ";
    //for(int j=0;j<current_feasable->length();j++){
    //    r += QString::number(current_feasable->at(j))+";";
    //}
    //r+= "\nFor a total of : "+QString::number(*current_feasable_objective_value)+"\n";

    //int const vars = model->at(0).size()-1;

    QList<double> existing_bars;
    int existing_bars_total = ui->eBarList->count();
    for(int i =0 ; i < existing_bars_total; i++){
        existing_bars.append(ui->eBarList->item(i)->text().toInt());
    }

    QList<double> requested_bars;
    int requested_bars_total = ui->rBarList->count();
    for(int i =0 ; i < requested_bars_total; i++){
        requested_bars.append(ui->rBarList->item(i)->text().toInt());
    }


    if(first_model){
        r += "There is a solution for the requested bars: ";
        int first_part = existing_bars.size();
        int middle_part = existing_bars.size()*requested_bars.size();
        int last_part = requested_bars.size();
        for(int i=first_part+middle_part;i<first_part+middle_part+last_part; i++){
            if(current_feasable->at(i) == 0){
                r = r + QString::number(requested_bars.at(i-first_part-middle_part)) + " ";
            }
        }
    }else{

    }
    ui->solution->setText(r);
}


void MainWindow::an_optimum_is_found(QList<int> *current_feasable,double *current_feasable_objective_value){
    QString r = "";
    //r += "Optimal solution : ";
    //for(int j=0;j<current_feasable->length();j++){
    //    r += QString::number(current_feasable->at(j))+";";
    //}
    //r+= "\nFor a total of : "+QString::number(*current_feasable_objective_value)+"\n";
    QList<double> existing_bars;
    int existing_bars_total = ui->eBarList->count();
    for(int i =0 ; i < existing_bars_total; i++){
        existing_bars.append(ui->eBarList->item(i)->text().toInt());
    }

    QList<double> requested_bars;
    int requested_bars_total = ui->rBarList->count();
    for(int i =0 ; i < requested_bars_total; i++){
        requested_bars.append(ui->rBarList->item(i)->text().toInt());
    }


    if(first_model){
        r += "There is an optimal solution for the requested bars: ";
        int first_part = existing_bars.size();
        int middle_part = existing_bars.size()*requested_bars.size();
        int last_part = requested_bars.size();
        for(int i=first_part+middle_part;i<first_part+middle_part+last_part; i++){
            if(current_feasable->at(i) == 0){
                r = r + QString::number(requested_bars.at(i-first_part-middle_part)) + " ";
            }
        }
    }else{

    }
    ui->solution->setText(r);
}

