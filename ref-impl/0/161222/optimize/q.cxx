#include <random>

int main(){
	std::mt19937 rnd;
	std::uniform_int_distribution<> dist(-20, 20);
	for(int i=0; i<1000; ++i){
		int a = dist(rnd);
		int b = dist(rnd);
		int c = dist(rnd);
		printf("%d\t%d\t%d\t%d\n", a-b+2*c, a, b, c);
	}
}
