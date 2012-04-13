all:
	g++ -c *.cpp -O3
	g++ -o main *.o -lboost_system -L/home/garethf/scratch/usr/lib -O3