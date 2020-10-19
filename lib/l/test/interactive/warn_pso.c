
/* $Id: ivi_fprintf.c 25587 2020-06-24 02:28:42Z kobus $ */


/*
   Always regenerate pse.c, pso.c, ivi_fprintf_term.c and fprintf.c 
   if this file is changed.

   Lines begining with null comment have stdout->stderr in ivi_fprintf_term.c 
*/

#include "l/l_incl.h" 

int main(void)
{
    int         i32;
    short       i16;
    float       fl;
    long_double ldb;
    const char* s1 = "Hi there everybody !!!";;
    int         res;
    int         i;
    int         j;
    int         count;
    const char* zap     = "zap";
    const char* longish = "longish";
    int         twelve  = 12;
    int         five    = 5;


    warn_pso("%%ABC\n");

    i32 = 666; 
    i16 = 666;

    warn_pso("Some %%'s followed by 666: %% %% %% %d.\n",i32); 

    warn_pso("6 666's:   %d %hd %ld %i %hi %li.\n",
                i32, i16, i32, i32, i16, i32); 

    warn_pso("--%d--%10d--%+10d--%-10d--%010d--% d--\n",
                i32, i32, i32, i32, i32, i32);

    warn_pso("--%*.*d--\n", 8, 4, i32); 
    /**/ warn_pso("--%*.*d--\n", 15, 6, i32); 
    warn_pso("--%*.*d--\n", 15, 1, i32); 

    fl = 100.0; 

    for (i=0; i<10; ++i) 
    {
        fl=i*1.4+.3245;
        /**/     warn_pso("i is %d and fl is %7.*f\n",i,2,fl); 
        warn_pso("i is %ld and fl is %*.2f\n",i,7,fl); 
        /**/     warn_pso("i is %hd and fl is %*.*f\n",(short)i,7,i,fl); 
        warn_pso("i is %lx and fl is %*.*f\n",i,7,i,fl); 
        /**/     warn_pso("i is %-7d and fl is %*.*f\n",i,7,i,fl); 
        warn_pso("i is % 3d and fl is %e\n",i, fl); 
    }

    warn_pso("a %% b %% c %% d %s\n","hello"); 

    warn_pso("--%40s--.\n",s1); 
    warn_pso("--%15s--.\n",s1); 
    /**/ warn_pso("--%-40s--.\n",s1); 
    warn_pso("--%-15s--.\n",s1); 
    warn_pso("--%40.10s--.\n",s1); 
    /**/ warn_pso("--%15.10s--.\n",s1); 
    warn_pso("--%-40.10s--.\n",s1); 
    warn_pso("--%-15.10s--.\n",s1); 

    warn_pso("--%*.*s--.\n",40,15,s1);
    warn_pso("--%*.*s--.\n",40,5,s1);
    /**/ warn_pso("--%*.*s--.\n",10,8,s1);
    warn_pso("--%*.s--.\n",40,s1);
    /**/ warn_pso("--%*.s--.\n",20,s1);
    warn_pso("--%.*s--.\n",5,s1);
    /**/ warn_pso("--%30.*s--.\n",8,s1);

    warn_pso("--%.s--.\n",s1);
    warn_pso("--%.s--.\n",s1);

    /**/ warn_pso("fl is %*.*f\n",-7,-2,fl); 
    warn_pso("fl is %*.*f\n",-7,-1,fl); 

    for (i=0; i<10; ++i) 
    {
        ldb=i*1.4+.3245;
        warn_pso("i is %d and fl is %7.2f\n",i, (double)ldb); 
        warn_pso("i is %d and fl is %7.2Lf\n",i, ldb); 
        warn_pso("i is %d and fl is %7.*Lf\n",i, 2,ldb); 
        /**/     warn_pso("i is %ld and fl is %*.2Lf\n",i,7,ldb); 
        warn_pso("i is %hd and fl is %*.*Lf\n",(short)i,7,i,ldb); 
        /**/     warn_pso("i is %lx and fl is %*.*Lf\n",i,7,i,ldb); 
        warn_pso("i is %-7d and fl is %*.*Lf\n",i,7,i,ldb); 
        warn_pso("i is % 3d and fl is %Le\n",i, ldb); 
    }

    count = 1;
    i=45;
    j=-45;

    warn_pso("\nCompare with  Harbison/Steele page 373, table 15-8\n\n");

    /**/ warn_pso("%-3d: ", count++);
    /**/ res = warn_pso("%12d | %12d", i, j); 
    warn_pso("    ---> %d\n", res);

    /**/ warn_pso("%-3d: ", count++);
    /**/ res = warn_pso("%012d | %012d", i, j); 
    warn_pso("    ---> %d\n", res);

    warn_pso("%-3d: ", count++);
    /**/ res = warn_pso("% 012d | % 012d", i, j); 
    warn_pso("    ---> %d\n", res);

    /**/ warn_pso("%-3d: ", count++);
    res = warn_pso("%+12d | %+12d", i, j); 
    warn_pso("    ---> %d\n", res);

    /**/ warn_pso("%-3d: ", count++);
    res = warn_pso("%+012d | %+012d", i, j); 
    /**/ warn_pso("    ---> %d\n", res);

    warn_pso("%-3d: ", count++);
    res = warn_pso("%-12d | %-12d", i, j); 
    /**/ warn_pso("    ---> %d\n", res);

    warn_pso("%-3d: ", count++);
    /**/ res = warn_pso("%- 12d | %- 12d", i, j); 
    warn_pso("    ---> %d\n", res);

    warn_pso("%-3d: ", count++);
    res = warn_pso("%-+12d | %-+12d", i, j); 
    /**/ warn_pso("    ---> %d\n", res);

    warn_pso("%-3d: ", count++);
    res = warn_pso("%12.4d | %12.4d", i, j); 
    /**/ warn_pso("    ---> %d\n", res);

    warn_pso("%-3d: ", count++);
    /**/ res = warn_pso("%-12.4d | %-12.4d", i, j); 
    warn_pso("    ---> %d\n", res);

    count = 1;

    /**/ warn_pso(
    "\nCompare with  Harbison/Steele page 373, table 15-9\n\n");

    /**/ warn_pso("%-3d: ", count++);
    res = warn_pso("%14u | %14u", i, j); 
    warn_pso("    ---> %d\n", res);

    /**/ warn_pso("%-3d: ", count++);
    res = warn_pso("%014u | %014u", i, j); 
    warn_pso("    ---> %d\n", res);

    warn_pso("%-3d: ", count++);
    res = warn_pso("%#14u | %#14u", i, j); 
    warn_pso("    ---> %d\n", res);

    /**/ warn_pso("%-3d: ", count++);
    res = warn_pso("%#014u | %#014u", i, j); 
    warn_pso("    ---> %d\n", res);

    warn_pso("%-3d: ", count++);
    /**/ res = warn_pso("%-14u | %-14u", i, j); 
    warn_pso("    ---> %d\n", res);

    warn_pso("%-3d: ", count++);
    res = warn_pso("%-#14u | %-#14u", i, j); 
    warn_pso("    ---> %d\n", res);

    warn_pso("%-3d: ", count++);
    /**/ res = warn_pso("%14.4u | %14.4u", i, j); 
    warn_pso("    ---> %d\n", res);

    warn_pso("%-3d: ", count++);
    res = warn_pso("%-14.4u | %-14.4u", i, j); 
    /**/ warn_pso("    ---> %d\n", res);

    count = 1;

    warn_pso(
                "\nCompare with Harbison/Steele page 376, table 15-15\n\n");

    warn_pso("%-3d: ", count++);
    /**/ res = warn_pso("%12s  |  %12s  |  ", zap, longish); 
    warn_pso("  ---> %d\n", res);

    warn_pso("%-3d: ", count++);
    res = warn_pso("%12.5s  |  %12.5s  |  ", zap, longish); 
    warn_pso("  ---> %d\n", res);

    warn_pso("%-3d: ", count++);
    res = warn_pso("%012s  |  %012s  |  ", zap, longish); 
    warn_pso("  ---> %d\n", res);

    warn_pso("%-3d: ", count++);
    res = warn_pso("%-12s  |  %-12s  |  ", zap, longish); 
    warn_pso("  ---> %d\n", res);

    count = 1;

    warn_pso(
                "\nCompare with Harbison/Steele page 376, table 15-15\n\n");

    count = 1;

    warn_pso("%-3d: ", count++);
    res = warn_pso("%*s  |  %*s  |  ", twelve, zap, twelve, longish); 
    /**/ warn_pso("  ---> %d\n", res);

    warn_pso("%-3d: ", count++);
    res = warn_pso("%*.*s  |  %*.*s  |  ", twelve, five, zap, twelve,
                      five, longish); 
    /**/ warn_pso("  ---> %d\n", res);

    warn_pso("%-3d: ", count++);
    res = warn_pso("%0*s  |  %0*s  |  ", twelve, zap, twelve, 
                      longish); 
    /**/ warn_pso("  ---> %d\n", res);

    warn_pso("%-3d: ", count++);
    res = warn_pso("%-*s  |  %-*s  |  ", twelve, zap, twelve,
                      longish); 
    warn_pso("  ---> %d\n", res);


    return EXIT_SUCCESS; 
}

