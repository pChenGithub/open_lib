#include "misc_opt.h"
#include <sys/syscall.h>
#include <unistd.h>

#if 1
pid_t gettid()
{
     return syscall(SYS_gettid);
}
#endif

