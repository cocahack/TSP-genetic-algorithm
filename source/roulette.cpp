#include "roulette.h"
#include <random>

roulette::roulette(const roulette &r)
    :shares{r.shares}
{

}

roulette &roulette::operator=(const roulette &r)
{
    shares = r.shares;
    return *this;
}

roulette::roulette(roulette&& r)
    :shares{r.shares}
{

}

roulette& roulette::operator=(roulette&& r)
{
    shares = r.shares;
    return *this;
}

int roulette::throw_ball()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double>
            range(.0, shares[shares.size()-1]);
    double shoot = range(gen);
    for(int i=0; i<shares.size();++i)
        if(shoot<shares[i]) return i;
}
