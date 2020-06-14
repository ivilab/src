
/* $Id: ivi_mkdir.c 25499 2020-06-14 13:26:04Z kobus $ */


#include "l/l_incl.h" 

int main(void)
{
    char dir[ MAX_FILE_NAME_SIZE ];
    


    while (BUFF_STDIN_GET_LINE("dir> ", dir) != EOF)
    {
        EPE(ivi_mkdir(dir));
    }

    return EXIT_SUCCESS; 
} 


/*  /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\   */

