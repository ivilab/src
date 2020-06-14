
/* $Id: fopen.c 25499 2020-06-14 13:26:04Z kobus $ */


#include "l/l_incl.h" 


/*ARGSUSED*/
int main(int argc, char **argv)
{
    char  line[ 3000 ];
    char  path[ 1000 ];
    char  name[ 1000 ];
    char  mode[ 10 ];
    int   res;
    int   do_it;
    FILE* fp;


    ivi_l_set("debug_level", "1"); 

    while ((res=BUFF_STDIN_GET_LINE("file> ",line)) != EOF) 
    {
        EPETE(res);
        dbs(line); 
        dbi(res); 

        res = BUFF_STDIN_GET_LINE("mode> ",mode);

        EPETE(res);
        dbs(mode); 
        dbi(res); 

        do_it = TRUE;

        if (*mode == 'w') 
        {
            res = BUFF_REAL_PATH(line, path);

            if (res != ERROR) 
            {
                if (! confirm_risky_action("Overwrite? ")) 
                {
                    do_it = FALSE;
                }
            }
        }

        if (do_it) 
        {
            NPE(fp = ivi_fopen(line, mode));
        }

        if (fp != NULL) 
        {
            EPE(BUFF_GET_FD_NAME(fileno(fp), name));

            dbs(name);
        }
    }

    return EXIT_SUCCESS; 
}


