#include <stdlib.h>
#include <string.h>

#include "lock.h"

int main(int argc, char** argv)
{
    sync_header_t* header = get_shared_mutex();
    if (header == NULL)
        return 1;
           
    enter_critical_section(header);
    cleanup(header, argc == 2 && strcmp(argv[1], "-c") == 0);

    return 0;
}