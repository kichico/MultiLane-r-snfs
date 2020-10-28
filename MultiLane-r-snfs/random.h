#pragma once
#ifndef RANDOM_H
#define RANDOM_H
#include <random>

int create_int_rand(std::mt19937 mt, int xmin, int xmax) {
	std::uniform_int_distribution<> rd(xmin, xmax);
	return rd(mt);
}

double create_double_rand(std::mt19937 mt, double xmin, double xmax) {
	std::uniform_real_distribution<> rd(xmin, xmax);
	return rd(mt);
}

int random(int N) {
	std::random_device rnd;
	std::mt19937 mt(rnd());
	return create_int_rand(mt, 0, N);
}

int random(int Nmin, int Nmax) {
	std::random_device rnd;
	std::mt19937 mt(rnd());
	return create_int_rand(mt, Nmin, Nmax);
}

double random(double D) {
	std::random_device rnd;
	std::mt19937 mt(rnd());
	return create_double_rand(mt, 0.0, D);
}

double random(double Dmin, double Dmax) {
	std::random_device rnd;
	std::mt19937 mt(rnd());
	return create_double_rand(mt, Dmin, Dmax);
}

#endif	//RANDOM_H