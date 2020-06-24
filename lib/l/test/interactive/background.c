
/* $Id: background.c 25587 2020-06-24 02:28:42Z kobus $ */



#include "l/l_incl.h" 


int main(void)
{
    int   in_background;
    FILE* fp;



    fp = fopen("xxx", "w");
    fprintf(fp, "Before\n");
    fflush(fp);

    in_background = is_in_background();
    fprintf(fp, "After\n");
    fflush(fp);

    fprintf(stdout, "in_background is %d\n", in_background);

    if (in_background)
    {
        fprintf(stderr, "I'm in the background\n");
    }
    else 
    {
        fprintf(stderr, "I'm in the foreground\n");
    }

    return EXIT_SUCCESS; 
}

