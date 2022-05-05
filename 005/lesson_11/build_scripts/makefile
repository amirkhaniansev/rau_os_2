all: ../include/nt_man.hpp ../include/currency.hpp nt_man.o
		g++ nt_man.o ../src/main.cpp -o rclient.o

nt_man.o: ../include/nt_man.hpp ../include/url_info.hpp ../include/currency.hpp ../src/nt_man.cpp
		g++ ../src/nt_man.cpp -c -o nt_man.o