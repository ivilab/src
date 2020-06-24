
/* $Id: count.c 25587 2020-06-24 02:28:42Z kobus $ */



#include "l/l_incl.h" 

int main(void)
{
    int  res;
    char line[ 1000 ];



    EPE(res = count_real_lines(stdin)); 
    dbi(res); 

    EPE(res = count_tokens_in_file(stdin));
    dbi(res); 

    EPE(res = gen_count_tokens_in_file(stdin, " ,"));
    dbi(res); 

    EPE(BUFF_FGET_LINE(stdin, line)); 

    EPE(res = count_real_lines(stdin)); 
    dbi(res); 

    EPE(res = count_tokens_in_file(stdin));
    dbi(res); 

    EPE(res = gen_count_tokens_in_file(stdin, " ,"));
    dbi(res); 

    return EXIT_SUCCESS; 
}

