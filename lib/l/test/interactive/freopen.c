
/* $Id: freopen.c 25587 2020-06-24 02:28:42Z kobus $ */


#include "l/l_incl.h" 

int main(void)
{
    char  line[ 30 ];
    FILE* fp1;
    char  name[ 200 ];


    term_get_line("r> ",line, 30); 
    NPETE(fp1 = ivi_fopen(line,"r")); 

    BUFF_GET_FD_NAME(fileno(stdout), name); 
    dbs(name); 
    BUFF_GET_FD_NAME(fileno(fp1), name); 
    dbs(name); 

    term_get_line("w> ",line, 30); 
    NPETE(ivi_freopen(line,"w", stdout)); 

    term_get_line("r> ",line, 30); 
    NPETE(ivi_freopen(line,"r", fp1)); 

    pso("HELLO WORLD"); 

    BUFF_GET_FD_NAME(fileno(stdout), name); 
    dbs(name); 

    BUFF_GET_FD_NAME(fileno(fp1), name); 
    dbs(name); 

    return EXIT_SUCCESS; 
}

