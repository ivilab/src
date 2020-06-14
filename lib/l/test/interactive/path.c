
/* $Id: path.c 25499 2020-06-14 13:26:04Z kobus $ */


#include "l/l_incl.h" 

/*ARGSUSED*/
int main(int argc, char **argv)
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


