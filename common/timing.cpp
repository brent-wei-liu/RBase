#include "pch.h"


Timing::Timing()
{
}

Timing::~Timing()
{
}

void Timing::startClock(){
    gettimeofday(&tt, 0);
    secs=tt.tv_sec;
    usecs=tt.tv_usec;
    start = clock();
};

unsigned long Timing::elapsed(){
    unsigned long time = ((unsigned long long)clock()-(unsigned long long)start)*1000/CLOCKS_PER_SEC;
    return time;
};


unsigned long Timing::stopClock(){
    gettimeofday(&tt, 0);
    secs = tt.tv_sec-secs;
    usecs = tt.tv_usec-usecs;
//    unsigned long time1 = ((unsigned long) 1000*secs+(usecs/1000));
    unsigned long time2 = ((unsigned long long)clock()-(unsigned long long)start)/(CLOCKS_PER_SEC/1000);
//    cerr << "timeofday elapsed time = " << time1 << endl;
    cerr << "clock elapsed time = " << time2 << endl;
    return time2;
};
