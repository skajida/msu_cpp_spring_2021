CPPFLAGS+=-std=c++17 -O2 -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op -Wmissing-field-initializers -Wctor-dtor-privacy  -Wnon-virtual-dtor -Wstrict-null-sentinel  -Wold-style-cast -Woverloaded-virtual -Wsign-promo -Weffc++
.PHONY: all clear

all: main

main: main.o serializer.o deserializer.o
	g++ $(CPPFLAGS) $^ -o $@

main.o: main.cpp
	g++ $(CPPFLAGS) -c $< -o $@

serializer.o: serializer.cpp
	g++ $(CPPFLAGS) -c $< -o $@

deserializer.o: deserializer.cpp
	g++ $(CPPFLAGS) -c $< -o $@

test: main
	./main

clear:
	rm -rf *.o
