
/* $Id: sig_block.c 25587 2020-06-24 02:28:42Z kobus $ */



#include "l/l_incl.h" 


static TRAP_FN_RETURN_TYPE trap_fn_0(int); 
static TRAP_FN_RETURN_TYPE trap_fn_1(int); 
static TRAP_FN_RETURN_TYPE trap_fn_2(int); 
static TRAP_FN_RETURN_TYPE trap_fn_3(int); 
static TRAP_FN_RETURN_TYPE trap_fn_4(int); 
static TRAP_FN_RETURN_TYPE trap_fn_5(int); 
static TRAP_FN_RETURN_TYPE trap_fn_6(int); 
static TRAP_FN_RETURN_TYPE trap_fn_7(int); 
static TRAP_FN_RETURN_TYPE trap_fn_8(int); 

int main(void)
{
    char line [ 100 ];
    ivi_int32 res; 
    ivi_int32 level = 0; 
    TRAP_FN_RETURN_TYPE (*trap_fns[ 9 ])(); 


    trap_fns[ 0 ] = (void(*)())trap_fn_0; 
    trap_fns[ 1 ] = (void(*)())trap_fn_1; 
    trap_fns[ 2 ] = (void(*)())trap_fn_2; 
    trap_fns[ 3 ] = (void(*)())trap_fn_3; 
    trap_fns[ 4 ] = (void(*)())trap_fn_4; 
    trap_fns[ 5 ] = (void(*)())trap_fn_5; 
    trap_fns[ 6 ] = (void(*)())trap_fn_6; 
    trap_fns[ 7 ] = (void(*)())trap_fn_7; 
    trap_fns[ 8 ] = (void(*)())trap_fn_8; 

    set_atn_trap(trap_fn_0, RESTART_AFTER_SIGNAL); 

    while ((res = term_get_line("trap> ", line, sizeof(line))) != EOF) 
    {
        dbi(res);
        dbs(line); 

        if (STRCMP_EQ(line, "s"))
        {
            level++;
            EPE(set_atn_trap((void(*)(int))trap_fns[ level ], RESTART_AFTER_SIGNAL)); 

            dbi(level); 
        }
        else if (STRCMP_EQ(line, "u"))
        {
            if (level > 0 ) level--;
            EPE(unset_atn_trap()); 
            dbi(level); 
        }
        else if (STRCMP_EQ(line, "d"))
        {
            EPE(dont_restart_on_atn());
        }
        else if (STRCMP_EQ(line, "r"))
        {
            EPE(restart_on_atn());
        }
        else if (STRCMP_EQ(line, "a"))
        {
            EPE(allow_user_signals());
        }
        else if (STRCMP_EQ(line, "b"))
        {
            EPE(block_user_signals());
        }
        else if (STRCMP_EQ(line, "t"))
        {
            EPE(reset_signals());
        }
        else if (STRCMP_EQ(line, "e"))
        {
            EPE(reset_restart_on_atn());
        }
        else 
        {
            dbm("No level change.\n"); 
        }
    }

    return EXIT_SUCCESS; 
}

static TRAP_FN_RETURN_TYPE trap_fn_0(int __attribute__((unused)) dummy_sig)
{

    p_stderr("Zero.\n"); 
}


static TRAP_FN_RETURN_TYPE trap_fn_1(int  __attribute__((unused)) dummy_sig)
{

    p_stderr("One.\n"); 
}

static TRAP_FN_RETURN_TYPE trap_fn_2(int  __attribute__((unused)) dummy_sig)
{

    p_stderr("Two.\n"); 
}

static TRAP_FN_RETURN_TYPE trap_fn_3(int  __attribute__((unused)) dummy_sig)
{

    p_stderr("Three.\n"); 
}

static TRAP_FN_RETURN_TYPE trap_fn_4(int  __attribute__((unused)) dummy_sig)
{

    p_stderr("Four.\n"); 
}

static TRAP_FN_RETURN_TYPE trap_fn_5(int  __attribute__((unused)) dummy_sig)
{

    p_stderr("Five.\n"); 
}

static TRAP_FN_RETURN_TYPE trap_fn_6(int  __attribute__((unused)) dummy_sig)
{

    p_stderr("Six.\n"); 
}

static TRAP_FN_RETURN_TYPE trap_fn_7(int  __attribute__((unused)) dummy_sig)
{

    p_stderr("Seven.\n"); 
}

static TRAP_FN_RETURN_TYPE trap_fn_8(int  __attribute__((unused)) dummy_sig)
{

    p_stderr("Eight.\n"); 
}




