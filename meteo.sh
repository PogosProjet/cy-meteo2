
#!/bin/bash

file=""
nb_of_files=0
nb_of_date=0
geo_jobs=""
nb_of_geo_jobs=0
var_jobs=""



for i in `seq $#` ; do #Get the name of the given file 
arg=${!i}
case $arg in 

'-f')
nb_of_files=$((nb_of_files+1));i=$((i+1));file=${!i};;

*);;
esac
done


for i in `seq $#` ; do #Check if parameters format is ok 
arg=${!i}
case $arg in 

'-f') ;;

$file) ;;

'-d') ;;

$date_min) ;;

$date_max) ;;

'-'[FGSAOQ])
geo_jobs="$geo_jobs $arg"; nb_of_geo_jobs=$((nb_of_geo_jobs+1));;

'-'[tp][123])
var_jobs="$var_jobs $arg";;

'-'[whm])
var_jobs="$var_jobs $arg";;

'--help')
cat help.txt; exit 5 ;;        

*)echo "Error :"'"'$arg'"' "is an incorrect input parameter." ; exit 5 ;;
esac
done

#######################################################################################





#CHECKING IF THE PARAMETERS ARE VALIDS


if [ $nb_of_files == 0 ]
then
	echo "Error : No input file."
	exit 5 
fi
if [ $nb_of_files -gt 1 ]
then
	echo "Error : To much input files."
	exit 5 
fi
if [ -e $file ] && [[ "$file" == *".csv" ]]
then
	echo 
else
	echo "Error : Input file format is wrong or doesn't existe."
	exit 5;
fi

if [ $nb_of_geo_jobs -gt 1 ] 
then
    echo "Incorrect input parameters."
    echo "Choose only one localisation commande between -F, -G, -S, -A, -O, -Q."
    exit 5;
fi


########################################################################################

echo 'Input file choosen is "'$file'" (last file given).'


# perform all the jobs 


#MODIFIES INPUT FILE TO START FILTERS

cat $file | tr 'T' ';' | tr -d '-' | tail -n+2 > meteo.csv




for j in $geo_jobs ;  do   # FILTER LOCALISATIONS

case $j in

'-F') cat meteo.csv | tr ',' ';' | awk -F ';' '{if($11>40 && $11<52 && $12>-10 && $12<12) {print $0}}' > filtered_geo.csv
;;
'-G') cat meteo.csv | tr ',' ';' | awk -F ';' '{if($11>-1 && $11<10 && $12>-58 && $12<-48) {print $0}}' > filtered_geo.csv
;;
'-S') cat meteo.csv | tr ',' ';' | awk -F ';' '{if($11>44 && $11<53 && $12>-64 && $12<-49) {print $0}}' > filtered_geo.csv 
;;
'-A') cat meteo.csv | tr ',' ';' | awk -F ';' '{if($11>12 && $11<19 && $12>-65 && $12<-58) {print $0}}' >  filtered_geo.csv  
;; 
'-O') cat meteo.csv | tr ',' ';' | awk -F ';' '{if($11>-60 && $11<3 && $12>26 && $12<100) {print $0}}' >  filtered_geo.csv
;;
'-Q') cat meteo.csv | tr ',' ';' | awk -F ';' '{if($11>-75 && $11<-53 && $12>102 && $12<150) {print $0}}' >  filtered_geo.csv
;;
*)exit 31;;
esac
done
rm meteo.csv  #meteo.csv is not anymore needed


for j in $var_jobs ;  do   # FILTER VARIABLE 
case $j in

'-t1')  cat filtered_geo.csv | cut -d ';' -f1,13 | grep -E ';$|;;' -v > filtered_temp1.csv    
;;
'-t2')  cat filtered_geo.csv | cut -d ';' -f2,13 | tr -d '-' | grep -E ';$|;;' -v > filtered_temp2.csv  
;;
'-p1')  cat filtered_geo.csv | cut -d ';' -f1,4 | grep -E ';$|;;' -v > filtered_pressure1.csv
;; 
'-p2')  cat filtered_geo.csv | cut -d ';' -f2,4 | tr -d '-' | grep -E ';$|;;' -v > filtered_pressure2.csv     
;;
'-w' )  cat filtered_geo.csv | cut -d ';' -f1,5,6 | grep -E ';$|;;' -v > filtered_wind.csv
;;
'-h' )  cat filtered_geo.csv | cut -d ';' -f1,16 | grep -E ';$|;;' -v > filtered_height.csv
;;
'-m' )  cat filtered_geo.csv | cut -d ';' -f1,7 | grep -E ';$|;;' -v > filtered_moisture.csv
;;
*)exit 32;;
esac
done
rm filtered_geo.csv #filtered_meteo.csv is not anymore needed



make all #compiling C files

for j in $var_jobs ;  do
case $j in

'-t1')
chmod 777 sort_tp1_abr.c 
./sort_tp1_abr.c filtered_temp1.csv sorted_temp1.csv ;gnuplot -persist gnu_t1.plt
;;
'-t2')
chmod 777 sort_tp2_abr.c
./sort_tp2_abr.c filtered_temp2.csv sorted_temp2.csv ; 
;;
'-p1')
chmod 777 sort_tp1_abr.c
./sort_tp1_abr.c filtered_pressure1.csv sorted_pressure1.csv ;gnuplot -persist gnu_p1.plt 
		
;; 
'-p2')
chmod 777 sort_tp2_abr.c
./sort_tp2_abr.c filtered_pressure2.csv sorted_pressure2.csv 
;;
'-w' )
chmod 777 sort_w_abr.c
./sort_w_abr.c filtered_wind.csv sorted_wind.csv
;;
'-h' )
chmod 777 sort_h_abr.c
./sort_h_abr.c filtered_height.csv sorted_height.csv
;;
'-m' )
chmod 777 sort_m_abr.c
./sort_m_abr.c filtered_moisture.csv sorted_moisture.csv
;;
*)exit 33;;

esac
done

exit 
