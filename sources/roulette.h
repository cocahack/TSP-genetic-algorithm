#ifndef ROULETTE_H
#define ROULETTE_H
#include <QVector>

class roulette
{
public:
    roulette() {}
    roulette(const QVector<double> s):shares{s}{}
    ~roulette() {}

    roulette(const roulette& r); //copy constructor
    roulette& operator=(const roulette& r); //copy operator

    roulette(roulette&& r); //move constructor
    roulette& operator=(roulette&& r); //move operator

    //setter
    void push_proportion(double proportion){shares.push_back(proportion);}

    int throw_ball();

    void clear_shares(){shares.clear();}
private:
    QVector<double> shares;
};

#endif // ROULETTE_H
