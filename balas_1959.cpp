#include "balas_1959.h"
#include <cmath>

QTextEdit *qt;

balas_1959::balas_1959(QObject *parent)
    : QObject{parent}
{
    this->stop_all = false;
}

void balas_1959::set_model(QList<QList<double>> *model_)
{
    model = model_;
}

void balas_1959::stop(){
    this->stop_all = true;
}

void balas_1959::solve()//QTextEdit *qt)
{
    //This requires integer variables and a positive coeficients at the objective
    //ex:

    //minimize : x-y
    //subject to : x+y >= 2
    //where x and y are binary

    //becomes

    //minimize : x + -(1-y) = x + y -1
    //subject to : x + (1-y) >=2 equivalent to x-y+1>=2

    //this is an implementation of https://www.youtube.com/watch?v=ajX2PCFt3R4


    int const vars = model->at(0).size()-1;
    int const constraints = model->size();





    //resort variables over a soso index of helpfullness
    //it is not necessary

    QList<double> indecies;
    QList<int> preference;
    for(int j=0;j<vars;j++){
        indecies.append(0);
        preference.append(j);
    }
    for(int j=0;j<vars;j++){
        for(int i=1;i<constraints;i++){
            indecies[j] += model->at(i).at(j);
        }
    }

    /*QString r = "";
    for(int j=0;j<vars;j++){
        r += QString::number(indecies[j])+",";
    }
    qt->setText(r);*/

    for(int i=0;i<vars;i++){
        for(int j=i;j<vars;j++){
            if(indecies[i]<indecies[j]){
                double swap = indecies[i];
                indecies[i] = indecies[j];
                indecies[j] = swap;
                swap = preference[i];
                preference[i] = preference[j];
                preference[j] = swap;
            }
        }
    }

    /*QString r = "";
    for(int j=0;j<vars;j++){
        r += QString::number(indecies[j])+";";
        r += QString::number(preference[j])+",";
    }
    qt->setText(r);*/

//Balas algorithm

    //Initialization

        //Select helpfull variables per constraint:
    QList<QList<int>> helpfull_per_constraint;
    for(int i=1;i<constraints;i++){
        QList<int> constraint_helpfull_vars;
        for(int j=0;j<vars;j++){
            if(model->at(i).at(preference[j])>0){
                constraint_helpfull_vars.append(preference[j]);
            }
        }
        helpfull_per_constraint.append(constraint_helpfull_vars);
    }


    QList<int> selected;
    QList<int> violated;
    QList<int> helpfull;//positive only
    QList<int> current_feasable;//positive only
    double current_feasable_objective_value = 1;
    for(int j=0;j<vars;j++){
        current_feasable_objective_value += model->at(0).at(j);
    }

    // Balas logic
    //----------------------------------------------------------------------------------

    int safety = 0;
    //QString r="";
    bool termination_when_backtrack_is_called = false;
    bool feasible1 = false;
    bool infeasible = false;
    stop_all = false;

    while(!this->stop_all){// && !(safety>15)


        safety++;

        if(safety==100){
            continue;
        }


        if(feasible1 || infeasible){
            termination_when_backtrack_is_called = termination_condition(&selected, vars);
            if(termination_when_backtrack_is_called){
                break;
            }
            backtrack(&selected, vars);//o1
        }else if(helpfull.size()>0) {
            selected.append(helpfull.at(0));
        }

        feasible(&selected,model, &violated, &current_feasable,&current_feasable_objective_value, vars, constraints, &stop_all);//o2
        feasible1 = violated.size()==0;
        infeasible = backtrack_condition(&selected,&violated,&helpfull,&helpfull_per_constraint,&preference , vars, &stop_all);//o2
        if(feasible1){infeasible=false;}//for the display

        //QThread::currentThread()->msleep(500);

/*
        r += "\n\nAfter Step "+QString::number(safety)+":::::: \n";

        r += "Selected :";
        for(int j=0;j<selected.length();j++){
            r += QString::number(selected.at(j))+";";
        }
        r+= "\n";

        r += "Violated constraints : ";
        for(int j=0;j<violated.length();j++){
            r += QString::number(violated.at(j))+";";
        }
        r+= "\n";

        if(feasible1){
            r+= "feasible = true \n";
        }else{
            r+= "feasible = false\n";
        }

        if(infeasible){
            r+="infeasible = true \n";
        }else{
            r+= "infeasible = false\n";
        }

        r += "Helpfull vars : ";
        for(int j=0;j<helpfull.length();j++){
            r += QString::number(helpfull.at(j))+";";
        }
        r+= "\n";

        r += "Current solution : ";
        for(int j=0;j<current_feasable.length();j++){
            r += QString::number(current_feasable.at(j))+";";
        }
        r+= "\nFor a total of : "+QString::number(current_feasable_objective_value)+"\n";
*/
    }
    //qt->setText(r);
    vars_values.clear();

    for(int i=0;i<vars;i++){
        vars_values.append(0);
    }
    for(int i= 0; i<current_feasable.size();i++){
        if(current_feasable.at(i) >= vars){
            vars_values[current_feasable.at(i)-vars] = 0;
        }else{
            vars_values[current_feasable.at(i)] = 1;
        }
    }
    send_optimum(&vars_values,&current_feasable_objective_value);
    //emit an_optimum_is_found(&current_feasable,&current_feasable_objective_value);


}

void balas_1959::backtrack(QList<int> *selected, const int vars){
    //int first_found;// = selected->size()-1;
    QList<int> selected_copy;
    //starting with thr rightest, flip the first not flipped
    //remove any at the right of the flipped
    bool start_copying = false;
    for(int i= selected->size()-1;i>=0;i--){
        if(selected->at(i)<vars && !start_copying){//not flipped
            selected->replace(i , selected->at(i) + vars);
            //first_found = i;
            start_copying = true;
        }
        if(start_copying){
            selected_copy.append(selected->at(i));
        }
    }
    selected->clear();

    for(int i=0;i<selected_copy.size();i++){
        selected->append(selected_copy.at(selected_copy.size()-1-i));
    }

}

bool balas_1959::backtrack_condition(QList<int> *selected,QList<int> *violated,QList<int> *helpfull,QList<QList<int>> *helpfull_per_constraint,QList<int> *preference , int const vars, bool *terminate){
    //remove the helpfull that are put through selection to 0


    //check that all violated conditions still have a helpfull in o2  time
    QList<int> helpfull_sub_set;
    bool backtrack=true;
    bool backtrack_due_to_set_0s_canceling_a_constraint_helpfull_vaiabales=false;
    const int vars1 = vars;
    const int vars2 = vars;
    QList<int> helpfull_original;
    QList<int> helpfull_original_subset;

    for(int i=0;i<vars;i++){
        helpfull_original.append(0);
        helpfull_original_subset.append(0);
    }

    for(int o=0;o<violated->size();o++){
        helpfull_sub_set = helpfull_per_constraint->at(violated->at(o));
        //helpfull_original[vars];

        for(int i=0;i<vars;i++){
            helpfull_original_subset[0]=0;
        }

        for(int i=0;i<helpfull_sub_set.size();i++){
            helpfull_original_subset[helpfull_sub_set.at(i)]=1;
            helpfull_original[helpfull_sub_set.at(i)]+=1;//this constraint increments the rating of the variable
        }

        /**********new section**********/
        //terminate if the all the set to 0s at the left canceled a constraint helpfull variables as those won't change
        int nots_at_the_left_biggest_id = 0;
        bool next=selected->size()>0;
        if(next){
            next = selected->at(nots_at_the_left_biggest_id)>=vars;
        }
        while(next){
            if(selected->at(nots_at_the_left_biggest_id)>=vars){//if 0 is imposed
                helpfull_original_subset[selected->at(nots_at_the_left_biggest_id)-vars] = 0;
                helpfull_original[selected->at(nots_at_the_left_biggest_id)-vars]=0;
            }else{
                //helpfull_original_subset[selected->at(nots_at_the_left_biggest_id)] = 0;
                //helpfull_original[selected->at(nots_at_the_left_biggest_id)]=0;
            }
            nots_at_the_left_biggest_id++;
            next=((selected->size()>nots_at_the_left_biggest_id) && (selected->at(nots_at_the_left_biggest_id)>=vars));
        }
        bool there_are_helpfull_vars_for_the_current_constraint = false;
        for(int i=0;i<helpfull_original_subset.length();i++){
            if(!(helpfull_original_subset[i]==0)){
                there_are_helpfull_vars_for_the_current_constraint = true;
            }
        }
        *terminate=!there_are_helpfull_vars_for_the_current_constraint;
        if(*terminate){
            break;
        }
        /******************************/

        for(int i=nots_at_the_left_biggest_id;i<selected->size();i++){//was for(int i = 0... before the new section
            if(selected->at(i)>=vars){//if 0 is imposed
                helpfull_original_subset[selected->at(i)-vars] = 0;
                helpfull_original[selected->at(i)-vars]=0;
            }else{// allredy selected (put to 1)
                //helpfull_original_subset[selected->at(i)] = 0;
                //helpfull_original[selected->at(i)]=0;
            }
        }

        /**********new section**********/
        there_are_helpfull_vars_for_the_current_constraint = false;
        for(int i=0;i<helpfull_original_subset.length();i++){
            if(!(helpfull_original_subset[i]==0)){
                there_are_helpfull_vars_for_the_current_constraint = true;
            }
        }
        backtrack_due_to_set_0s_canceling_a_constraint_helpfull_vaiabales=!there_are_helpfull_vars_for_the_current_constraint;
        if(backtrack_due_to_set_0s_canceling_a_constraint_helpfull_vaiabales){
            continue;//for debugging
        }
        /******************************/

        for(int i=nots_at_the_left_biggest_id;i<selected->size();i++){//wasn't before the new sections
            //remove any selected
            if(selected->at(i)<vars){// allredy selected (put to 1)
                helpfull_original_subset[selected->at(i)] = 0;
                helpfull_original[selected->at(i)]=0;
            }
        }

        helpfull_sub_set.clear();

    }

    /**********new section**********/
    if(!backtrack_due_to_set_0s_canceling_a_constraint_helpfull_vaiabales){
    /*******************************/
        //If 1 (NOT ALL) constraint could be helped we continue
        for(int i=0;i<vars;i++){
            if(helpfull_original[i]>=1){//violated->size()){
                //helpfull_sub_set->append(preference[i]);
                backtrack=false;//there are still helpfull variables to all violated constraints
            }
        }
    }

    int maximum_importance = 0;
    int maximum_importance_id = -1;
    helpfull->clear();

    for(int i=0;i<vars;i++){
        if(maximum_importance < helpfull_original[i]){
            maximum_importance = helpfull_original[i];
            maximum_importance_id = i;
        }
    }
    //mixing preference and maximum importance
    if(maximum_importance_id>=0 && maximum_importance>helpfull_original[preference->at(0)]){//
        helpfull->append(maximum_importance_id);
    }else{
        maximum_importance_id = -1;
    }

    for(int i=0;i<vars;i++){
        if((maximum_importance_id != preference->at(i)) && helpfull_original[preference->at(i)]>=1){
            //it is possible to put back the soso preference
            helpfull->append(preference->at(i));
            //backtrack=false;//there are still helpfull variables
        }
    }

    if(backtrack){
        return true;
    }
    return false;
}

bool balas_1959::termination_condition(QList<int> *selected, int const vars1){
    if(selected->size()==0){
        return false;
    }
    for(int i=0;i<selected->size();i++){
        if(selected->at(i)< vars1){
            return false;
        }
    }
    return true;
}


void balas_1959::feasible(QList<int> *selected,QList<QList<double>> *model, QList<int> *violated, QList<int> *current_feasable,double *current_feasable_objective_value,int const vars1, int const constraints1, bool *terminate){
    int const vars = vars1;
    int const constraints = constraints1;
    violated->clear();
    QList<int> x;
    for(int j=0;j<vars;j++){
        x.append(0);;
    }
    double may_be_possible_objective_value = 0;
    for(int j=0;j<selected->size();j++){
        if(selected->at(j)<vars){
            x[selected->at(j)]=1;
            may_be_possible_objective_value += model->at(0).at(selected->at(j));
        }
    }
    //bool current_constraint_violated=false;
    for(int i=1;i<constraints;i++){
        double constraint_sum = 0;
        for(int j=0;j<vars;j++){
            if(x[j]==1){
                constraint_sum += model->at(i).at(j);
            }
        }
        if(constraint_sum < model->at(i).at(vars)){
            violated->append(i-1);
        }
    }
    if(violated->size()==0 && *current_feasable_objective_value > may_be_possible_objective_value){
        current_feasable->clear();
        for(int j=0;j<selected->size();j++){
            current_feasable->append(selected->at(j));
        }
        *current_feasable_objective_value=may_be_possible_objective_value;
        if(abs(*current_feasable_objective_value) < 0.000001){
            *terminate = true;
        }

        vars_values.clear();
        for(int i=0;i<vars;i++){
            vars_values.append(0);
        }
        for(int i= 0; i<current_feasable->size();i++){
            if(current_feasable->at(i) >= vars){
                vars_values[current_feasable->at(i)-vars] = 0;
            }else{
                vars_values[current_feasable->at(i)] = 1;
            }
        }
        emit a_better_feasible_is_found(&vars_values,current_feasable_objective_value);
    }
}

void balas_1959::send_optimum(QList<int> *current_feasable,double *current_feasable_objective_value){

    emit an_optimum_is_found(current_feasable,current_feasable_objective_value);
    QThread::currentThread()->msleep(500);
}
