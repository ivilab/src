
/* $Id: ivi_open.c 25499 2020-06-14 13:26:04Z kobus $ */


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

    BUFF_GET_FD_NAME(fileno(stdin), name);
    p_stderr("Name of stdin is %s (ivi_isatty==>%d).\n", name,
             ivi_isatty(fileno(stdin))); 

    BUFF_GET_FD_NAME(fileno(stdout), name);
    p_stderr("Name of stdout is %s (ivi_isatty==>%d).\n", name,
             ivi_isatty(fileno(stdout))); 

    BUFF_GET_FD_NAME(fileno(stderr), name);
    p_stderr("Name of stderr is %s (ivi_isatty==>%d).\n", name,
             ivi_isatty(fileno(stderr))); 

    p_stderr("\n");

    term_set_raw_mode_with_no_echo();

    while ((res = BUFF_STDIN_GET_LINE("file> ",line)) != EOF) 
    {
        EPETE(res);

        res = BUFF_STDIN_GET_LINE("mode> ",mode);

        EPETE(res);

        do_it = TRUE;

        res = BUFF_REAL_PATH(line, path);
        if (res != ERROR)
        {
            p_stderr("PATH: %s\n", path); 
        }

        if (*mode == 'w') 
        {
            if ((res != ERROR) && (! HEAD_CMP_EQ("/dev", path))) 
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
        else 
        {
            fp = NULL;
        }

        if (fp != NULL) 
        {
            EPE(BUFF_GET_FD_NAME(fileno(fp), name));
            p_stderr("REAL Name is: %s (ivi_isatty==>%d)\n", name,
                     ivi_isatty(fileno(fp))); 

            EPE(BUFF_GET_USER_FD_NAME(fileno(fp), name));
            p_stderr("USER Name is: %s (ivi_isatty==>%d)\n", name, 
                     ivi_isatty(fileno(fp))); 
        }

        p_stderr("\n");
    }

    term_reset();  /* Not really needed because ivi_cleanup below. */

    ivi_cleanup();

    return EXIT_SUCCESS; 
}

