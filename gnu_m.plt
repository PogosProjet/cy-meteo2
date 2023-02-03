set dgrid3d
set xlabel "Longitude"
set ylabel "Latitude"
set datafile separator ";"
set pm3d map interpolate 64,64
splot "sorted_m_abr.csv" using 4:3:1 w pm3d title "Moisture for each station"
