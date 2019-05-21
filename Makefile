Mcc: Mcc.c


test: Mcc
	./test.sh


clean:
	rm -f Mcc *.o* ~tmp*
