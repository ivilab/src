#!/usr/bin/env bash

# Kobus says. I do not know who wrote this or when. 

# A helper script to find TIES-related things are
# Usage: ./grep.sh <string>
# Make sure to enclose the string with quotes if the string has spaces in it.

grep -r "$1" *.cpp *.h ../../lib/dynamical_cpp/*.cpp *.h
