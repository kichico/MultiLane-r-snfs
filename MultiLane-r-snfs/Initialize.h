#ifndef BASIC_TRAFFIC_INITIALIZE_H
#define BASIC_TRAFFIC_INITIALIZE_H
#include "Class.h"

void Initialize::_initialize() {
	//初期化を行う
	car.initialize_car(N);
	information.initialize_information(constant.number_of_lanes);
	map_information.initialize_Map_Information(constant.number_of_lanes, constant.lane_length);
	_initialize_cars_strategy();
	_initialize_cars_position();
	_initial_search();
}

void Initialize::_initialize_cars_strategy() {
	//各車両の戦略を戦略比に沿って，ランダムに決定する
	int numC, numD;	//C戦略の車両数とD戦略の車両数
	if (C == 0) {
		numC = 0;
		numD = N;
	}
	else if (D == 0) {
		numC = N;
		numD = 0;
	}
	else {
		numC = int(N * C + 0.5);
		if (numC < 0) numC = 0;
		if (numC > N) numC = N;
		numD = N - numC;
	}
	if (numC > 0) {
		std::vector<int> remaining_ID(N);
		for (int ID = 0; ID < N; ID++) remaining_ID[ID] = ID;
		while (remaining_ID.size() != numD) {
			int rem_size = remaining_ID.size();
			int xrem = random(rem_size - 1);
			int num = remaining_ID[xrem];
			car.strategy[num] = Strategy::C;
			remaining_ID.erase(remaining_ID.begin() + xrem);
		}
	}
}

void Initialize::_initialize_cars_position() {
	//各車両の初期位置をランダムに決定する
	struct Car_Position {
		int lane;
		int coordinate;
	};
	std::vector<Car_Position> cp;
	cp.reserve(constant.lane_length * constant.number_of_lanes);
	for (int i = 0; i < constant.lane_length; i++) for (int j = 0; j < constant.number_of_lanes; j++) {
		Car_Position _cp;
		_cp.lane = j;
		_cp.coordinate = i;
		cp.emplace_back(_cp);
	}
	for (int ID = 0; ID < N; ID++) {
		int rem_cp = cp.size();
		int xrem = random(rem_cp - 1);
		Car_Position _cp = cp[xrem];
		cp.erase(cp.begin() + xrem);
		car.position.current[ID] = _cp.coordinate;
		car.position.previous[ID] = _cp.coordinate;
		car.lane_number[ID] = _cp.lane;
		map_information.map.existence.current[_cp.lane][_cp.coordinate] = map_information.map.existence.previous[_cp.lane][_cp.coordinate] = true;
		map_information.map.ID.current[_cp.lane][_cp.coordinate] = map_information.map.ID.previous[_cp.lane][_cp.coordinate] = ID;
		map_information.number_of_cars[_cp.lane]++;
		if (random(1.0) < constant.r) car.S[ID] = constant.S;
		else car.S[ID] = 1;
	}
}

void Initialize::_initial_search() {
	//ここでは初期配置の車について，前後車両情報のみを調査する
	Lead_Car_ID lci;
	lci.existence = false;
	lead_car_ID = std::vector<Lead_Car_ID>(constant.number_of_lanes, lci);
	for (int i = 0; i < constant.number_of_lanes; i++) if (map_information.number_of_cars[i] > 0) {
		int start;
		int rear;
		for (int j = 0; j < constant.lane_length; j++) if (map_information.map.existence.current[i][j]) {
			start = j;
			rear = map_information.map.ID.current[i][j];
			break;
		}
		for (int j = 1; j <= constant.lane_length; j++) {
			int position = start + j;
			if (position >= constant.lane_length) position -= constant.lane_length;
			if (map_information.map.existence.current[i][position]) {
				int front = map_information.map.ID.current[i][position];
				car.around.front.current.ID[rear] = car.around.front.previous.ID[rear] = front;
				car.around.rear.current.ID[front] = car.around.rear.previous.ID[front] = rear;
				int distance = car.position.current[front] - car.position.current[rear];
				if (distance <= 0) distance += constant.lane_length;
				car.around.front.current.distance[rear] = car.around.front.previous.distance[rear] = distance;
				car.around.rear.current.distance[front] = car.around.rear.previous.distance[front] = distance;
				if (!lead_car_ID[i].existence) {
					lead_car_ID[i].existence = true;
					lead_car_ID[i].ID = rear;
					lead_car_ID[i].maximum_gap = distance;
				}
				else {
					if (distance >= lead_car_ID[i].maximum_gap) {
						lead_car_ID[i].ID = rear;
						lead_car_ID[i].maximum_gap = distance;
					}
				}
				rear = front;
			}
		}
	}
	//CHECK(map_information.map.ID.current, map_information.map.existence.current);
	//getchar();
}

void Initialize::_CHECK(std::vector<std::vector<int>>& ID, std::vector<std::vector<bool>>& existence) {
	std::cout << "***************************************************************" << std::endl;
	bool check = true;
	for (int i = 0; i < constant.number_of_lanes; i++) {
		std::cout << i << ": ";
		int num = 0;
		for (int j = 0; j < constant.lane_length; j++) if (existence[i][j]) {
			std::cout << "(" << ID[i][j] << "," << j << ") ";
			num++;
		}
		std::cout << ":: " << num << " " << map_information.number_of_cars[i] << std::endl;
		if (num != map_information.number_of_cars[i]) check = false;
		num = 0;
		std::cout << " : ";
		if (lead_car_ID[i].existence) {
			num = 1;
			int ID = lead_car_ID[i].ID;
			while (true) {
				std::cout << "(" << ID << "," << car.position.current[ID] << ") ";
				ID = car.around.rear.current.ID[ID];
				if (ID == lead_car_ID[i].ID) break;
				else num++;
			}
		}
		std::cout << ":: " << num << " " << map_information.number_of_cars[i] << std::endl;
		if (num != map_information.number_of_cars[i]) check = false;
	}
	std::cout << "***************************************************************" << std::endl;
	if (!check) {
		getchar();
		exit(EXIT_FAILURE);
	}
}

#endif // !BASIC_TRAFFIC_INITIALIZE_H

