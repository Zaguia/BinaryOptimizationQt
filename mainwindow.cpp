#include "mainwindow.h"
#include "algorithms.h"
#include "./ui_mainwindow.h"
#include <cmath>

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

    /*

    double t = 100;
    func= -17*x[0]-35*x[1]-34*x[3]
       -t*log(-40*x[0]-14*x[1]-4*x[2]+49)
       -t*log(-5*x[0]-31*x[1]-26*x[2]+50)
       -t*log(-35*x[0]-3*x[1]-4*x[2]+10)
       -t*log(-5*x[0]*x[0] + 5*x[0]) //binary constraint
       -t*log(-5*x[1]*x[1] + 5*x[1])
       -t*log(-5*x[2]*x[2] + 5*x[2]); //function

    grad[0] = -17
           -t*(-40)/(-40*x[0]-14*x[1]-4*x[2]+49)
           -t*(-5)/(-5*x[0]-31*x[1]-26*x[2]+50)
           -t*(-35)/-(35*x[0]-3*x[1]-4*x[2]+10)
           -t*(-5*x[0]+5)/(-5*x[0]*x[0]+5*x[0]); //derivative d/dx0
    grad[1] = -35
            -t*(14)/(-40*x[0]-14*x[1]-4*x[2]+49)
            -t*(31)/(-5*x[0]-31*x[1]-26*x[2]+50)
            -t*(3)/(-35*x[0]-3*x[1]-4*x[2]+10)
            -t*(-5*x[1]+5)/(-5*x[1]*x[1]+5*x[1]); //derivative d/dx1
    grad[2] = -34
            -t*(4)/(-40*x[0]-14*x[1]-4*x[2]+49)
            -t*(26)/(-5*x[0]-31*x[1]-26*x[2]+50)
            -t*(4)/(-35*x[0]-3*x[1]-4*x[2]+10)
            -t*(-5*x[2]+5)/(-5*x[2]*x[2]+5*x[2]); //derivative d/dx2
            */
    algorithms* alg = new algorithms();
    alg->balas_1959(ui->solution);

    //QString r = "look at the libraries to test in the notes";
    //ui->solution->setText(r);
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

