#ifndef BASIC_TRAFFIC_H
#define BASIC_TRAFFIC_H
#include "Update_Position.h"
#include <fstream>

class Basic_Traffic : public Update_Position {
	/*
		モデル全体を司るクラス
		全てのクラスを継承している
	*/
public:
	double q;	//流量
	double rho;	//密度
	//void calculation(int number_of_cars, double C, double D) { _calculation(number_of_cars, C, D); }	//モデルの計算
	void _calculation_neglect_C_and_D();
private:
	void _calculation(int number_of_cars, double C, double D);	//実装
	void _proceed();
};

/*class Basic_Traffic:public Update_Position {
private:
	void _proceed();
public:
	void _calculation_neglect_C_and_D();
};
*/
/*
void Basic_Traffic::_calculation(int number_of_cars, double C_, double D_) {
	N = number_of_cars;
	C = C_;
	D = D_;
	q = 0;
	for (int n = 0; n < constant.ensemble; n++) {
		double _q = 0;
		initialize();
		for (int i = 0; i < constant.run_up_steps; i++) {
			//if ((i + 1) % 1000 == 0) std::cout << i + 1 << std::endl;
			_proceed();
		}
		for (int i = 0; i < constant.mesurement_steps; i++) {
			_proceed();
			_q += information.q;
		}
		std::cout << "n: " << n + 1 << " car: " << number_of_cars << " C: " << C << " D: " << D << " q: " << _q / double(constant.mesurement_steps);
		std::cout << " left: " << double(information.turn_left) / double(10000 + constant.mesurement_steps) << " right: " << double(information.turn_right) / double(10000 + constant.mesurement_steps) << std::endl;
		q += _q / double(constant.mesurement_steps);
	}
	q /= double(constant.ensemble);
}

void Basic_Traffic::_proceed() {
	/*流れ
	ルール1から4を適応
	車線変更の意思を問う
	一旦位置更新
	車線変更
	もう一度位置更新
	データアップデート
	*/

	/*
	apply_rules_1_to_4();
	try_lane_change();
	//位置情報更新の前に，現在情報を保存
	car.update_previous_information();	//一括情報更新 (位置情報，自車周り情報)
	map_information.map.existence.previous = map_information.map.existence.current;
	map_information.update_position.existence = std::vector<std::vector<bool>>(constant.number_of_lanes, std::vector<bool>(constant.lane_length, false));
	map_information.map.ID.previous = map_information.update_position.ID = map_information.map.ID.current;
	canditate_velocity = std::vector<int>(N, 0);
	still_room_for_movement = std::vector<bool>(N, true);
	//位置を更新
	update_position();	//一回目位置更新
	bool lane_change_flag = lane_change();		//落ち着いたところで車線変更
	if (lane_change_flag) update_position();	//車線変更した車と，その周辺の車の速度回復(是正)	車線変更がなければ，このupdateは行わない
	//全情報更新
	car.velocity = canditate_velocity;
	map_information.map.existence.current = map_information.update_position.existence;
	map_information.map.ID.current = map_information.update_position.ID;
	information.q = double(N) / (double(constant.number_of_lanes) * double(constant.lane_length)) * double(information.sum_velocity) / double(N);
	//CHECK(map_information.map.ID.current, map_information.map.existence.current);
	//getchar();
}

void Basic_Traffic::_calculation_neglect_C_and_D() {
	for (int n = 0; n < constant.ensemble; n++) {
		double _q = 0;
		initialize();
		for (int i = 0; i < constant.run_up_steps; i++) {
			_proceed();
		}
		for (int i = 0; i < constant.mesurement_steps; i++) {
			_proceed();
			_q += information.q;
		}
		std::cout<<
		//std::cout << "n: " << n + 1 << " car: " << number_of_cars << " C: " << C << " D: " << D << " q: " << _q / double(constant.mesurement_steps);
		//std::cout << " left: " << double(information.turn_left) / double(10000 + constant.mesurement_steps) << " right: " << double(information.turn_right) / double(10000 + constant.mesurement_steps) << std::endl;
		//q += _q / double(constant.mesurement_steps);
	}
	//q /= double(constant.ensemble);
}
*/
#endif // !BASIC_TRAFFIC_H

