#include "kernel/types.h"
#include "user/user.h"

#define RD 0
#define WR 1

int lpipe_first_data(int lpipe[2], int *dst)
{
    if (read(lpipe[RD], dst, sizeof(int)) == sizeof(int))
    {
        printf("prime %d\n", *dst);
        return 0;
    }
    return -1;
}

void transmit_data(int lpipe[2], int rpipe[2], int first)
{
    int data;
    while (read(lpipe[RD], &data, sizeof(int)) == sizeof(int))
    {
        if (data % first != 0)
        {
            write(rpipe[WR], &data, sizeof(int));
        }
    }
    close(lpipe[RD]);
    close(rpipe[WR]);
}

void primes(int lpipe[2])
{
    close(lpipe[WR]);
    int first;
    if (lpipe_first_data(lpipe, &first) == 0)
    {
        int p[2];
        pipe(p);
        transmit_data(lpipe, p, first);
        int pid = fork();
        if (pid == 0)
        {
            primes(p);
        }
        else
        {
            wait(0);
            close(p[RD]);
        }
    }
    exit(0);
}

int main(int argc, char const *argv[])
{
    int p[2];

    pipe(p);

    for (int i = 2; i <= 35; ++i)
    {
        write(p[WR], &i, sizeof(int));
    }

    int pid = fork();

    if (pid == 0)
    {
        primes(p);
    }
    else
    {
        close(p[WR]);
        close(p[RD]);
        wait(0);
    }
    exit(0);
}
