all:
	g++ -c *.cpp -O3
	g++ -o main *.o -lboost_system -O3