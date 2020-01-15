#!/usr/bin/env python2.7

# This script is designed to validate any user's CSV file for the CompTIES app,
# rejecting if necessary, and cleaning up any quirks if possible.  The main
# two tasks it does now are these:
#
# * Standardize the end-of-line marks always to be Windows-style \r\n
#
# * Lexically sort rows 2 and up so that for dyad k, the individual-0 records
#   always precede the individual-1 records, and that the time records for
#   dyad k individual 0 are together in a single ascending block.  Likewise
#   for dyad k individual 1.  In other words we stably sort by time, then by
#   the individual distinguisher field, then by dyad number.  If that order
#   seems weird, please review radix sorting for a fuller exposition why.
#
# This script takes one command line argument, the name of the distinguisher
# field.  Input is drawn from standard input, and output is sent to standard
# output.  Warnings and errors are sent to standard error.
#
# $Id: csv_rectifier.py 20279 2016-01-25 04:32:40Z predoehl $

import csv
import sys

def why_is_it_bad(n, col_count):
    for rnum, rlen in enumerate(col_count):
        if rlen != n:
            return Exception('Row ' + str(1+rnum) + ' has ' + str(col_count[rnum])
                    + ' columns, expected ' + str(n) + ' columns')
    return Exception('internal error -- unspecified error in input')

def lacks_x_row1(x):
    return Exception('Input file lacks required ' + x + ' in row 1')

if __name__ == '__main__':
    if len(sys.argv) < 2:
        raise Exception('Required argument:   name_of_distinguisher')

#   Read standard input, grab first line and then a list of remaining lines.
    grid_reader = csv.reader(sys.stdin.read().splitlines())
    header = grid_reader.next()
    data_table = [row for row in grid_reader]

#   Ignore any empty rows at the end of the file (very often there is one).
    while 0 == len(data_table[-1]):
        del data_table[-1]

#   Verify that the grid is rectangular and has the required column labels
    col_count = [len(row) for row in data_table]
    n = len(header)
    if max(col_count) != n or min(col_count) != n:
        raise why_is_it_bad(len(header), col_count)
    if 'Dyad' not in header:
        raise lacks_x_row1('"Dyad" column label')
    if 'time' not in header:
        raise lacks_x_row1('"time" column label')
    dist_name = sys.argv[1]
    if dist_name not in header:
        raise lacks_x_row1('distinguisher column label "' + dist_name + '"')

#   Locate the column number of the important labels
    col_Dyad = header.index('Dyad')
    col_time = header.index('time')
    col_dist = header.index(dist_name)

#   Add helper time field temporarily, as a NUMERIC key, not lexicographic.
#   If the time field is marked as "NA" ignore; otherwise must be a number.
    kill_list = []
    for i, row in enumerate(data_table):
        try:
            row.append(float(row[col_time]))
        except ValueError:
            if 'NA' == row[col_time]:
                kill_list.append(i)
                sys.stderr.write('Warning: ignoring row ' + str(1+i)
                                    + ', which lacks a time index\n')
            else:
                raise Exception('Row ' + str(1+i) + ' has a bad time index')

#   Trim off bad rows (in DESCENDING order, for obvious reasons).
    while len(kill_list) > 0:
        del data_table[kill_list[-1]]
        kill_list = kill_list[:-1]

#   Stably sort the grid (not counting header line) by our three key fields.
    data_table.sort(key = lambda r:r[n]) # tertiary sort: time
    data_table.sort(key = lambda r:r[col_dist]) # secondary sort: distinguisher
    data_table.sort(key = lambda r:r[col_Dyad]) # primary sort: Dyad

#   Write the sorted output (trimming off the helper time field)
    grid_writer = csv.writer(sys.stdout, quoting=csv.QUOTE_MINIMAL)
    grid_writer.writerow(header)
    for row in data_table:
        grid_writer.writerow(row[:-1])

