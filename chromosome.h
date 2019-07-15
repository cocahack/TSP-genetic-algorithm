#ifndef CHROMOSOME_H
#define CHROMOSOME_H

#include <QVector>

class chromosome
{
public:
    chromosome(const QVector<int>& my_gene, const double distance, const double fitness);
    chromosome(const QVector<int>& my_gene);
    chromosome(): distance{.0}, fitness{0} {}

    chromosome(const chromosome& chromo); //copy constructor
    chromosome& operator=(const chromosome& chromo); // copy operator

    chromosome(chromosome &&chromo); //move constructor
    chromosome& operator=(chromosome&& chromo); //move operator

    bool operator==(chromosome const& R);
    bool operator()(chromosome const & L, chromosome const & R) const
    { return L.fitness>R.fitness; }
    bool operator>(chromosome const & R) const
    { return this->fitness>R.fitness; }

    ~chromosome() {}
    //setter
    void set_distance(double distance){this->distance = distance;}
    void set_fitness(double fitness){ this->fitness = fitness;}
    void push_base(int base){ gene.push_back(base);}
    //getter
    int get_gene(int i) const {return gene[i];}
    double get_distance() const {return distance;}
    double get_fitness() const{return fitness;}
    int get_city (int i) const;

    void swap_gene();
    void swap_gene(int i, int j);
    int indexOf(int n);
    void slice_and_concat(int i, int j);

    int indexOf_variation(int val, int v=1);
    void clear_gene(){gene.clear();}\

    QVector<int> copy_gene() {return gene;}
private:
    QVector<int> gene;
    double distance;
    double fitness;
};

#endif // CHROMOSOME_H
