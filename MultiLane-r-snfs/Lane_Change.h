#ifndef BASIC_TRAFFIC_LANE_CHANGE_H
#define BASIC_TRAFFIC_LANE_CHANGE_H
#include "Decide_Velocity.h"

void Lane_Change::_try_lane_change() {
	//車線変更を行う車両の順番を決める
	_create_canditate_of_lane_change(_decide_top_car());
	//std::cout << "finish> ";
	//インセンティブ確認
	information.try_turn.clear();
	information.try_turn_signal.clear();
	if (!canditate_lane_change.empty()) for (int i = 0; i < canditate_lane_change.size(); i++) {
		int ID = canditate_lane_change[i].ID;
		_check_insentive(i, Signal::left);	//左車線へ車線変更try
		_check_insentive(i, Signal::right);	//右車線へ車線変更try
		_decide_which_signal(i);	//後方車両の行動を決定
	}
}

int Lane_Change::_decide_top_car() {
	//まず各レーンから真の先頭車両を等確率で選ぶ
	std::vector<int> lead;
	for (int i = 0; i < constant.number_of_lanes; i++) if (lead_car_ID[i].existence) lead.emplace_back(lead_car_ID[i].ID);
	int lead_size = lead.size();
	int xlead = random(lead_size - 1);
	return lead[xlead];	//真の先頭車両
}

void Lane_Change::_create_canditate_of_lane_change(int leader) {
	//先頭車両から順にD戦略車両だけを抜き出していく この抜き出された順で車線変更を行っていく
	canditate_lane_change.clear();
	int lead_position = car.position.current[leader];
	int remaining = N;
	for (int i = 0; i <= constant.lane_length; i++) {
		int position = lead_position - i;
		if (position < 0) position += constant.lane_length;
		std::vector<Lane_Change::Canditate> same_position;
		for (int j = 0; j < constant.number_of_lanes; j++) {
			if (map_information.map.existence.current[j][position]) {
				int ID = map_information.map.ID.current[j][position];
				Lane_Change::Canditate canditate;
				switch (car.strategy[ID]) {
				case Strategy::D:
					canditate.ID = ID;
					canditate.winker.left = false;
					canditate.winker.right = false;
					same_position.emplace_back(canditate);
					break;
				default:
					break;
				}
				remaining--;
			}
		}
		if (!same_position.empty()) {
			if (same_position.size() > 1) while (same_position.size() > 0) {
				int same_size = same_position.size();
				int xsame = random(same_size - 1);
				Lane_Change::Canditate Cand = same_position[xsame];
				same_position.erase(same_position.begin() + xsame);
				canditate_lane_change.emplace_back(Cand);
			}
			else canditate_lane_change.emplace_back(same_position[0]);
		}
		if (remaining == 0) break;
	}
}

void Lane_Change::_check_insentive(int canditate_num, int signal) {
	bool turn_flag = false;
	bool fg = false;
	int ID = canditate_lane_change[canditate_num].ID;
	int current_lane = car.lane_number[ID];
	int position = car.position.current[ID];
	int side_lane = current_lane;
	if (signal == Signal::left) {
		if (car.lane_number[ID] > 0) {
			//左車線が存在すれば，左ウィンカーを出すことができる
			fg = true;
			side_lane += -1;
		}
	}
	else {
		if (car.lane_number[ID] < constant.number_of_lanes - 1) {
			//右車線が存在すれば，右ウィンカーを出すことができる
			fg = true;
			side_lane += 1;
		}
	}
	if (fg) {
		if (!map_information.map.existence.current[side_lane][position]) if (car.around.front.current.distance[ID] <= car.velocity[ID] - car.velocity[car.around.front.current.ID[ID]]) {
			//車線変更先に誰もいなかったら，絶対に車線変更できる
			if (map_information.number_of_cars[side_lane] == 0) {
				canditate_lane_change[canditate_num].side_information.front_side.distance = constant.lane_length;
				turn_flag = true;
			}
			else {
				_get_side_information(canditate_num, signal);
				if (canditate_lane_change[canditate_num].side_information.front_side.distance > car.velocity[ID] - car.velocity[canditate_lane_change[canditate_num].side_information.front_side.ID]) turn_flag = true;
			}
		}
	}
	if (signal == Signal::left) canditate_lane_change[canditate_num].winker.left = turn_flag;
	else canditate_lane_change[canditate_num].winker.right = turn_flag;
}

void Lane_Change::_get_side_information(int canditate_num, int signal) {
	int side_lane;
	int ID = canditate_lane_change[canditate_num].ID;
	if (signal == Signal::left) side_lane = car.lane_number[ID] - 1;
	else side_lane = car.lane_number[ID] + 1;
	int position = car.position.current[ID];
	int n = 1;
	while (true) {
		int front_position = position + n;
		int rear_position = position - n;
		if (front_position >= constant.lane_length) front_position -= constant.lane_length;
		if (rear_position < 0) rear_position += constant.lane_length;
		if (map_information.map.existence.current[side_lane][front_position]) {
			int front_side = map_information.map.ID.current[side_lane][front_position];
			canditate_lane_change[canditate_num].side_information.front_side.ID = front_side;
			canditate_lane_change[canditate_num].side_information.front_side.distance = n;
			canditate_lane_change[canditate_num].side_information.rear_side.ID = car.around.rear.current.ID[front_side];
			canditate_lane_change[canditate_num].side_information.rear_side.distance = car.position.current[ID] - car.position.current[canditate_lane_change[canditate_num].side_information.rear_side.ID];
			if (canditate_lane_change[canditate_num].side_information.rear_side.distance < 0) canditate_lane_change[canditate_num].side_information.rear_side.distance += constant.lane_length;
			break;
		}
		if (map_information.map.existence.current[side_lane][rear_position]) {
			int rear_side = map_information.map.ID.current[side_lane][rear_position];
			canditate_lane_change[canditate_num].side_information.rear_side.ID = rear_side;
			canditate_lane_change[canditate_num].side_information.rear_side.distance = n;
			canditate_lane_change[canditate_num].side_information.front_side.ID = car.around.front.current.ID[rear_side];
			canditate_lane_change[canditate_num].side_information.front_side.distance = car.position.current[ID] - car.position.current[canditate_lane_change[canditate_num].side_information.front_side.ID];
			if (canditate_lane_change[canditate_num].side_information.front_side.distance < 0) canditate_lane_change[canditate_num].side_information.front_side.distance += constant.lane_length;
			break;
		}
		n++;
	}
}

void Lane_Change::_decide_which_signal(int canditate_num) {
	//両方に車線変更できるなら，よりgapがある方へ車線変更
	if (canditate_lane_change[canditate_num].winker.left and canditate_lane_change[canditate_num].winker.right) {
		if (canditate_lane_change[canditate_num].side_information.front_side.distance > canditate_lane_change[canditate_num].side_information.front_side.distance) 
			canditate_lane_change[canditate_num].winker.right = false;
		else if (canditate_lane_change[canditate_num].side_information.front_side.distance == canditate_lane_change[canditate_num].side_information.front_side.distance) {
			double x = random(1.0);
			if (x <= 0.5) canditate_lane_change[canditate_num].winker.left = false;
			else canditate_lane_change[canditate_num].winker.right = false;
		}
		else canditate_lane_change[canditate_num].winker.left = false;
	}
	if (canditate_lane_change[canditate_num].winker.left or canditate_lane_change[canditate_num].winker.right) {
		information.try_turn.emplace_back(canditate_lane_change[canditate_num].ID);
		if (canditate_lane_change[canditate_num].winker.left) {
			information.try_turn_signal.emplace_back(Signal::left);
			information.turn_left++;
		}
		if (canditate_lane_change[canditate_num].winker.right) {
			information.try_turn_signal.emplace_back(Signal::right);
			information.turn_right++;
		}
	}
}

bool Lane_Change::_lane_change() {
	bool lane_change_flag = false;
	if (!information.try_turn.empty()) for (int i = 0; i < information.try_turn.size(); i++) {
		int ID = information.try_turn[i];
		int lanechange = 1;
		bool lanechange_fg = false;
		if (information.try_turn_signal[i] == Signal::left) lanechange = -1;
		//移動先のレーンに誰もいないのだから，無条件で車線変更できる
		if (map_information.number_of_cars[car.lane_number[ID] + lanechange] == 0) lanechange_fg = true;
		else {
			//移動先に車がいたら車線変更できない
			if (!map_information.update_position.existence[car.lane_number[ID] + lanechange][car.position.current[ID]]) {
				//まず車線変更先後方の車両の情報を取得
				int j = 1;
				while (true) {
					int position = car.position.current[ID] - j;
					if (position < 0) position += constant.lane_length;
					if (map_information.update_position.existence[car.lane_number[ID] + lanechange][position]) {
						int rear_side = map_information.update_position.ID[car.lane_number[ID] + lanechange][position];
						//安全基準判断
						if (j >= canditate_velocity[rear_side] - canditate_velocity[ID]) lanechange_fg = true;
						break;
					}
					else j++;
				}
			}
		}
		if (lanechange_fg) {
			if (!lane_change_flag) lane_change_flag = true;
			still_room_for_movement[ID] = true;	//車線変更をしたから，より前方に移動できる可能性がある→true
			//自身が先頭車両だった場合，元居たレーンの先頭車両は不在になる
			if (lead_car_ID[car.lane_number[ID]].ID == ID) lead_car_ID[car.lane_number[ID]].existence = false;
			map_information.number_of_cars[car.lane_number[ID]]--;
			map_information.update_position.existence[car.lane_number[ID]][car.position.current[ID]] = false;
			_search_around(car.lane_number[ID], car.position.current[ID]);	//元居たレーンの前後関係を調べる
			car.lane_number[ID] += lanechange;
			map_information.number_of_cars[car.lane_number[ID]]++;
			map_information.update_position.existence[car.lane_number[ID]][car.position.current[ID]] = true;
			map_information.update_position.ID[car.lane_number[ID]][car.position.current[ID]] = ID;
			_search_around(car.lane_number[ID], car.position.current[ID]);	//車線変更後の前後関係を調べる
		}
	}
	return lane_change_flag;
}

void Lane_Change::_search_around(int lane_number, int position) {
	//与えられたレーン番号と位置周りの車両の前後関係を調べる
	int fID, rID;
	int fposition, rposition;
	bool ffg, rfg;
	ffg = rfg = true;
	fposition = position + 1;
	rposition = position - 1;
	//以下で前方車両と後方車両を探す
	while (true) {
		if (fposition >= constant.lane_length) fposition -= constant.lane_length;
		if (rposition < 0) rposition += constant.lane_length;
		if (fposition == position) ffg = false;
		if (rposition == position) rfg = false;
		if (ffg) if (map_information.update_position.existence[lane_number][fposition]) ffg = false;
		if (rfg) if (map_information.update_position.existence[lane_number][rposition]) rfg = false;
		if (!ffg and !rfg) break;
		if (ffg) fposition++;
		if (rfg) rposition--;
	}
	fID = map_information.update_position.ID[lane_number][fposition];
	rID = map_information.update_position.ID[lane_number][rposition];
	int distance;
	int lID;	//lead ID
	if (map_information.update_position.existence[lane_number][position]) {
		//この場合，車線変更後のID周りの前後車両の情報を取得する
		int ID = map_information.update_position.ID[lane_number][position];
		int fdistance = fposition - position;
		if (fdistance <= 0) fdistance += constant.lane_length;
		car.around.rear.current.ID[ID] = rID;
		car.around.front.current.ID[rID] = ID;
		car.around.rear.current.distance[ID] = car.around.front.current.distance[rID] = fdistance;
		int rdistance = position - rposition;
		if (rdistance <= 0) rdistance += constant.lane_length;
		car.around.rear.current.ID[fID] = ID;
		car.around.front.current.ID[ID] = fID;
		car.around.rear.current.distance[fID] = car.around.front.current.distance[ID] = rdistance;
		if (fdistance >= rdistance) {
			distance = fdistance;
			lID = ID;
		}
		else {
			distance = rdistance;
			lID = rID;
		}
	}
	else {
		//この場合，車線変更前の自身がいなくなった後の前後車両の情報を取得する
		distance = fposition - rposition;
		if (distance <= 0) distance += constant.lane_length;
		car.around.rear.current.ID[fID] = rID;
		car.around.front.current.ID[rID] = fID;
		car.around.rear.current.distance[fID] = car.around.front.current.distance[rID] = distance;
		lID = rID;
	}
	if (!lead_car_ID[lane_number].existence) {
		lead_car_ID[lane_number].existence = true;
		lead_car_ID[lane_number].ID = lID;
		lead_car_ID[lane_number].maximum_gap = distance;
	}
	else {
		if (distance >= lead_car_ID[lane_number].maximum_gap) {
			lead_car_ID[lane_number].ID = lID;
			lead_car_ID[lane_number].maximum_gap = distance;
		}
	}
}

#endif // !BASIC_TRAFFIC_LANE_CHANGE_H
