reset
set datafile separator ";"
set xdata time
set timefmt "%d"
set format x "%d"
set xlabel "Jours"     lfeknelfn
set ylabel "Valeurs mesurées"
set key off
set term png
set output "multi-lines.png"
plot 'sorted_temp3.csv' using 2:3:(i) with lines lc int(column(3)==i) title sprintf("Heure %01d", i)
