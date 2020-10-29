#ifndef BASIC_TRAFFIC_H
#define BASIC_TRAFFIC_H
#include "Update_Position.h"

class Basic_Traffic : public Update_Position {
	/*
		モデル全体を司るクラス
		全てのクラスを継承している
	*/
public:
	double q;	//流量
	double rho;	//密度
	void calculation(int number_of_cars, double C, double D) { _calculation(number_of_cars, C, D); }	//モデルの計算
	void _calculation_neglect_C_and_D(double C_,double D_);
private:
	void _calculation(int number_of_cars, double C, double D);	//実装
	void _proceed();
};




#endif // !BASIC_TRAFFIC_H

