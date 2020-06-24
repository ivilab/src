 
==========================================================================
vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
test_program_2 test trim 01 mac_osx_14_i386_i7/dev/clang++_clang/ standard test_runs/trim/01/test/standard 
 
Script test_program_2 test_type is test.
Running initial test to establish valid output for trim with input 01.
Begin running of trim in /Volumes/home/kobus/src/lib/l/test/test_runs/trim/01/test/standard

PAREN /Volumes/home/kobus/src/lib/l/test/mac_osx_14_i386_i7/dev/clang++_clang/trim  LESS STDIN GREATER STDOUT PAREN GREATER_AMP STDERR
Test run_dir_files: RC STDERR STDOUT
Filtering STDERR
1c1
< (no-ivi) dbi: res is 3 on source line 57 of trim.c
---
> (no-ivi) dbi: res is 3 on source line 23 of trim.c
3,5c3,5
< (no-ivi) dbs: line_ptr is -> p <- on source line 60 of trim.c
< (no-ivi) dbi: strlen(line_ptr) is 3 on source line 61 of trim.c
< (no-ivi) dbi: line_trim_len is 1 on source line 63 of trim.c
---
> (no-ivi) dbs: line_ptr is -> p <- on source line 25 of trim.c
> (no-ivi) dbi: strlen(line_ptr) is 3 on source line 26 of trim.c
> (no-ivi) dbi: line_trim_len is 1 on source line 28 of trim.c
8,11c8,11
< (no-ivi) dbs: line_ptr is ->p <- on source line 68 of trim.c
< (no-ivi) dbi: strlen(line_ptr) is 2 on source line 70 of trim.c
< (no-ivi) dbi: line_trim_len is 1 on source line 71 of trim.c
< (no-ivi) dbi: trim_count is 1 on source line 72 of trim.c
---
> (no-ivi) dbs: line_ptr is ->p <- on source line 33 of trim.c
> (no-ivi) dbi: strlen(line_ptr) is 2 on source line 35 of trim.c
> (no-ivi) dbi: line_trim_len is 1 on source line 36 of trim.c
> (no-ivi) dbi: trim_count is 1 on source line 37 of trim.c
14,18c14,19
< (no-ivi) dbs: line_ptr is ->p<- on source line 78 of trim.c
< (no-ivi) dbi: strlen(line_ptr) is 1 on source line 80 of trim.c
< (no-ivi) dbi: line_trim_len is 1 on source line 81 of trim.c
< (no-ivi) dbi: trim_count is 1 on source line 82 of trim.c
< (no-ivi) dbi: res is 4 on source line 57 of trim.c
---
> (no-ivi) dbs: line_ptr is ->p<- on source line 42 of trim.c
> (no-ivi) dbi: strlen(line_ptr) is 1 on source line 44 of trim.c
> (no-ivi) dbi: line_trim_len is 1 on source line 45 of trim.c
> (no-ivi) dbi: trim_count is 1 on source line 46 of trim.c
> -------------------------------Test Ends Here
> (no-ivi) dbi: res is 4 on source line 23 of trim.c
20,22c21,23
< (no-ivi) dbs: line_ptr is ->  p <- on source line 60 of trim.c
< (no-ivi) dbi: strlen(line_ptr) is 4 on source line 61 of trim.c
< (no-ivi) dbi: line_trim_len is 1 on source line 63 of trim.c
---
> (no-ivi) dbs: line_ptr is ->  p <- on source line 25 of trim.c
> (no-ivi) dbi: strlen(line_ptr) is 4 on source line 26 of trim.c
> (no-ivi) dbi: line_trim_len is 1 on source line 28 of trim.c
25,28c26,29
< (no-ivi) dbs: line_ptr is ->p <- on source line 68 of trim.c
< (no-ivi) dbi: strlen(line_ptr) is 2 on source line 70 of trim.c
< (no-ivi) dbi: line_trim_len is 1 on source line 71 of trim.c
< (no-ivi) dbi: trim_count is 2 on source line 72 of trim.c
---
> (no-ivi) dbs: line_ptr is ->p <- on source line 33 of trim.c
> (no-ivi) dbi: strlen(line_ptr) is 2 on source line 35 of trim.c
> (no-ivi) dbi: line_trim_len is 1 on source line 36 of trim.c
> (no-ivi) dbi: trim_count is 2 on source line 37 of trim.c
31,35c32,37
< (no-ivi) dbs: line_ptr is ->p<- on source line 78 of trim.c
< (no-ivi) dbi: strlen(line_ptr) is 1 on source line 80 of trim.c
< (no-ivi) dbi: line_trim_len is 1 on source line 81 of trim.c
< (no-ivi) dbi: trim_count is 1 on source line 82 of trim.c
< (no-ivi) dbi: res is 5 on source line 57 of trim.c
---
> (no-ivi) dbs: line_ptr is ->p<- on source line 42 of trim.c
> (no-ivi) dbi: strlen(line_ptr) is 1 on source line 44 of trim.c
> (no-ivi) dbi: line_trim_len is 1 on source line 45 of trim.c
> (no-ivi) dbi: trim_count is 1 on source line 46 of trim.c
> -------------------------------Test Ends Here
> (no-ivi) dbi: res is 5 on source line 23 of trim.c
37,39c39,41
< (no-ivi) dbs: line_ptr is ->  p  <- on source line 60 of trim.c
< (no-ivi) dbi: strlen(line_ptr) is 5 on source line 61 of trim.c
< (no-ivi) dbi: line_trim_len is 1 on source line 63 of trim.c
---
> (no-ivi) dbs: line_ptr is ->  p  <- on source line 25 of trim.c
> (no-ivi) dbi: strlen(line_ptr) is 5 on source line 26 of trim.c
> (no-ivi) dbi: line_trim_len is 1 on source line 28 of trim.c
42,45c44,47
< (no-ivi) dbs: line_ptr is ->p  <- on source line 68 of trim.c
< (no-ivi) dbi: strlen(line_ptr) is 3 on source line 70 of trim.c
< (no-ivi) dbi: line_trim_len is 1 on source line 71 of trim.c
< (no-ivi) dbi: trim_count is 2 on source line 72 of trim.c
---
> (no-ivi) dbs: line_ptr is ->p  <- on source line 33 of trim.c
> (no-ivi) dbi: strlen(line_ptr) is 3 on source line 35 of trim.c
> (no-ivi) dbi: line_trim_len is 1 on source line 36 of trim.c
> (no-ivi) dbi: trim_count is 2 on source line 37 of trim.c
48,52c50,55
< (no-ivi) dbs: line_ptr is ->p<- on source line 78 of trim.c
< (no-ivi) dbi: strlen(line_ptr) is 1 on source line 80 of trim.c
< (no-ivi) dbi: line_trim_len is 1 on source line 81 of trim.c
< (no-ivi) dbi: trim_count is 2 on source line 82 of trim.c
< (no-ivi) dbi: res is 6 on source line 57 of trim.c
---
> (no-ivi) dbs: line_ptr is ->p<- on source line 42 of trim.c
> (no-ivi) dbi: strlen(line_ptr) is 1 on source line 44 of trim.c
> (no-ivi) dbi: line_trim_len is 1 on source line 45 of trim.c
> (no-ivi) dbi: trim_count is 2 on source line 46 of trim.c
> -------------------------------Test Ends Here
> (no-ivi) dbi: res is 6 on source line 23 of trim.c
54,56c57,59
< (no-ivi) dbs: line_ptr is ->  p   <- on source line 60 of trim.c
< (no-ivi) dbi: strlen(line_ptr) is 6 on source line 61 of trim.c
< (no-ivi) dbi: line_trim_len is 1 on source line 63 of trim.c
---
> (no-ivi) dbs: line_ptr is ->  p   <- on source line 25 of trim.c
> (no-ivi) dbi: strlen(line_ptr) is 6 on source line 26 of trim.c
> (no-ivi) dbi: line_trim_len is 1 on source line 28 of trim.c
59,62c62,65
< (no-ivi) dbs: line_ptr is ->p   <- on source line 68 of trim.c
< (no-ivi) dbi: strlen(line_ptr) is 4 on source line 70 of trim.c
< (no-ivi) dbi: line_trim_len is 1 on source line 71 of trim.c
< (no-ivi) dbi: trim_count is 2 on source line 72 of trim.c
---
> (no-ivi) dbs: line_ptr is ->p   <- on source line 33 of trim.c
> (no-ivi) dbi: strlen(line_ptr) is 4 on source line 35 of trim.c
> (no-ivi) dbi: line_trim_len is 1 on source line 36 of trim.c
> (no-ivi) dbi: trim_count is 2 on source line 37 of trim.c
65,69c68,73
< (no-ivi) dbs: line_ptr is ->p<- on source line 78 of trim.c
< (no-ivi) dbi: strlen(line_ptr) is 1 on source line 80 of trim.c
< (no-ivi) dbi: line_trim_len is 1 on source line 81 of trim.c
< (no-ivi) dbi: trim_count is 3 on source line 82 of trim.c
< (no-ivi) dbi: res is 3 on source line 57 of trim.c
---
> (no-ivi) dbs: line_ptr is ->p<- on source line 42 of trim.c
> (no-ivi) dbi: strlen(line_ptr) is 1 on source line 44 of trim.c
> (no-ivi) dbi: line_trim_len is 1 on source line 45 of trim.c
> (no-ivi) dbi: trim_count is 3 on source line 46 of trim.c
> -------------------------------Test Ends Here
> (no-ivi) dbi: res is 3 on source line 23 of trim.c
71,73c75,77
< (no-ivi) dbs: line_ptr is ->a a<- on source line 60 of trim.c
< (no-ivi) dbi: strlen(line_ptr) is 3 on source line 61 of trim.c
< (no-ivi) dbi: line_trim_len is 3 on source line 63 of trim.c
---
> (no-ivi) dbs: line_ptr is ->a a<- on source line 25 of trim.c
> (no-ivi) dbi: strlen(line_ptr) is 3 on source line 26 of trim.c
> (no-ivi) dbi: line_trim_len is 3 on source line 28 of trim.c
76,79c80,83
< (no-ivi) dbs: line_ptr is ->a a<- on source line 68 of trim.c
< (no-ivi) dbi: strlen(line_ptr) is 3 on source line 70 of trim.c
< (no-ivi) dbi: line_trim_len is 3 on source line 71 of trim.c
< (no-ivi) dbi: trim_count is 0 on source line 72 of trim.c
---
> (no-ivi) dbs: line_ptr is ->a a<- on source line 33 of trim.c
> (no-ivi) dbi: strlen(line_ptr) is 3 on source line 35 of trim.c
> (no-ivi) dbi: line_trim_len is 3 on source line 36 of trim.c
> (no-ivi) dbi: trim_count is 0 on source line 37 of trim.c
82,86c86,91
< (no-ivi) dbs: line_ptr is ->a a<- on source line 78 of trim.c
< (no-ivi) dbi: strlen(line_ptr) is 3 on source line 80 of trim.c
< (no-ivi) dbi: line_trim_len is 3 on source line 81 of trim.c
< (no-ivi) dbi: trim_count is 0 on source line 82 of trim.c
< (no-ivi) dbi: res is 3 on source line 57 of trim.c
---
> (no-ivi) dbs: line_ptr is ->a a<- on source line 42 of trim.c
> (no-ivi) dbi: strlen(line_ptr) is 3 on source line 44 of trim.c
> (no-ivi) dbi: line_trim_len is 3 on source line 45 of trim.c
> (no-ivi) dbi: trim_count is 0 on source line 46 of trim.c
> -------------------------------Test Ends Here
> (no-ivi) dbi: res is 3 on source line 23 of trim.c
88,90c93,95
< (no-ivi) dbs: line_ptr is ->a b<- on source line 60 of trim.c
< (no-ivi) dbi: strlen(line_ptr) is 3 on source line 61 of trim.c
< (no-ivi) dbi: line_trim_len is 3 on source line 63 of trim.c
---
> (no-ivi) dbs: line_ptr is ->a b<- on source line 25 of trim.c
> (no-ivi) dbi: strlen(line_ptr) is 3 on source line 26 of trim.c
> (no-ivi) dbi: line_trim_len is 3 on source line 28 of trim.c
93,96c98,101
< (no-ivi) dbs: line_ptr is ->a b<- on source line 68 of trim.c
< (no-ivi) dbi: strlen(line_ptr) is 3 on source line 70 of trim.c
< (no-ivi) dbi: line_trim_len is 3 on source line 71 of trim.c
< (no-ivi) dbi: trim_count is 0 on source line 72 of trim.c
---
> (no-ivi) dbs: line_ptr is ->a b<- on source line 33 of trim.c
> (no-ivi) dbi: strlen(line_ptr) is 3 on source line 35 of trim.c
> (no-ivi) dbi: line_trim_len is 3 on source line 36 of trim.c
> (no-ivi) dbi: trim_count is 0 on source line 37 of trim.c
99,103c104,109
< (no-ivi) dbs: line_ptr is ->a b<- on source line 78 of trim.c
< (no-ivi) dbi: strlen(line_ptr) is 3 on source line 80 of trim.c
< (no-ivi) dbi: line_trim_len is 3 on source line 81 of trim.c
< (no-ivi) dbi: trim_count is 0 on source line 82 of trim.c
< (no-ivi) dbi: res is 5 on source line 57 of trim.c
---
> (no-ivi) dbs: line_ptr is ->a b<- on source line 42 of trim.c
> (no-ivi) dbi: strlen(line_ptr) is 3 on source line 44 of trim.c
> (no-ivi) dbi: line_trim_len is 3 on source line 45 of trim.c
> (no-ivi) dbi: trim_count is 0 on source line 46 of trim.c
> -------------------------------Test Ends Here
> (no-ivi) dbi: res is 5 on source line 23 of trim.c
105,107c111,113
< (no-ivi) dbs: line_ptr is ->aabba<- on source line 60 of trim.c
< (no-ivi) dbi: strlen(line_ptr) is 5 on source line 61 of trim.c
< (no-ivi) dbi: line_trim_len is 5 on source line 63 of trim.c
---
> (no-ivi) dbs: line_ptr is ->aabba<- on source line 25 of trim.c
> (no-ivi) dbi: strlen(line_ptr) is 5 on source line 26 of trim.c
> (no-ivi) dbi: line_trim_len is 5 on source line 28 of trim.c
110,113c116,119
< (no-ivi) dbs: line_ptr is ->aabba<- on source line 68 of trim.c
< (no-ivi) dbi: strlen(line_ptr) is 5 on source line 70 of trim.c
< (no-ivi) dbi: line_trim_len is 5 on source line 71 of trim.c
< (no-ivi) dbi: trim_count is 0 on source line 72 of trim.c
---
> (no-ivi) dbs: line_ptr is ->aabba<- on source line 33 of trim.c
> (no-ivi) dbi: strlen(line_ptr) is 5 on source line 35 of trim.c
> (no-ivi) dbi: line_trim_len is 5 on source line 36 of trim.c
> (no-ivi) dbi: trim_count is 0 on source line 37 of trim.c
116,120c122,127
< (no-ivi) dbs: line_ptr is ->aabba<- on source line 78 of trim.c
< (no-ivi) dbi: strlen(line_ptr) is 5 on source line 80 of trim.c
< (no-ivi) dbi: line_trim_len is 5 on source line 81 of trim.c
< (no-ivi) dbi: trim_count is 0 on source line 82 of trim.c
< (no-ivi) dbi: res is 9 on source line 57 of trim.c
---
> (no-ivi) dbs: line_ptr is ->aabba<- on source line 42 of trim.c
> (no-ivi) dbi: strlen(line_ptr) is 5 on source line 44 of trim.c
> (no-ivi) dbi: line_trim_len is 5 on source line 45 of trim.c
> (no-ivi) dbi: trim_count is 0 on source line 46 of trim.c
> -------------------------------Test Ends Here
> (no-ivi) dbi: res is 9 on source line 23 of trim.c
122,124c129,131
< (no-ivi) dbs: line_ptr is ->a abb a a<- on source line 60 of trim.c
< (no-ivi) dbi: strlen(line_ptr) is 9 on source line 61 of trim.c
< (no-ivi) dbi: line_trim_len is 9 on source line 63 of trim.c
---
> (no-ivi) dbs: line_ptr is ->a abb a a<- on source line 25 of trim.c
> (no-ivi) dbi: strlen(line_ptr) is 9 on source line 26 of trim.c
> (no-ivi) dbi: line_trim_len is 9 on source line 28 of trim.c
127,130c134,137
< (no-ivi) dbs: line_ptr is ->a abb a a<- on source line 68 of trim.c
< (no-ivi) dbi: strlen(line_ptr) is 9 on source line 70 of trim.c
< (no-ivi) dbi: line_trim_len is 9 on source line 71 of trim.c
< (no-ivi) dbi: trim_count is 0 on source line 72 of trim.c
---
> (no-ivi) dbs: line_ptr is ->a abb a a<- on source line 33 of trim.c
> (no-ivi) dbi: strlen(line_ptr) is 9 on source line 35 of trim.c
> (no-ivi) dbi: line_trim_len is 9 on source line 36 of trim.c
> (no-ivi) dbi: trim_count is 0 on source line 37 of trim.c
133,137c140,145
< (no-ivi) dbs: line_ptr is ->a abb a a<- on source line 78 of trim.c
< (no-ivi) dbi: strlen(line_ptr) is 9 on source line 80 of trim.c
< (no-ivi) dbi: line_trim_len is 9 on source line 81 of trim.c
< (no-ivi) dbi: trim_count is 0 on source line 82 of trim.c
< (no-ivi) dbi: res is 11 on source line 57 of trim.c
---
> (no-ivi) dbs: line_ptr is ->a abb a a<- on source line 42 of trim.c
> (no-ivi) dbi: strlen(line_ptr) is 9 on source line 44 of trim.c
> (no-ivi) dbi: line_trim_len is 9 on source line 45 of trim.c
> (no-ivi) dbi: trim_count is 0 on source line 46 of trim.c
> -------------------------------Test Ends Here
> (no-ivi) dbi: res is 11 on source line 23 of trim.c
139,141c147,149
< (no-ivi) dbs: line_ptr is ->abcabcabc  <- on source line 60 of trim.c
< (no-ivi) dbi: strlen(line_ptr) is 11 on source line 61 of trim.c
< (no-ivi) dbi: line_trim_len is 9 on source line 63 of trim.c
---
> (no-ivi) dbs: line_ptr is ->abcabcabc  <- on source line 25 of trim.c
> (no-ivi) dbi: strlen(line_ptr) is 11 on source line 26 of trim.c
> (no-ivi) dbi: line_trim_len is 9 on source line 28 of trim.c
144,147c152,155
< (no-ivi) dbs: line_ptr is ->abcabcabc  <- on source line 68 of trim.c
< (no-ivi) dbi: strlen(line_ptr) is 11 on source line 70 of trim.c
< (no-ivi) dbi: line_trim_len is 9 on source line 71 of trim.c
< (no-ivi) dbi: trim_count is 0 on source line 72 of trim.c
---
> (no-ivi) dbs: line_ptr is ->abcabcabc  <- on source line 33 of trim.c
> (no-ivi) dbi: strlen(line_ptr) is 11 on source line 35 of trim.c
> (no-ivi) dbi: line_trim_len is 9 on source line 36 of trim.c
> (no-ivi) dbi: trim_count is 0 on source line 37 of trim.c
150,154c158,163
< (no-ivi) dbs: line_ptr is ->abcabcabc<- on source line 78 of trim.c
< (no-ivi) dbi: strlen(line_ptr) is 9 on source line 80 of trim.c
< (no-ivi) dbi: line_trim_len is 9 on source line 81 of trim.c
< (no-ivi) dbi: trim_count is 2 on source line 82 of trim.c
< (no-ivi) dbi: res is 14 on source line 57 of trim.c
---
> (no-ivi) dbs: line_ptr is ->abcabcabc<- on source line 42 of trim.c
> (no-ivi) dbi: strlen(line_ptr) is 9 on source line 44 of trim.c
> (no-ivi) dbi: line_trim_len is 9 on source line 45 of trim.c
> (no-ivi) dbi: trim_count is 2 on source line 46 of trim.c
> -------------------------------Test Ends Here
> (no-ivi) dbi: res is 14 on source line 23 of trim.c
156,158c165,167
< (no-ivi) dbs: line_ptr is ->abcabcabbbcabc<- on source line 60 of trim.c
< (no-ivi) dbi: strlen(line_ptr) is 14 on source line 61 of trim.c
< (no-ivi) dbi: line_trim_len is 14 on source line 63 of trim.c
---
> (no-ivi) dbs: line_ptr is ->abcabcabbbcabc<- on source line 25 of trim.c
> (no-ivi) dbi: strlen(line_ptr) is 14 on source line 26 of trim.c
> (no-ivi) dbi: line_trim_len is 14 on source line 28 of trim.c
161,164c170,173
< (no-ivi) dbs: line_ptr is ->abcabcabbbcabc<- on source line 68 of trim.c
< (no-ivi) dbi: strlen(line_ptr) is 14 on source line 70 of trim.c
< (no-ivi) dbi: line_trim_len is 14 on source line 71 of trim.c
< (no-ivi) dbi: trim_count is 0 on source line 72 of trim.c
---
> (no-ivi) dbs: line_ptr is ->abcabcabbbcabc<- on source line 33 of trim.c
> (no-ivi) dbi: strlen(line_ptr) is 14 on source line 35 of trim.c
> (no-ivi) dbi: line_trim_len is 14 on source line 36 of trim.c
> (no-ivi) dbi: trim_count is 0 on source line 37 of trim.c
167,171c176,181
< (no-ivi) dbs: line_ptr is ->abcabcabbbcabc<- on source line 78 of trim.c
< (no-ivi) dbi: strlen(line_ptr) is 14 on source line 80 of trim.c
< (no-ivi) dbi: line_trim_len is 14 on source line 81 of trim.c
< (no-ivi) dbi: trim_count is 0 on source line 82 of trim.c
< (no-ivi) dbi: res is 6 on source line 57 of trim.c
---
> (no-ivi) dbs: line_ptr is ->abcabcabbbcabc<- on source line 42 of trim.c
> (no-ivi) dbi: strlen(line_ptr) is 14 on source line 44 of trim.c
> (no-ivi) dbi: line_trim_len is 14 on source line 45 of trim.c
> (no-ivi) dbi: trim_count is 0 on source line 46 of trim.c
> -------------------------------Test Ends Here
> (no-ivi) dbi: res is 6 on source line 23 of trim.c
173,175c183,185
< (no-ivi) dbs: line_ptr is ->!@#abc<- on source line 60 of trim.c
< (no-ivi) dbi: strlen(line_ptr) is 6 on source line 61 of trim.c
< (no-ivi) dbi: line_trim_len is 6 on source line 63 of trim.c
---
> (no-ivi) dbs: line_ptr is ->!@#abc<- on source line 25 of trim.c
> (no-ivi) dbi: strlen(line_ptr) is 6 on source line 26 of trim.c
> (no-ivi) dbi: line_trim_len is 6 on source line 28 of trim.c
178,181c188,191
< (no-ivi) dbs: line_ptr is ->!@#abc<- on source line 68 of trim.c
< (no-ivi) dbi: strlen(line_ptr) is 6 on source line 70 of trim.c
< (no-ivi) dbi: line_trim_len is 6 on source line 71 of trim.c
< (no-ivi) dbi: trim_count is 0 on source line 72 of trim.c
---
> (no-ivi) dbs: line_ptr is ->!@#abc<- on source line 33 of trim.c
> (no-ivi) dbi: strlen(line_ptr) is 6 on source line 35 of trim.c
> (no-ivi) dbi: line_trim_len is 6 on source line 36 of trim.c
> (no-ivi) dbi: trim_count is 0 on source line 37 of trim.c
184,188c194,199
< (no-ivi) dbs: line_ptr is ->!@#abc<- on source line 78 of trim.c
< (no-ivi) dbi: strlen(line_ptr) is 6 on source line 80 of trim.c
< (no-ivi) dbi: line_trim_len is 6 on source line 81 of trim.c
< (no-ivi) dbi: trim_count is 0 on source line 82 of trim.c
< (no-ivi) dbi: res is 5 on source line 57 of trim.c
---
> (no-ivi) dbs: line_ptr is ->!@#abc<- on source line 42 of trim.c
> (no-ivi) dbi: strlen(line_ptr) is 6 on source line 44 of trim.c
> (no-ivi) dbi: line_trim_len is 6 on source line 45 of trim.c
> (no-ivi) dbi: trim_count is 0 on source line 46 of trim.c
> -------------------------------Test Ends Here
> (no-ivi) dbi: res is 5 on source line 23 of trim.c
190,192c201,203
< (no-ivi) dbs: line_ptr is ->!@abc<- on source line 60 of trim.c
< (no-ivi) dbi: strlen(line_ptr) is 5 on source line 61 of trim.c
< (no-ivi) dbi: line_trim_len is 5 on source line 63 of trim.c
---
> (no-ivi) dbs: line_ptr is ->!@abc<- on source line 25 of trim.c
> (no-ivi) dbi: strlen(line_ptr) is 5 on source line 26 of trim.c
> (no-ivi) dbi: line_trim_len is 5 on source line 28 of trim.c
195,198c206,209
< (no-ivi) dbs: line_ptr is ->!@abc<- on source line 68 of trim.c
< (no-ivi) dbi: strlen(line_ptr) is 5 on source line 70 of trim.c
< (no-ivi) dbi: line_trim_len is 5 on source line 71 of trim.c
< (no-ivi) dbi: trim_count is 0 on source line 72 of trim.c
---
> (no-ivi) dbs: line_ptr is ->!@abc<- on source line 33 of trim.c
> (no-ivi) dbi: strlen(line_ptr) is 5 on source line 35 of trim.c
> (no-ivi) dbi: line_trim_len is 5 on source line 36 of trim.c
> (no-ivi) dbi: trim_count is 0 on source line 37 of trim.c
201,205c212,217
< (no-ivi) dbs: line_ptr is ->!@abc<- on source line 78 of trim.c
< (no-ivi) dbi: strlen(line_ptr) is 5 on source line 80 of trim.c
< (no-ivi) dbi: line_trim_len is 5 on source line 81 of trim.c
< (no-ivi) dbi: trim_count is 0 on source line 82 of trim.c
< (no-ivi) dbi: res is 7 on source line 57 of trim.c
---
> (no-ivi) dbs: line_ptr is ->!@abc<- on source line 42 of trim.c
> (no-ivi) dbi: strlen(line_ptr) is 5 on source line 44 of trim.c
> (no-ivi) dbi: line_trim_len is 5 on source line 45 of trim.c
> (no-ivi) dbi: trim_count is 0 on source line 46 of trim.c
> -------------------------------Test Ends Here
> (no-ivi) dbi: res is 7 on source line 23 of trim.c
207,209c219,221
< (no-ivi) dbs: line_ptr is ->!  @abc<- on source line 60 of trim.c
< (no-ivi) dbi: strlen(line_ptr) is 7 on source line 61 of trim.c
< (no-ivi) dbi: line_trim_len is 7 on source line 63 of trim.c
---
> (no-ivi) dbs: line_ptr is ->!  @abc<- on source line 25 of trim.c
> (no-ivi) dbi: strlen(line_ptr) is 7 on source line 26 of trim.c
> (no-ivi) dbi: line_trim_len is 7 on source line 28 of trim.c
212,215c224,227
< (no-ivi) dbs: line_ptr is ->!  @abc<- on source line 68 of trim.c
< (no-ivi) dbi: strlen(line_ptr) is 7 on source line 70 of trim.c
< (no-ivi) dbi: line_trim_len is 7 on source line 71 of trim.c
< (no-ivi) dbi: trim_count is 0 on source line 72 of trim.c
---
> (no-ivi) dbs: line_ptr is ->!  @abc<- on source line 33 of trim.c
> (no-ivi) dbi: strlen(line_ptr) is 7 on source line 35 of trim.c
> (no-ivi) dbi: line_trim_len is 7 on source line 36 of trim.c
> (no-ivi) dbi: trim_count is 0 on source line 37 of trim.c
218,222c230,235
< (no-ivi) dbs: line_ptr is ->!  @abc<- on source line 78 of trim.c
< (no-ivi) dbi: strlen(line_ptr) is 7 on source line 80 of trim.c
< (no-ivi) dbi: line_trim_len is 7 on source line 81 of trim.c
< (no-ivi) dbi: trim_count is 0 on source line 82 of trim.c
< (no-ivi) dbi: res is 9 on source line 57 of trim.c
---
> (no-ivi) dbs: line_ptr is ->!  @abc<- on source line 42 of trim.c
> (no-ivi) dbi: strlen(line_ptr) is 7 on source line 44 of trim.c
> (no-ivi) dbi: line_trim_len is 7 on source line 45 of trim.c
> (no-ivi) dbi: trim_count is 0 on source line 46 of trim.c
> -------------------------------Test Ends Here
> (no-ivi) dbi: res is 9 on source line 23 of trim.c
224,226c237,239
< (no-ivi) dbs: line_ptr is ->! @abc! @<- on source line 60 of trim.c
< (no-ivi) dbi: strlen(line_ptr) is 9 on source line 61 of trim.c
< (no-ivi) dbi: line_trim_len is 9 on source line 63 of trim.c
---
> (no-ivi) dbs: line_ptr is ->! @abc! @<- on source line 25 of trim.c
> (no-ivi) dbi: strlen(line_ptr) is 9 on source line 26 of trim.c
> (no-ivi) dbi: line_trim_len is 9 on source line 28 of trim.c
229,232c242,245
< (no-ivi) dbs: line_ptr is ->! @abc! @<- on source line 68 of trim.c
< (no-ivi) dbi: strlen(line_ptr) is 9 on source line 70 of trim.c
< (no-ivi) dbi: line_trim_len is 9 on source line 71 of trim.c
< (no-ivi) dbi: trim_count is 0 on source line 72 of trim.c
---
> (no-ivi) dbs: line_ptr is ->! @abc! @<- on source line 33 of trim.c
> (no-ivi) dbi: strlen(line_ptr) is 9 on source line 35 of trim.c
> (no-ivi) dbi: line_trim_len is 9 on source line 36 of trim.c
> (no-ivi) dbi: trim_count is 0 on source line 37 of trim.c
235,239c248,253
< (no-ivi) dbs: line_ptr is ->! @abc! @<- on source line 78 of trim.c
< (no-ivi) dbi: strlen(line_ptr) is 9 on source line 80 of trim.c
< (no-ivi) dbi: line_trim_len is 9 on source line 81 of trim.c
< (no-ivi) dbi: trim_count is 0 on source line 82 of trim.c
< (no-ivi) dbi: res is 12 on source line 57 of trim.c
---
> (no-ivi) dbs: line_ptr is ->! @abc! @<- on source line 42 of trim.c
> (no-ivi) dbi: strlen(line_ptr) is 9 on source line 44 of trim.c
> (no-ivi) dbi: line_trim_len is 9 on source line 45 of trim.c
> (no-ivi) dbi: trim_count is 0 on source line 46 of trim.c
> -------------------------------Test Ends Here
> (no-ivi) dbi: res is 12 on source line 23 of trim.c
241,243c255,257
< (no-ivi) dbs: line_ptr is -> !@ !@ !@abc<- on source line 60 of trim.c
< (no-ivi) dbi: strlen(line_ptr) is 12 on source line 61 of trim.c
< (no-ivi) dbi: line_trim_len is 11 on source line 63 of trim.c
---
> (no-ivi) dbs: line_ptr is -> !@ !@ !@abc<- on source line 25 of trim.c
> (no-ivi) dbi: strlen(line_ptr) is 12 on source line 26 of trim.c
> (no-ivi) dbi: line_trim_len is 11 on source line 28 of trim.c
246,249c260,263
< (no-ivi) dbs: line_ptr is ->!@ !@ !@abc<- on source line 68 of trim.c
< (no-ivi) dbi: strlen(line_ptr) is 11 on source line 70 of trim.c
< (no-ivi) dbi: line_trim_len is 11 on source line 71 of trim.c
< (no-ivi) dbi: trim_count is 1 on source line 72 of trim.c
---
> (no-ivi) dbs: line_ptr is ->!@ !@ !@abc<- on source line 33 of trim.c
> (no-ivi) dbi: strlen(line_ptr) is 11 on source line 35 of trim.c
> (no-ivi) dbi: line_trim_len is 11 on source line 36 of trim.c
> (no-ivi) dbi: trim_count is 1 on source line 37 of trim.c
252,256c266,271
< (no-ivi) dbs: line_ptr is ->!@ !@ !@abc<- on source line 78 of trim.c
< (no-ivi) dbi: strlen(line_ptr) is 11 on source line 80 of trim.c
< (no-ivi) dbi: line_trim_len is 11 on source line 81 of trim.c
< (no-ivi) dbi: trim_count is 0 on source line 82 of trim.c
< (no-ivi) dbi: res is 13 on source line 57 of trim.c
---
> (no-ivi) dbs: line_ptr is ->!@ !@ !@abc<- on source line 42 of trim.c
> (no-ivi) dbi: strlen(line_ptr) is 11 on source line 44 of trim.c
> (no-ivi) dbi: line_trim_len is 11 on source line 45 of trim.c
> (no-ivi) dbi: trim_count is 0 on source line 46 of trim.c
> -------------------------------Test Ends Here
> (no-ivi) dbi: res is 13 on source line 23 of trim.c
258,260c273,275
< (no-ivi) dbs: line_ptr is -> !@ !@abc !@ <- on source line 60 of trim.c
< (no-ivi) dbi: strlen(line_ptr) is 13 on source line 61 of trim.c
< (no-ivi) dbi: line_trim_len is 11 on source line 63 of trim.c
---
> (no-ivi) dbs: line_ptr is -> !@ !@abc !@ <- on source line 25 of trim.c
> (no-ivi) dbi: strlen(line_ptr) is 13 on source line 26 of trim.c
> (no-ivi) dbi: line_trim_len is 11 on source line 28 of trim.c
263,266c278,281
< (no-ivi) dbs: line_ptr is ->!@ !@abc !@ <- on source line 68 of trim.c
< (no-ivi) dbi: strlen(line_ptr) is 12 on source line 70 of trim.c
< (no-ivi) dbi: line_trim_len is 11 on source line 71 of trim.c
< (no-ivi) dbi: trim_count is 1 on source line 72 of trim.c
---
> (no-ivi) dbs: line_ptr is ->!@ !@abc !@ <- on source line 33 of trim.c
> (no-ivi) dbi: strlen(line_ptr) is 12 on source line 35 of trim.c
> (no-ivi) dbi: line_trim_len is 11 on source line 36 of trim.c
> (no-ivi) dbi: trim_count is 1 on source line 37 of trim.c
269,273c284,289
< (no-ivi) dbs: line_ptr is ->!@ !@abc !@<- on source line 78 of trim.c
< (no-ivi) dbi: strlen(line_ptr) is 11 on source line 80 of trim.c
< (no-ivi) dbi: line_trim_len is 11 on source line 81 of trim.c
< (no-ivi) dbi: trim_count is 1 on source line 82 of trim.c
< (no-ivi) dbi: res is 7 on source line 57 of trim.c
---
> (no-ivi) dbs: line_ptr is ->!@ !@abc !@<- on source line 42 of trim.c
> (no-ivi) dbi: strlen(line_ptr) is 11 on source line 44 of trim.c
> (no-ivi) dbi: line_trim_len is 11 on source line 45 of trim.c
> (no-ivi) dbi: trim_count is 1 on source line 46 of trim.c
> -------------------------------Test Ends Here
> (no-ivi) dbi: res is 7 on source line 23 of trim.c
275,277c291,293
< (no-ivi) dbs: line_ptr is -> !a!a!a<- on source line 60 of trim.c
< (no-ivi) dbi: strlen(line_ptr) is 7 on source line 61 of trim.c
< (no-ivi) dbi: line_trim_len is 6 on source line 63 of trim.c
---
> (no-ivi) dbs: line_ptr is -> !a!a!a<- on source line 25 of trim.c
> (no-ivi) dbi: strlen(line_ptr) is 7 on source line 26 of trim.c
> (no-ivi) dbi: line_trim_len is 6 on source line 28 of trim.c
280,283c296,299
< (no-ivi) dbs: line_ptr is ->!a!a!a<- on source line 68 of trim.c
< (no-ivi) dbi: strlen(line_ptr) is 6 on source line 70 of trim.c
< (no-ivi) dbi: line_trim_len is 6 on source line 71 of trim.c
< (no-ivi) dbi: trim_count is 1 on source line 72 of trim.c
---
> (no-ivi) dbs: line_ptr is ->!a!a!a<- on source line 33 of trim.c
> (no-ivi) dbi: strlen(line_ptr) is 6 on source line 35 of trim.c
> (no-ivi) dbi: line_trim_len is 6 on source line 36 of trim.c
> (no-ivi) dbi: trim_count is 1 on source line 37 of trim.c
286,289c302,306
< (no-ivi) dbs: line_ptr is ->!a!a!a<- on source line 78 of trim.c
< (no-ivi) dbi: strlen(line_ptr) is 6 on source line 80 of trim.c
< (no-ivi) dbi: line_trim_len is 6 on source line 81 of trim.c
< (no-ivi) dbi: trim_count is 0 on source line 82 of trim.c
---
> (no-ivi) dbs: line_ptr is ->!a!a!a<- on source line 42 of trim.c
> (no-ivi) dbi: strlen(line_ptr) is 6 on source line 44 of trim.c
> (no-ivi) dbi: line_trim_len is 6 on source line 45 of trim.c
> (no-ivi) dbi: trim_count is 0 on source line 46 of trim.c
> -------------------------------Test Ends Here
Filtering STDOUT
Initial run for trim with input 01 finished
 
Time span: 16:37:03 to 16:37:04
 
Leaving script test_program_2
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ 
==========================================================================
 
