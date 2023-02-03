all : sort_tp1_abr.o sort_tp2_abr.o sort_tp3_abr.o sort_w_abr.o sort_h_abr.o sort_m_abr.o clean

sort_tp1_abr.o : sort_tp1_abr.c
	gcc -o sort_tp1_abr.o -c sort_tp1_abr.c
	
sort_tp2_abr.o : sort_tp2_abr.c
	gcc -o sort_tp2_abr.o -c sort_tp2_abr.c

sort_tp3_abr.o : sort_tp3_abr.c
	gcc -o sort_tp3_abr.o -c sort_tp3_abr.c
	
sort_w_abr.o : sort_w_abr.c
	gcc -o sort_w_abr.o -c sort_w_abr.c
	
sort_h_abr.o : sort_h_abr.c
	gcc -o sort_h_abr.o -c sort_h_abr.c

sort_m_abr.o : sort_m_abr.c
	gcc -o sort_m_abr.o -c sort_m_abr.c

clean : 
	rm -f *.o
