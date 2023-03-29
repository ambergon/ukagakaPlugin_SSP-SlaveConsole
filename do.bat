


del /q main.dll
g++ -pthread -shared -o main.dll main.cpp -I ./include  liblua54.a -g
@rem g++ -shared -o main.dll main.cpp -I ./include  liblua54.a -g
ssp /g Ghost_Mine
