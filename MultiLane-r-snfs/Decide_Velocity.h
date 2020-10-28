#ifndef BASIC_TRAFFIC_DECIDE_VELOCITY_H
#define BASIC_TRAFFIC_DECIDE_VELOCITY_H
#include "Initialize.h"

void Decide_Velocity::_apply_rules_1_to_4() {
	information.sum_velocity = 0;
	information.lane_velocity = std::vector<int>(constant.number_of_lanes, 0);
	information.v0 = car.velocity;
	//このループは並列処理が可能
	//将来的には並列計算に変更したい，こんだけ丁寧に関数に分かれていれば，すぐにできるはず
	for (int ID = 0; ID < N; ID++) {
		_rule1(ID);
		_rule2(ID);
		_rule3(ID);
		_rule4(ID);
		information.sum_velocity += car.velocity[ID];
		information.lane_velocity[car.lane_number[ID]] += car.velocity[ID];
	}
}

void Decide_Velocity::_rule1(int ID) {
	if (car.around.front.current.distance[ID] >= constant.G or car.velocity[ID] <= information.v0[car.around.front.current.ID[ID]]) car.velocity[ID]++;
	if (car.velocity[ID] > constant.Vmax) car.velocity[ID] = constant.Vmax;
	if (car.velocity[ID] < 0) {
		std::cout << "v1 :: " << ID << std::endl;
		std::cout << car.velocity[ID] << std::endl;
	}
}

void Decide_Velocity::_rule2(int ID) {
	int distance_S = 0;
	if (random(1.0) <= constant.q) {
		car.S[ID] = 1;
		if (random(1.0) < constant.r) car.S[ID] = constant.S;
		int s = 0;
		int id = ID;
		while (s < car.S[ID]) {
			distance_S += car.around.front.previous.distance[id];
			id = car.around.front.previous.ID[id];
			s++;
		}
		int canditate_v2 = distance_S - car.S[ID];
		if (canditate_v2 < car.velocity[ID]) car.velocity[ID] = canditate_v2;
	}
	if (car.velocity[ID] < 0) {
		std::cout << "v2 :: " << ID << std::endl;
		std::cout << car.velocity[ID] << " " << distance_S << " " << car.S[ID] << std::endl;
		int s = 0;
		int id = ID;
		while (s < car.S[ID]) {
			std::cout << id << " " << car.around.front.previous.ID[id] << std::endl;
			id = car.around.front.previous.ID[id];
			s++;
		}
		getchar();
	}
}
void Decide_Velocity::_rule3(int ID) {
	int s = 0;
	int id = ID;
	int distance_S = 0;
	while (s < car.S[ID]) {
		distance_S += car.around.front.current.distance[id];
		id = car.around.front.current.ID[id];
		s++;
	}
	int canditate_v3 = distance_S - car.S[ID];
	if (canditate_v3 < car.velocity[ID]) car.velocity[ID] = canditate_v3;
	if (car.velocity[ID] < 0) {
		std::cout << "v3 :: " << ID << std::endl;
		std::cout << car.velocity[ID] << " " << distance_S << " " << canditate_v3 << std::endl;
	}
}
void Decide_Velocity::_rule4(int ID) {
	double p = constant.p1;
	if (car.around.front.current.distance[ID] < constant.G) {
		int front = car.around.front.current.ID[ID];
		if (information.v0[ID] < information.v0[front]) p = constant.p2;
		else if (information.v0[ID] == information.v0[front]) p = constant.p3;
		else p = constant.p4;
	}
	if (random(1.0) < p) {
		int canditate_v4 = car.velocity[ID] - 1;
		if (1 < canditate_v4) car.velocity[ID] = canditate_v4;
		else car.velocity[ID] = 1;
	}
	if (car.velocity[ID] < 0) {
		std::cout << ID << " こっちでエラー" << std::endl;
		std::cout << car.velocity[ID] << std::endl;
		getchar();
		exit(EXIT_FAILURE);
	}
}

#endif // !BASIC_TRAFFIC_DECIDE?VELOCITY_H
