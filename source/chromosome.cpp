#include "chromosome.h"
#include <random>
#include <vector>


int get_random(int min, int max)
{
    std::random_device rn;
    std::mt19937 rnd(rn());
    std::uniform_int_distribution<int> range(min, max);

    return range(rnd);
}

chromosome::chromosome
(const QVector<int> &my_gene, const double d, const double f)
    : chromosome(my_gene)
{
    this->distance = d;
    this->fitness = f;
}

chromosome::chromosome(const QVector<int> &my_gene)
{
    this->gene = my_gene;
}

chromosome::chromosome(const chromosome &chromo)
    :gene{chromo.gene}, distance{chromo.distance}, fitness{chromo.fitness}
{

}

chromosome &chromosome::operator=(const chromosome &chromo)
{
    this->gene = chromo.gene;
    this->distance = chromo.distance;
    this->fitness = chromo.fitness;
    return *this;
}

chromosome::chromosome(chromosome&& chromo)
    :gene{chromo.gene}, distance{chromo.distance}, fitness{chromo.fitness}
{

}

chromosome &chromosome::operator=(chromosome &&chromo)
{
    this->gene = chromo.gene;
    this->distance = chromo.distance;
    this->fitness = chromo.fitness;
    return *this;
}

bool chromosome::operator==(const chromosome &R)
{
    if(this->distance!=R.distance) return false;
    if(this->fitness!=R.fitness) return false;
    for(int i=0; i<gene.size(); ++i)
        if(this->gene[i]!=R.gene[i]) return false;
    return true;
}

int chromosome::get_city(int i) const
{
    for(int j=0; j<gene.size(); ++j)
        if(gene[j]==i) return j;
    return -1;
}

void chromosome::swap_gene()
{
    int i,j,temp;
    i = j = get_random(0, gene.size()-1);
    while(i==j) j = get_random(0, gene.size()-1);
    temp = gene[i];
    gene[i] = gene[j];
    gene[j] = temp;
}

void chromosome::swap_gene(int i, int j)
{
    int temp;
    temp = gene[i];
    gene[i] = gene[j];
    gene[j] = temp;
}

int chromosome::indexOf(int n)
{
    return gene.indexOf(n);
}

void chromosome::slice_and_concat(int i, int j)
{
    if(i<j){
        std::vector<int> copy;
        int m;
        for(m=0; m<gene.size(); ++m)
            copy.push_back(gene[m]);
        std::vector<int> v1(copy.begin(), copy.begin()+i);
        std::vector<int> v2(copy.begin()+i, copy.begin()+j);
        std::vector<int> v3(copy.begin()+j, copy.end());

        copy.clear();
        copy.insert(copy.end(), v2.begin(), v2.end());
        copy.insert(copy.end(), v1.begin(), v1.end());
        copy.insert(copy.end(), v3.begin(), v3.end());

        gene.clear();
        for(m=0; m<copy.size(); ++m)
            gene.push_back(copy[m]);
    }
}

int chromosome::indexOf_variation(int val, int v)
{
    int returnval = gene.indexOf(val);
    returnval +=v;
    if(returnval>=gene.size()) returnval = gene[0];
    else if(returnval<0) returnval = gene[gene.size()-1];
    else returnval = gene[returnval];
    return returnval;
}

