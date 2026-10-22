#include "loadbalancer.h"

int loadbalancer::getTime()
{
    return systemTime;
}

void loadbalancer::incTime()
{
    systemTime++;
}

void loadbalancer::addRequest(request r)
{
    requestQueue.push(r);
    incTime();
}

request loadbalancer::getRequest()
{
    incTime();

    if (!requestQueue.empty())
    {
        request r = requestQueue.front();   // queue
        // request r = requestQueue.top(); // stack and priority_queue

        requestQueue.pop();
        return r;
    }

    request r{};
    return r;
}

bool loadbalancer::isRequestQueueEmpty()
{
    return requestQueue.empty();
}

int loadbalancer::getSize()
{
    return requestQueue.size();
}
