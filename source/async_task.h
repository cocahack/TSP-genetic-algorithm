#ifndef ASYNC_TASK_H
#define ASYNC_TASK_H
#include <QtCore>
#include "genetic_algorithm.h"

class async_thread : public QThread
{
    Q_OBJECT
public:
    async_thread(){}
    async_thread(genetic_algorithm* instance):g_instance{instance}, stop{false} {}
    void init(genetic_algorithm* instance);
    void run();
    bool stop;
signals:
    void UpdateGui(chromosome);
    void UpdateStatus(int, int);
private:
    genetic_algorithm* g_instance;
};
#endif // ASYNC_TASK_H
