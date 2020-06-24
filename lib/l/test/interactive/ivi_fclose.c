
/* $Id: ivi_fclose.c 25587 2020-06-24 02:28:42Z kobus $ */


#include "l/l_incl.h" 

int main(void)
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

