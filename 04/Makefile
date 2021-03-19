CPPFLAGS+=-std=c++20 -O2 -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op -Wmissing-field-initializers -Wctor-dtor-privacy  -Wnon-virtual-dtor -Wstrict-null-sentinel  -Wold-style-cast -Woverloaded-virtual -Wsign-promo -Weffc++
.PHONY: all clear

all: main

main: main.o bigint.o vector.o
	g++ $(CPPFLAGS) $^ -o $@

main.o: main.cpp
	g++ $(CPPFLAGS) -c $< -o $@

bigint.o: bigint.cpp
	g++ $(CPPFLAGS) -c $< -o $@

vector.o: vector.cpp
	g++ $(CPPFLAGS) -c $< -o $@

test: main
	./main

clear:
	rm -rf *.o
