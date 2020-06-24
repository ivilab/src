
/* $Id: path.c 25587 2020-06-24 02:28:42Z kobus $ */


#include "l/l_incl.h" 

int main(void)
{
    char line[ 3000 ]; 
    char path[ 100 ]; 
    int res; 


    ivi_l_set("debug_level", "1"); 

    while ((res=BUFF_STDIN_GET_LINE("file> ",line)) != EOF) 
    {
        EPETE(res);
        dbs(line); 
        dbi(res); 
        EPE(BUFF_REAL_PATH(line, path));
        dbs(path); 
    }

    return EXIT_SUCCESS; 
}


