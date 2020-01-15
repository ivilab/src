#!/usr/bin/env python2.7

# $Id$
# Language: python 2.7
# This scripts is adapted from ties_plot.py writtien by Andrew Predoehl 
#

import argparse
import glob
import matplotlib.figure, matplotlib.backends.backend_agg
import numpy, numpy.ma
import os, os.path


def _draw_basic_plot(ax, title, x, y):
    '''
    Plot the four curves on the plot-axes, and label the axes.
    '''
    handles = ax.plot(x, y, 'r')
    ax.set_title(title)
    ax.set_xlabel('Iterations')
    ax.set_ylabel('Param value')

    return handles


def _plot_(title, x, y): #, legend_str):
    'Generate and return a figure, on which we draw the basic plot.'
    #fig = matplotlib.figure.Figure(figsize=(30, 10))
    fig = matplotlib.figure.Figure()
    ax = fig.add_subplot(111)
    _draw_basic_plot(ax, title, x, y)
    #fig.legend(h, (legend_str), loc='center right')
    return fig


def _gen_a_plot(title, plot_path_stem, data_fp, index):
    '''
    Inputs correspond to one dyad, one flavor of model, one observable cateory,
    and we want to plot the timeseries of both, overlaid together.  The plot
    is saved to files, with path and filename glued together from the inputs,
    and in a few formats (currently PNG, EPS, and CSV).

    String 'plot_path_stem' is part of the pathname of the output file -- it
    still lacks a filetype extension.

    '''

    print data_fp
    print plot_path_stem
    try:
        os.makedirs(plot_path_stem)
    except os.error:
        pass
    ## read in the ll.txt file 
    trace = numpy.loadtxt(data_fp);
    if index < len(trace[0]):
        raise Exception('Index is out of bound')
    x = trace[:, 0]
    y = trace[:, index]
    fig = _plot_(title, x, y)  
    canvas = matplotlib.backends.backend_agg.FigureCanvasAgg(fig)
    filepat = 'param_' + index + '.eps'
    filename = os.path.join(plot_path_stem, filepat)
    canvas.print_figure(filename)
    filepat = 'param_' + index + '.png'
    filename = os.path.join(plot_path_stem, filepat)
    canvas.print_figure(filename)

def generate_individual_plots(args):

    fold_states = os.path.join('00[0-9][0-9]')
    #fold_states = os.path.join('0056')
    model_dir = args.model_dir
    print os.path.join(model_dir, fold_states)
    fold_paths = glob.glob(os.path.join(model_dir, fold_states))
    print fold_paths
    for fold_dir in fold_paths:
        _gen_a_plot('Trace',
            os.path.join(fold_dir, 'trace'), 
            os.path.join(fold_dir, 'trace.txt'),
            args.index)
        #_gen_a_plot('Testing Posteriors changes',
            #os.path.join(fold_dir, 'testing', 'll'), 
            #os.path.join(fold_dir, 'testing', 'll.txt'))

if __name__ == '__main__':

    parser = argparse.ArgumentParser(description='Plot the Lss parameter trace')
    parser.add_argument('--model-dir', 
        help='Model directory(Required)')
    parser.add_argument('--index',
        help='Index of the parameter(Required)')
    args = parser.parse_args()
    generate_individual_plots(args)
    print "finish" 

