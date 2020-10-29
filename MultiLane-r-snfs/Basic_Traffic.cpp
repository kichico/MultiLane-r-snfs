#include "Basic_Traffic.h"
#include <string>
#include <iostream>
#include <fstream>

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
	information.elapsedtime++;
	//CHECK(map_information.map.ID.current, map_information.map.existence.current);
	//getchar();
}

void Basic_Traffic::_calculation_neglect_C_and_D(double C_,double D_) {
	for (int i = 5; i < 95; i += 5) {
		double global_rho = i / 100.0;
		N = int(global_rho * constant.lane_length);
		//if (double(N) != global_rho * Map.Length) global_rho = car.constants.N / Map.Length;
		std::string Filename1 = "C:/Users/sueki/source/repos/Result/singlelane_r-nfs_tanaka_";
		std::string Filename2 = "C:/Users/sueki/source/repos/Result/singlelane_r-nfs_tanimoto_";
		std::string Filename3 = "C:/Users/sueki/source/repos/Result/Position_time_fixedVmax_1800idle.csv";
		std::ofstream ofsAV1(Filename1, std::ios::app);
		std::ofstream ofsAV2(Filename2, std::ios::app);
		std::ofstream ofsPT(Filename3, std::ios::app);
		ofsAV1 << "Q,k" << std::endl;
		ofsAV2 << "Q,k," << std::endl;
		ofsPT << "elapsedtime,position" << std::endl;
		std::cout << "Q => k" << std::endl;
		for (int n = 0; n < 1/*constant.ensemble*/; n++) {
			double _q = 0;
			initialize();
			information.elapsedtime = 0;
			for (int i = 0; i < constant.run_up_steps; i++) {
				_proceed();
			}
			for (int i = 0; i < constant.mesurement_steps; i++) {
				_proceed();
				_q += information.q;
				for (int j = 0; j < N; j++) {
					ofsPT << information.elapsedtime << "," << car.position.current[j] << std::endl;
				}
			}
				//std::cout << "n: " << n + 1 << " car: " << number_of_cars << " C: " << C << " D: " << D << " q: " << _q / double(constant.mesurement_steps);
				//std::cout << " left: " << double(information.turn_left) / double(10000 + constant.mesurement_steps) << " right: " << double(information.turn_right) / double(10000 + constant.mesurement_steps) << std::endl;
				//q += _q / double(constant.mesurement_steps);
		}
		//q /= double(constant.ensemble);
		ofsAV1.close();
		ofsAV2.close();
		ofsPT.close();
	}
}