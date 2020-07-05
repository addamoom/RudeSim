puts "Simulation started!"

# creates a list to store the signals
set signalsList [list]

# returns number of facilities in the dumpfile
set nfacs [ gtkwave::getNumFacs ]


for {set i 0} {$i < $nfacs } {incr i} {
    # gets the next signal and appends it to the list     
    set facname [ gtkwave::getFacName $i ]
    lappend signalsList "$facname"
}

set sig_add [gtkwave::addSignalsFromList $signalsList]
puts "Signals added to viewer."

