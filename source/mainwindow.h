#ifndef MAINWINDOW_H
#define MAINWINDOW_H

class genetic_algorithm;
class async_thread;
#include <QMainWindow>
#include <QWidget>
#include <QVector>
#include "chromosome.h"
#include "async_task.h"


#define SIDE 700
#define WIDTH 1266
#define HEIGHT 700

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    int get_random(int min, int max);

    void set_sbool(){this->seted_supreme = true;}
    void set_supreme(const chromosome& supreme){this->supreme = supreme;}
    int get_chromosize(){return individuals;}
    double get_mutation_prob(){return mutation_probability;}
    double get_crossover_prob(){return crossover_probability;}
protected:
    void paintEvent(QPaintEvent *event);
public slots:
    void onUpdateGui(chromosome);
    void onUpdateStatus(int, int);
private slots:
    void on_add_city_btn_pressed();
    void on_start_btn_pressed();
    void on_stop_btn_pressed();
    void on_add_city_btn_clicked();

private:
    Ui::MainWindow *ui;
    QRect mainframe; // 프레임
    QPoint offset_point; // 프레임 시작

    QVector<QPoint> cities;
//    bool alloc_state[SIDE][SIDE];
    bool alloc_state[WIDTH][HEIGHT];

    double mutation_probability;
    double crossover_probability;
    int individuals;
    bool stop_btn_pressed;
    bool seted_points;
    bool seted_supreme;
    int current_generation;
    int mutation_times;

    chromosome supreme;

    genetic_algorithm* g_instance;
    async_thread* th;
};

#endif // MAINWINDOW_H
