# SIS Model 
### (Work in progress)

The SIS model (Susceptible-Infected-Susceptible model) is an epidemiological and mathematical model used to describe the spread of 
certain diseases. There are many variations to this model (SIR, SIRS, including demography, including vaccination, etc) but I'm going
to focus on this one and the SIRS because they are the most relevant to the COVID pandemic.

Now, **sis_rw.cpp** contains the code for the Monte Carlo simulation. Some parameters can be changed through the input file **datin.txt** and 
other parameters have to be changed directly in the code. However, the code can be modified to have them all read from the input file. 
Another fun file is **gnuplot.h**, which is a header to generate plots directly from the simulation without the need to open
gnuplot and do it yourself afterward. However, it is not automatized. Finally, **avgresults.sh** is a shell script for Linux that
helps a lot when you have many output files and need to average them out. Why? Because this is a Monte Carlo simulation and the more
simulations, the better and the more trustworthy the results will be.

Needless to say, this code can and will be improved as I keep learning new things every day :) 
