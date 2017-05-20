#include "async_task.h"
#include <QtCore>

void async_thread::init(genetic_algorithm *instance)
{
    g_instance = instance;
    stop = false;
}

void async_thread::run()
{
    QMutex mutex;
    /*int iter = 0, max = 0;*/
    bool termination = false;
    while(!termination){
        while(stop){
            this->msleep(100);
        }
        mutex.lock();
        g_instance->before_tour();
        mutex.unlock();
        emit UpdateGui(g_instance->get_supreme());
        emit UpdateStatus(g_instance->generation, g_instance->mutation_times);
        mutex.lock();
        termination = g_instance->tour();
        mutex.unlock();
    }

}
