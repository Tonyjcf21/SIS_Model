#!/usr/bin/awk -f

#==================================================================
#-> NR variable will simply print the line numbers of all the lines of our target text 
#file.(Rows)
#-> FNR is similar to NR, but relative to the current file. It is useful when AWK is 
#operating on multiple files. Value of FNR resets with new file.
#-> NF variable is used to print the number of fields in all the lines (Columns) of any 
#provided file. https://linuxhint.com/awk-nf-ubuntu-20-04/
#==================================================================

# Count number of files: increment on the first line of each new file
FNR == 1 { ++nfiles }

BEGIN { CONVFMT = "%.2f" }

{
    tiempo[FNR, 1] = $1
    
    # (Pseudo) 2D array summing up fields across files
    for (i = 2; i <= NF; ++i) {
        values[FNR, i] += $i #Here we sum the i-th column value for every row in all files.
    }
    real_columns = NF # I need to define this in order to be able to define z later
}

END {
    # Loop over lines of array with sums
    for (i = 1; i <= FNR; ++i) {
        # Loop over fields of current line in array of sums
        for (j = 2; j <= real_columns; ++j) {

            # Build record with averages and avg^2.
            
            z = real_columns+(j-1) #we use real_columns because if we make
            # z = NF+(j-1), then $z would create a new column and that would 
            # automatically increment the value of NF and make the loop infinite.
            
            $j = values[i, j]/nfiles
            $z = (values[i, j]/nfiles)^2
            $1 = tiempo[i, 1]
        
            #print NF, z ---> This made me figure out the problem!! NF was changing (YUNI IS THE BEST)
        }
        #$5 = (values[i, 2]/nfiles)^2
        #$6 = (values[i, 3]/nfiles)^2 
        #$7 = (values[i, 4]/nfiles)^2
        # .                             This works but its not general. I dont like it. Not efficient and automatic.
        # .
        # . 
        #$n = (values[i, n-3]/nfiles)^2
        print > "sis_results_08.dat"
    }
}
#./avgresults.sh timeseries_08_*

