#ifndef BASIC_TRAFFIC_H
#define BASIC_TRAFFIC_H
#include "Update_Position.h"

class Basic_Traffic : public Update_Position {
	/*
		���f���S�̂��i��N���X
		�S�ẴN���X���p�����Ă���
	*/
public:
	double q;	//����
	double rho;	//���x
	void calculation(int number_of_cars, double C, double D) { _calculation(number_of_cars, C, D); }	//���f���̌v�Z
	void _calculation_neglect_C_and_D(double C_,double D_);
private:
	void _calculation(int number_of_cars, double C, double D);	//����
	void _proceed();
};




#endif // !BASIC_TRAFFIC_H

