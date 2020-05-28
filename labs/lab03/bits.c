// CP1521 Lab (warm-up)

#include <stdio.h>


int main()
{
	// Code to generate and display the largest "int" value
	int x = 0;
	for (int i = 0; i < (8 * sizeof x) - 1; i++) {
		x |= 1 << i;
	}
	printf("int %x, %d\n", x, x);

	// Code to generate and display the largest "unsigned int" value
	unsigned int y = 0;
	for (unsigned int i = 0; i < 8 * sizeof y; i++) {
		y |= (unsigned int) 1 << i;
	}
	printf("unsigned int %x, %u\n", y, y);

	// Code to generate and display the largest "long int" value
	long int xx = 0;
	for (long int i = 0; i < (8 * sizeof xx) - 1; i++) {
		xx |= (long int) 1 << i;
	}
	printf("long int %lx, %ld\n", xx, xx);

	// Code to generate and display the largest "unsigned long int" value
	unsigned long int xy = 0;
	for (unsigned long int i = 0; i < 8 * sizeof xy; i++) {
		xy |= (unsigned long int) 1 << i;
	}
	printf("unsigned long int %lx, %lu\n", xy, xy);

	// Code to generate and display the largest "long long int" value
	long long int xxx = 0;
	for (long long int i = 0; i < (8 * sizeof xxx) - 1; i++) {
		xxx |= (long long int) 1 << i;
	}
	printf("long long int %llx, %lld\n", xxx, xxx);

	// Code to generate and display the largest "unsigned long long int" value
	unsigned long long int xxy = 0;
	for (unsigned long long int i = 0; i < 8 * sizeof xxy; i++) {
		xxy |= (unsigned long long int) 1 << i;
	}
	printf("unsigned long long int %llx, %llu\n", xxy, xxy);

	return 0;
}


