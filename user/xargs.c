#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

#define MAXSZ 1024

int main(int argc, char *argv[])
{
    if (argc - 1 >= MAXARG)
    {
        fprintf(2, "xargs: too many arguments.\n");
    }
    sleep(5);
    // 
    char buf[MAXSZ];
    read(0, buf, MAXSZ);

    char *xargv[MAXARG];
    int xargc = 0;
    for (int i = 1; i < argc; ++i)
    {
        xargv[xargc++] = argv[i];
    }

    char *p = buf;
    for (int i = 0; i < MAXSZ; ++i)
    {
        if (buf[i] == '\n')
        {
            if (fork() == 0)
            {
                buf[i] = 0;
                xargv[xargc++] = p;
                xargv[xargc++] = 0;
                exec(argv[1], xargv);
                exit(1);
            }
            else
            {
                wait(0);
            }
            p = &buf[i + 1];
        }
    }

    exit(0);

    // char line[MSGSIZE];
    // char *params[MAXARG];
    // int n = 0, args_index = 0;
    // char *cmd = argv[1];

    // for (int i = 1; i < argc; ++i)
    // {
    //     params[args_index++] = argv[i];
    // }

    // // for (int i = 0; i < args_index; ++i)
    // // {
    // //     printf("%s", params[i]);
    // // }

    // while ((n = read(0, line, 1024)) > 0)
    // {
    //     if (fork() == 0)
    //     {
    //         char *arg = (char *) malloc(sizeof(line));
    //         int index = 0;
    //         for (int i = 0; i < n; ++i)
    //         {
    //             if (line[i] == ' ' || line[i] == '\n')
    //             {
    //                 arg[index] = 0;
    //                 params[args_index++] = arg;
    //                 index = 0;
    //                 arg = (char *) malloc(sizeof(line));
    //             }
    //             else
    //             {
    //                 arg[index++] = line[i];
    //             }
    //         }
    //         arg[index] = 0;
    //         params[args_index] = 0;
    //         // for (int i = 0; i < args_index; ++i)
    //         // {
    //         //     printf("%s", params[i]);
    //         // }
    //         exec(cmd, params);
    //     }
    //     else
    //     {
    //         wait(0);
    //     }
    // }
    // exit(0);
}