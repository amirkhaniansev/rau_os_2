#include "threadpool.h"

output_t work(input_t input)
{
    // do some stuff
    // return some output.
}

int main(int argc, char** argv)
{
    threadpool_t* threadpool = create_threadpool(4);

    execute(threadpool, work);
    execute(threadpool, work);
    execute(threadpool, work);
    execute(threadpool, work);

    sleep(100);

    destroy_threadpool(threadpool);
    
    return 0;
}