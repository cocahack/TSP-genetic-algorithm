#include "genetic_algorithm.h"
#include <QDebug>
#include <random>
#include <algorithm>
#include <QList>
using namespace std;

double get_random(double min, double max)
{
    random_device rn;
    mt19937_64 rnd(rn());
    uniform_real_distribution<double> range(min, max);

    return range(rnd);
}

genetic_algorithm::genetic_algorithm(MainWindow *wptr, const QVector<QPoint>& cities )
    : winptr{wptr}, weight{nullptr}
{
    init(wptr, cities);
}

genetic_algorithm::~genetic_algorithm()
{
    for(int i=0; i<city_size; ++i)
        delete[] weight[i];
    delete[] weight;
}

void genetic_algorithm::init(MainWindow *wptr, const QVector<QPoint> &cities)
{
    winptr = wptr;
    weight = nullptr;
    city_size = cities.size();
    population = winptr->get_chromosize();
    calculate_weight(cities);
    set_first_generation();
    generation = 1;
    best_generation = 1;
    mutation_times = 0;
    mutation_probability = wptr->get_mutation_prob();
    crossover_probability = wptr->get_crossover_prob();
}

void genetic_algorithm::calculate_weight(const QVector<QPoint>& cities)
{
    int i,j;
    if(weight!=nullptr){
        for(i=0; i<city_size; ++i)
            delete[] weight[i];
        delete[] weight;
    }
    weight = new double*[city_size];
    for(i=0; i<city_size; ++i)
        weight[i] = new double[city_size];
    for(i=0; i<city_size; ++i)
        for(j=0; j<city_size; ++j)
            weight[i][j] = pow(abs(cities[i].x()-cities[j].x()),2)
                    +pow(abs(cities[i].y()-cities[j].y()),2);
}

void genetic_algorithm::set_first_generation()
{
    chromosome chromo;
    QVector<int> seq_city;
    random_device rd;
    mt19937 g(rd());
    int i,j;
    for(i=0; i<city_size; ++i)
        seq_city.push_back(i);
    for(i=0; i<population; ++i){
        shuffle(seq_city.begin(), seq_city.end(), g);
        chromo = chromosome();
        for(j=0; j<city_size; ++j){
            chromo.push_base(seq_city[j]);
        }
        individuals.push_back(chromo);
    }
}

void genetic_algorithm::fitness_func()
{
    int i,j,m,n;
    double distance, total = .0, cumul = .0;
    for(i=0; i< population; ++i){
        distance = .0;
        for(j=0; j<city_size-1; ++j){
            m = individuals[i].get_gene(j);
            n = individuals[i].get_gene(j+1);
            distance+=weight[m][n];
        }
        m = individuals[i].get_gene(j);
        n = individuals[i].get_gene(0);
        distance+=weight[m][n];
        individuals[i].set_distance(distance);
        individuals[i].set_fitness(1/distance);
    }
    sort(individuals.begin(), individuals.end(), desc());
    for(i=0; i<population/2; ++i)
        total += individuals[i].get_fitness();
    for(i=0; i<population/2; ++i){
        cumul += individuals[i].get_fitness()/total;
        wheel.push_proportion(cumul);
    }
}

void genetic_algorithm::set_supreme()
{
    int shotest_path = 0, i;
    for(i=0; i< individuals.size(); ++i)
        if(individuals[shotest_path].get_distance()
                >individuals[i].get_distance())
            shotest_path = i;
    if(best_chromo.get_distance()==0)
        best_chromo = individuals[shotest_path];
    else if(best_chromo.get_fitness()<individuals[shotest_path].get_fitness()){
        best_chromo = individuals[shotest_path];
        best_generation = generation;
    }
}

int genetic_algorithm::selection()
{
    return wheel.throw_ball();
}

void genetic_algorithm::crossover()
{
    int elite = 3;
    QList<chromosome> copy_individuals;
    QVector <chromosome> next_individuals;
    chromosome copy = best_chromo;
    int i,j,m,n;
    next_individuals.push_back(best_chromo);
    next_individuals.push_back(best_chromo);
    mutate_chromosome(1);
    do_mutate(copy);
    next_individuals.push_back(copy);
    copy = best_chromo;
    push_mutate(copy);
    next_individuals.push_back(copy);
//    for(i=0; i<elite; ++i)
//        next_individuals.push_back(mutate_chromosome(best_chromo));

    for(i=0; i<4; ++i)
        for(j=0; j<(population-elite)/2; ++j)
            copy_individuals.push_back(individuals[j]);


    while(copy_individuals.size()!=0){
        m = n = winptr->get_random(0,copy_individuals.size()-1);
        while(m==n) n = winptr->get_random(0,copy_individuals.size()-1);
        nnx(next_individuals, copy_individuals[m], copy_individuals[n]);
        copy_individuals.removeAt(m);
        if(m<n) copy_individuals.removeAt(n-1);
        else copy_individuals.removeAt(n);
    }
    individuals = next_individuals;
}

void genetic_algorithm::nnx(QVector <chromosome>& next_individuals, const chromosome& c1, const chromosome& c2)
{
    QList<int> origin, unvisited_vertex;
    int i,j, vertex, c1_current, c2_current, c1_next, c2_next;
    double distance1, distance2;
    bool success_tour = false;
    chromosome descendant;
    for(i=0; i<city_size; ++i)
        origin.append(i);

    for(i=0; i<city_size; ++i){
        unvisited_vertex = origin;
        vertex = unvisited_vertex.at(i); // start v0
        unvisited_vertex.removeAt(unvisited_vertex.indexOf(vertex));
    //    union_graph(descendant, unvisited_vertex, chromoparam, vertex, c1, c2);
        while(unvisited_vertex.size()!=0){
            if(descendant.indexOf(vertex)!=-1){
                vertex = -1;
                descendant.clear_gene();
                break;
            }
            descendant.push_base(vertex);
            c1_current = c1.get_city(vertex); // index of c1.gene that has same vertex number;
            c2_current = c2.get_city(vertex); // index of c2.gene that has same vertex number;
            c1_next = c1.get_gene(c1_current+1==city_size?0:c1_current+1);
            c2_next = c2.get_gene(c2_current+1==city_size?0:c2_current+1);

            if(unvisited_vertex.indexOf(c1_next)==-1
                    && unvisited_vertex.indexOf(c2_next)==-1){
                vertex = -1;
                descendant.clear_gene();
                break;
            }
            if(unvisited_vertex.indexOf(c1_next)==-1){
                vertex = c2_next;
                unvisited_vertex.removeAt(unvisited_vertex.indexOf(c2_next));
            }

            else if(unvisited_vertex.indexOf(c2_next)==-1){
                vertex = c1_next;
                unvisited_vertex.removeAt(unvisited_vertex.indexOf(c1_next));
            }
            else{
                distance1 = weight[vertex][c1_next];
                distance2 = weight[vertex][c2_next];
                if(distance1<distance2){
                    vertex = c1_next;
                    unvisited_vertex.removeAt(unvisited_vertex.indexOf(c1_next));
                }
                else{
                    vertex = c2_next;
                    unvisited_vertex.removeAt(unvisited_vertex.indexOf(c2_next));
                }
            }
        }
        if(vertex==-1) break;
        if(vertex!=-1){
            descendant.push_base(vertex);
            success_tour = true;
            break;
        }
    }
    if(success_tour)
        next_individuals.push_back(descendant);
    else{
       if(c1>c2) next_individuals.push_back(c1);
       else next_individuals.push_back(c2);
    }
}

void genetic_algorithm::other_crossover()
{
    int elite = 4;
    QVector <chromosome> next_individuals;
    QVector <int> select_index;
    chromosome copy = best_chromo;
    int i,j,m,n;
    next_individuals.push_back(best_chromo);
    next_individuals.push_back(best_chromo);
    mutate_chromosome(1);
    do_mutate(copy);
    next_individuals.push_back(copy);
    copy = best_chromo;
    push_mutate(copy);
    next_individuals.push_back(copy);

    for(i=elite; i<population; ++i)
        next_individuals.push_back(individuals[selection()]);

    for(i=0; i<population; ++i)
        if(get_random(.0,99.0)<crossover_probability)
            select_index.push_back(i);
    random_device rd;
    mt19937_64 gen(rd());
    shuffle(select_index.begin(), select_index.end(), gen);

    for(i=0, j=select_index.size()-1; i<j; i+=2)
        do_crossover(next_individuals, select_index[i], select_index[i+1]);
    individuals = next_individuals;
}

void genetic_algorithm::do_crossover(QVector<chromosome>& next_gen, int x, int y)
{
    chromosome c1,c2,cx,cy;
    cx = individuals[x]; cy = individuals[y];
    c1 = get_child(true, cx,cy);
    c2 = get_child(false, cy,cx);
    next_gen[x] = c1;
    next_gen[y] = c2;
}

chromosome genetic_algorithm::get_child(bool isnext, chromosome &x, chromosome &y)
{
    QVector<int> px, py;
    int i, c, dx, dy, variation;
    px = x.copy_gene();
    py = y.copy_gene();
    chromosome solution;
    c = px[winptr->get_random(0,city_size-1)];
    solution.push_base(c);

    if(isnext) variation = 1;
    else variation -1;

    while(px.size()>1){
        if(isnext){
            dx = px.indexOf(c)+1==px.size()?px[0]:px[px.indexOf(c)+1];
            dy = py.indexOf(c)+1==py.size()?py[0]:py[py.indexOf(c)+1];
        }
        else{
            dx = px.indexOf(c)-1==-1?px[px.size()-1]:px[px.indexOf(c)-1];
            dy = py.indexOf(c)-1==-1?py[py.size()-1]:py[py.indexOf(c)-1];
        }
        px.removeOne(c);
        py.removeOne(c);
        c = weight[c][dx] < weight[c][dy] ? dx : dy;
        solution.push_base(c);
    }
    return solution;
}

void genetic_algorithm::mutation()
{
    int i;
    for(i=0; i<population; ++i){
        if(get_random(.0,99.0) < mutation_probability)
            if(get_random(.0, 99.0)< 0.5)
                push_mutate(individuals[i]);
            else
                do_mutate(individuals[i]);
//            mutate_chromosome(i);
    }
}

void genetic_algorithm::mutate_chromosome(int i)
{
    individuals[i].swap_gene();
    this->mutation_times++;
}

void genetic_algorithm::do_mutate(chromosome& chromo)
{
    int m,n,i,j;
    do{
        m = winptr->get_random(0,city_size-3);
        n = winptr->get_random(0,city_size-1);
    }while(m>=n);

    for(i=0, j=(n-m+1)>>1; i<j; ++i)
        chromo.swap_gene(m+i, n-i);
    this->mutation_times++;
}

void genetic_algorithm::push_mutate(chromosome &chromo)
{
    int m,n;
    do{
        m = winptr->get_random(0,city_size>>1);
        n = winptr->get_random(0,city_size-1);
    }while(m>=n);

    chromo.slice_and_concat(m,n);
    this->mutation_times++;
}

chromosome genetic_algorithm::mutate_chromosome(chromosome best)
{
    best.swap_gene();
    return best;
}

bool genetic_algorithm::check_termination()
{
    if(generation - best_generation > 5000)
        return true;
    return false;
}

void genetic_algorithm::before_tour()
{
    fitness_func();
    set_supreme();
}

bool genetic_algorithm::tour()
{
//    crossover();
    other_crossover();
    this->generation++;
    mutation();
    return check_termination();
}
