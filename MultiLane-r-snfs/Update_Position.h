#ifndef BASIC_TRAFFIC_UPDATE_POSITION_H
#define BASIC_TRAFFIC_UPDATE_POSITION_H
#include "Lane_Change.h"

void Update_Position::_update_position() {
	//全車両の速度が変化しなくなるまで繰り返す
	//常時，前後関係を更新する
	//次ステップに備え，先頭車両を選定する
	information.sum_velocity = 0;
	LCI = std::vector<Lead_Car_ID>(constant.number_of_lanes);
	//ここも並列計算できる
	for (int i = 0; i < constant.number_of_lanes; i++) if (lead_car_ID[i].existence) {
		int lead_car = lead_car_ID[i].ID;
		LCI[i].existence = false;
		//先頭車両が最大速度で移動出来た，もしくはレーンの速度が変化しなくなれば，このループを抜ける
		bool fg = true;
		while (fg) fg = _update_position_front_to_back(i);
		information.sum_velocity += information.lane_velocity[i];
	}
	lead_car_ID = LCI;
	//CHECK(map_information.update_position.ID, map_information.update_position.existence);
}

bool Update_Position::_update_position_front_to_back(int lane_number) {
	bool flag = true;
	map_information.update_position.existence[lane_number] = std::vector<bool>(constant.lane_length, false);
	int previous_lane_velocity = information.lane_velocity[lane_number];
	information.lane_velocity[lane_number] = 0;
	int ID = lead_car_ID[lane_number].ID;
	//ループの前に先頭車両の前方車両(最後尾)の位置を登録しておく
	map_information.update_position.existence[lane_number][car.position.current[car.around.front.current.ID[ID]]] = true;
	//先頭車両から，後続車両へ向かってループ
	//つまり，先頭車両から順に後続車両に向かって自車の位置を更新していく
	while (true) {
		//まだ移動の余地がある車両のみ考える
		if (still_room_for_movement[ID]) _move_forward_car(lane_number, ID);
		else {
			int next_position = car.position.current[ID];
			map_information.update_position.existence[lane_number][next_position] = true;
			map_information.update_position.ID[lane_number][next_position] = ID;
		}
		if (ID == lead_car_ID[lane_number].ID and map_information.number_of_cars[lane_number] > 1) map_information.update_position.existence[lane_number][car.position.current[car.around.front.current.ID[ID]]] = false;
		information.lane_velocity[lane_number] += canditate_velocity[ID];
		if (!LCI[lane_number].existence) {
			LCI[lane_number].existence = true;
			LCI[lane_number].ID = ID;
			LCI[lane_number].maximum_gap = car.around.front.current.distance[ID];
		}
		else {
			if (car.around.front.current.distance[ID] > LCI[lane_number].maximum_gap) {
				LCI[lane_number].ID = ID;
				LCI[lane_number].maximum_gap = car.around.front.current.distance[ID];
			}
		}
		if (lead_car_ID[lane_number].ID == car.around.rear.current.ID[ID]) {
			//先頭車両に戻ってきたので終了
			//先頭車両と最後尾車両の距離を更新
			int rear_ID = car.around.rear.current.ID[ID];
			int rear_distance = car.position.current[ID] - car.position.current[rear_ID];
			if (rear_distance <= 0) rear_distance += constant.lane_length;
			car.around.rear.current.distance[ID] = car.around.front.current.distance[rear_ID] = rear_distance;
			if (rear_distance > LCI[lane_number].maximum_gap) {
				LCI[lane_number].ID = rear_ID;
				LCI[lane_number].maximum_gap = rear_distance;
			}
			break;
		}
		else ID = car.around.rear.current.ID[ID];
	}
	if (previous_lane_velocity == information.lane_velocity[lane_number] or !still_room_for_movement[lead_car_ID[lane_number].ID]) flag = false;
	return flag;
}

void Update_Position::_move_forward_car(int lane_number, int ID) {
	//現在の位置を使って次のpositionを一つずつ加算していく
	//最大移動位置になっても前方車両に追いつかなければ，最大移動できる
	int remaining_distance = car.velocity[ID] - canditate_velocity[ID];
	int _v = canditate_velocity[ID];
	bool clash = false;
	int front_position;
	std::vector<int> check_position;
	for (int i = 1; i <= remaining_distance; i++) {
		front_position = car.position.current[ID] + i;
		if (front_position >= constant.lane_length) front_position -= constant.lane_length;
		check_position.emplace_back(front_position);
		if (map_information.update_position.existence[lane_number][front_position]) {
			clash = true;
			break;
		}
	}
	int next_position = car.position.previous[ID] + car.velocity[ID];
	if (clash) next_position = front_position - 1;
	else still_room_for_movement[ID] = false;	//最大移動を達成
	if (next_position >= constant.lane_length) next_position -= constant.lane_length;
	if (next_position < 0) next_position += constant.lane_length;
	_check_clash(ID, lane_number, next_position, clash);	//クラッシュしていないかチェック
	int v = next_position - car.position.previous[ID];
	if (v < 0) v += constant.lane_length;
	canditate_velocity[ID] = v;
	_check_velocity(ID, _v, &check_position, next_position, clash);	//スピードがおかしくないかチェック
	map_information.update_position.existence[lane_number][next_position] = true;
	map_information.update_position.ID[lane_number][next_position] = ID;
	car.position.current[ID] = next_position;
	//自車と前方車両の距離を更新
	int front_ID = car.around.front.current.ID[ID];
	int front_distance = car.position.current[front_ID] - car.position.current[ID];
	if (front_distance <= 0) front_distance += constant.lane_length;
	car.around.front.current.distance[ID] = car.around.rear.current.distance[front_ID] = front_distance;
}

void Update_Position::_check_clash(int ID, int lane_number, int next_position, bool clash) {
	if (map_information.update_position.existence[lane_number][next_position]) {
		std::cout << "CLASH " << ID << " " << map_information.update_position.ID[lane_number][next_position] << std::endl;
		std::cout << car.velocity[ID] << " " << canditate_velocity[ID] << std::endl;
		std::cout << car.position.previous[ID] << " " << next_position << std::endl;
		std::cout << clash << std::endl;
		CHECK(map_information.update_position.ID, map_information.update_position.existence);
		getchar();
		exit(EXIT_FAILURE);
	}
}

void Update_Position::_check_velocity(int ID, int _v, std::vector<int>* check_position, int next_position, bool clash) {
	if (canditate_velocity[ID] < 0 or car.velocity[ID] < canditate_velocity[ID]) {
		std::cout << "ERROR VELOCITY in loop " << ID << " " << canditate_velocity[ID] << " " << car.velocity[ID] << " " << _v << std::endl;
		std::cout << next_position << " " << car.position.previous[ID] << std::endl;
		std::cout << clash << std::endl;
		std::cout << "check position ::";
		for (int w = 0; w < (*check_position).size(); w++) std::cout << " " << (*check_position)[w];
		std::cout << std::endl;
		CHECK(map_information.update_position.ID, map_information.update_position.existence);
		getchar();
		exit(EXIT_FAILURE);
	}
}

#endif // !BASIC_TRAFFIC_UPDATE_POSITION_H
