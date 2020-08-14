#include "bg_lib.h"

void printf(const char* fmt, ...);

/*
================
vmMain

This is the only way control passes into the module.
This must be the very first function compiled into the .qvm file
================
*/
int vmMain(int command, int arg0, int arg1, int arg2, int arg3, int arg4,
           int arg5, int arg6, int arg7, int arg8, int arg9, int arg10,
           int arg11)
{
    int test = 2;
    if (command == 0)
    {
        printf("Hello World!\n");
    }
    else
    {
        printf("Unknown command.\n");
    }

    test = test + 4;
    test = test + TestFunc();
    printf("test: %d\n",test);

    return 0;
}

int TestFunc(void) {
    printf("Test function!\n");
    return 0;
}

void printf(const char* fmt, ...)
{
    va_list argptr;
    char    text[1024];

    va_start(argptr, fmt);
    vsprintf(text, fmt, argptr);
    va_end(argptr);

    trap_Printf(text);
}
