#include "request.h"
#include "webserver.cpp"
#include "loadbalancer.h"

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <sstream>

using namespace std;

const int NUM_SERVERS = 5;
static int ps = 0;

request createRandomRequest()
{
    stringstream ips, ipd;
    request r;

    ips << rand() % 256 << "."
        << rand() % 256 << "."
        << rand() % 256 << "."
        << rand() % 256;

    ipd << rand() % 256 << "."
        << rand() % 256 << "."
        << rand() % 256 << "."
        << rand() % 256;

    r.source = ips.str();
    r.destination = ipd.str();
    r.processTime = rand() % 500;
    r.pid = ++ps;

    return r;
}

int main()
{
    srand(time(0));

    loadbalancer lb;

    for (int i = 0; i < 10; i++)
    {
        request r = createRandomRequest();
        lb.addRequest(r);
    }

    webserver webarray[NUM_SERVERS];

    for (int i = 0; i < NUM_SERVERS; i++)
    {
        webarray[i] = webserver(static_cast<char>('A' + i));
        webarray[i].addRequest(lb.getRequest(), lb.getTime());
    }

    while (lb.getTime() < 10000)
    {
        int currTime = lb.getTime();
        int size = lb.getSize();
        int serverIndex = currTime % NUM_SERVERS;

        if (webarray[serverIndex].isRequestDone(currTime))
        {
            request r = webarray[serverIndex].getRequest();

            cout << "Finished at " << currTime << " "
                 << webarray[serverIndex].getName()
                 << " pid " << r.pid << "/" << size
                 << " process time was " << r.processTime
                 << " processed request from " << r.source
                 << " to " << r.destination << endl;

            if (!lb.isRequestQueueEmpty())
            {
                webarray[serverIndex].addRequest(
                    lb.getRequest(), lb.getTime()
                );
            }
        }

        if (rand() % 100 == 0)
        {
            lb.addRequest(createRandomRequest());
        }

        lb.incTime();
    }

    return 0;
}
