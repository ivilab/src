
/* $Id: term_get_line.c 25587 2020-06-24 02:28:42Z kobus $ */


#include "l/l_incl.h" 


int main(void)
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

