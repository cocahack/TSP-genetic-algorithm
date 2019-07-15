#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "genetic_algorithm.h"
#include "async_task.h"
#include <QPainter>
#include <QDebug>
#include <random>
#include <math.h>
#include <QString>
#include <chrono>
#include <QVector>
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Genetic Algorithm");
    ui->start_btn->setEnabled(false);
    ui->stop_btn->setEnabled(false);
    ui->mutation_value->setMinimum(0.5);
    ui->mutation_value->setMaximum(30.0);
    ui->mutation_value->setSingleStep(0.5);
    ui->mutation_value->setValue(0.5);
    mutation_probability = .0;

    ui->individuals_value->setMinimum(30);
    ui->individuals_value->setMaximum(100);
    ui->individuals_value->setSingleStep(2);
    ui->individuals_value->setValue(30);
    individuals = 0;

    ui->crossover_value->setMinimum(80.0);
    ui->crossover_value->setMaximum(100.0);
    ui->crossover_value->setSingleStep(0.5);
    ui->crossover_value->setValue(90.0);
    crossover_probability = .0;

    seted_supreme = false;
    seted_points = false;
    stop_btn_pressed = false;
    current_generation =1;
    mutation_times = 0;
    g_instance = nullptr;
    th = nullptr;

    int side = SIDE;
    int window_width = this->width();
    int window_height = this->height();
//    mainframe = QRect(window_height/2-side/2, window_width/2-side/2,side,side);
    mainframe = QRect(50, 50, WIDTH, HEIGHT);
    offset_point = QPoint(mainframe.left(),mainframe.top());
    for(int i=0; i<WIDTH; ++i)
        for(int j=0; j<HEIGHT; ++j)
            alloc_state[i][j] = false;

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    int i,city_size;
    QPen pen;
    QPainter painter(this);
    painter.drawRect(mainframe);
    if(seted_points){

        pen.setWidth(5);
        pen.setColor(Qt::red);
        painter.setPen(pen);
        city_size = cities.size();
        painter.drawPoint(cities[0]);
        pen.setColor(Qt::black);
        painter.setPen(pen);
        for(i=1; i<city_size; ++i)
            painter.drawPoint(cities[i]);
    }

    if(seted_supreme){
        int m,n;
        pen.setWidth(1);
        pen.setColor(Qt::red);
        painter.setPen(pen);
        for(i=0; i<city_size-1; ++i){
            m = supreme.get_gene(i);
            n = supreme.get_gene(i+1);
            painter.drawLine(cities[m],cities[n]);
        }
        m = supreme.get_gene(i);
        n = supreme.get_gene(0);
        painter.drawLine(cities[m],cities[n]);
    }
}

void MainWindow::on_add_city_btn_pressed()
{
    ui->start_btn->setEnabled(true);
    int i, x, y, o_x= offset_point.x(), o_y = offset_point.y();
    for(i=0; i<50; ++i){
        do{
            x = get_random(o_x+1, o_x+WIDTH-1);
            y = get_random(o_y+1, o_y+HEIGHT-1);
        }while(alloc_state[x-o_x][y-o_y]);
        alloc_state[x-o_x][y-o_y] = true;
        cities.push_back(QPoint(x,y));
    }
    seted_points = true;
    this->update();
}

int MainWindow::get_random(int min, int max)
{
    random_device rn;
    mt19937_64 rnd( rn() );
    uniform_int_distribution<int> range(min, max);

    return range( rnd );
}

void MainWindow::onUpdateGui(chromosome c)
{
    if(supreme.get_distance()==0){
        this->supreme = c;
        this->seted_supreme = true;
    }
    if(c.get_distance() < this->supreme.get_distance())
       this->supreme = c ;


    this->repaint();
}

void MainWindow::onUpdateStatus(int gen, int mut)
{
    ui->status_label->setText(
                "There are " + QString::number(cities.size()) + " cities in the map, "
                +"the " + QString::number(gen) + "th generation with "
                + QString::number(mut) + " times of mutation. best value: "
                + QString::number(sqrt(supreme.get_distance()))
                );
}

void MainWindow::on_start_btn_pressed()
{
    if(g_instance!=nullptr) delete g_instance;
    if(th!=nullptr){
        th->terminate();
        delete th;
    }
    ui->add_city_btn->setEnabled(false);
    ui->start_btn->setEnabled(false);
    ui->stop_btn->setEnabled(true);
    // TODO: execute Genetic algorithm
    individuals = ui->individuals_value->value();
    mutation_probability = ui->mutation_value->value();
    crossover_probability = ui->crossover_value->value();
    if(g_instance!=nullptr) delete g_instance;
    g_instance = new genetic_algorithm(this,cities);
    if(th!=nullptr) delete th;
    th = new async_thread(g_instance);
    connect(th, SIGNAL(UpdateGui(chromosome)), this, SLOT(onUpdateGui(chromosome)));
    connect(th, SIGNAL(UpdateStatus(int,int)), this, SLOT(onUpdateStatus(int,int)));
    th->start();
}

void MainWindow::on_stop_btn_pressed()
{
    th->stop = true;
    ui->add_city_btn->setEnabled(true);
    ui->start_btn->setEnabled(true);
    ui->stop_btn->setEnabled(false);
}

void MainWindow::on_add_city_btn_clicked()
{

}
