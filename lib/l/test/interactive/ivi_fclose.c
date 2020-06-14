
/* $Id: ivi_fclose.c 25499 2020-06-14 13:26:04Z kobus $ */


#include "l/l_incl.h" 

/*ARGSUSED*/
int main(int argc, char **argv)
{
    FILE* fp; 


    NPETE(fp = ivi_fopen("test_file", "w")); 

    pso("Close of test_file"); 
    EPE(ivi_fclose(fp));

    pso("SECOND close of test_file"); 
    EPE(ivi_fclose(fp));

    pso("Close of stdin");
    EPE(ivi_fclose(stdin));

    pso("Close of stdout");
    EPE(ivi_fclose(stdout));

    pso("Close of stderr");
    EPE(ivi_fclose(stderr));

    return EXIT_SUCCESS; 
}

