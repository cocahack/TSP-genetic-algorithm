#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H
class MainWindow;
#include "chromosome.h"
#include "roulette.h"
#include "mainwindow.h"
#include <QVector>
#include <QList>

struct desc
{
template<class T>
    bool operator()(T const &a, T const &b) const {return a>b;}
};

class genetic_algorithm
{
public:
    genetic_algorithm(){}
    genetic_algorithm(MainWindow *wptr, const QVector<QPoint> &cities);
    ~genetic_algorithm();

    void init(MainWindow* wptr, const QVector<QPoint> &cities);
    void before_tour();
    bool tour();
    chromosome get_supreme() { return best_chromo; }

    int generation;
    int mutation_times;
private:
    MainWindow *winptr;
    chromosome best_chromo;
    int best_generation;
    QVector<chromosome> individuals;

    int city_size;
    int population;
    roulette wheel;
    double mutation_probability;
    double crossover_probability;
    double** weight;

    void calculate_weight(const QVector<QPoint>& cities);
    void set_first_generation();
    void fitness_func();
    void set_supreme();
    int selection();
    void crossover();
    void nnx(QVector<chromosome> &next_individuals, const chromosome &c1, const chromosome &c2);
    void other_crossover();
    void do_crossover(QVector<chromosome> &next_gen, int x, int y);
    chromosome get_child(bool isnext, chromosome &x, chromosome& y);
    void mutation();
    void mutate_chromosome(int i);
    void do_mutate(chromosome &chromo);
    void push_mutate(chromosome& chromo);
    chromosome mutate_chromosome(chromosome best);
    bool check_termination();
};


#endif // GENETIC_ALGORITHM_H
