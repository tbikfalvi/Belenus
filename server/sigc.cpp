#include <qglobal.h>
#include "sigc.h"

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>


int installSIGCHandler()
{
    signal(SIGINT, sigc_handler);

    return true;
}








