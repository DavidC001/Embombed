
exec: $(FILE)
	gcc -I "../msp/" -I "../libs/" -I "../games/" -D __MSP432P401R__ -D TESTING -pthread --no-warnings -o exec $(FILE)

clean:
	rm -f exec