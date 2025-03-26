g++ -std=c++23 -Wall -Wextra -Wold-style-cast -Werror -pedantic -O3 -march=native main.cpp Bill.cpp Customer.cpp Provider.cpp Service.cpp -latomic -ltbb -lpqxx -lpq && ./a.out
