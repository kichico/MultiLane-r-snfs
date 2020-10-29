#include "Basic_Traffic.h"

int main() {
	Basic_Traffic basic_traffic;
	basic_traffic._calculation_neglect_C_and_D();
	return 0;
	/*for (int c = 0; c <= 10; c++) {
		double C = c * 0.1;
		double D = 1.0 - C;
		for (int number_of_cars = 1; number_of_cars <= 500; number_of_cars += 1) {
			double rho = double(number_of_cars) / (double(basic_traffic.constant.number_of_lanes)*double(basic_traffic.constant.lane_length));
			basic_traffic.calculation(number_of_cars, C, D);
			std::cout << " rho: " << rho << " q: " << basic_traffic.q << std::endl << std::endl;
		}
	}
	*/

}