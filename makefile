TARGETS:=Purify

Purify: src/Purify.cc include/Purify.hh
	g++ -Iinclude src/Purify.cc -o Purify -O3 -Wall

clean:
	rm $(TARGETS)
	rm *~