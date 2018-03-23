#ifndef CALCULATEFORCES_H
#define CALCULATEFORCES_H
#include <QPointF>
#include <mutex>
#include <condition_variable>
#include <unordered_map>

class QGraphicsItem;

class CalculateForces
{
public:
    CalculateForces(std::unordered_map<QGraphicsItem *, QPointF> &balls_,
                    std::mutex& mut_,
                    std::condition_variable& cond_var_);

    void operator()();

private:
    std::mutex& mut;
    std::condition_variable& cond_var;
    std::unordered_map<QGraphicsItem*, QPointF>& balls;
};

#endif // CALCULATEFORCES_H
