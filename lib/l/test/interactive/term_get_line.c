
/* $Id: term_get_line.c 25499 2020-06-14 13:26:04Z kobus $ */


#include "l/l_incl.h" 


/*ARGSUSED*/
int main(int argc, char **argv)
{
    char line[ 3000 ]; 
    int  res; 


    while ((res=BUFF_STDIN_GET_LINE("abc>",line)) != EOF)
    {
        EPETE(res);
        ivi_fprintf(stderr, "%s\n", line);
    }

    return EXIT_SUCCESS; 
}

