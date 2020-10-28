#ifndef BASIC_TRAFFIC_UPDATE_POSITION_H
#define BASIC_TRAFFIC_UPDATE_POSITION_H
#include "Lane_Change.h"

void Update_Position::_update_position() {
	//�S�ԗ��̑��x���ω����Ȃ��Ȃ�܂ŌJ��Ԃ�
	//�펞�C�O��֌W���X�V����
	//���X�e�b�v�ɔ����C�擪�ԗ���I�肷��
	information.sum_velocity = 0;
	LCI = std::vector<Lead_Car_ID>(constant.number_of_lanes);
	//����������v�Z�ł���
	for (int i = 0; i < constant.number_of_lanes; i++) if (lead_car_ID[i].existence) {
		int lead_car = lead_car_ID[i].ID;
		LCI[i].existence = false;
		//�擪�ԗ����ő呬�x�ňړ��o�����C�������̓��[���̑��x���ω����Ȃ��Ȃ�΁C���̃��[�v�𔲂���
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
	//���[�v�̑O�ɐ擪�ԗ��̑O���ԗ�(�Ō��)�̈ʒu��o�^���Ă���
	map_information.update_position.existence[lane_number][car.position.current[car.around.front.current.ID[ID]]] = true;
	//�擪�ԗ�����C�㑱�ԗ��֌������ă��[�v
	//�܂�C�擪�ԗ����珇�Ɍ㑱�ԗ��Ɍ������Ď��Ԃ̈ʒu���X�V���Ă���
	while (true) {
		//�܂��ړ��̗]�n������ԗ��̂ݍl����
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
			//�擪�ԗ��ɖ߂��Ă����̂ŏI��
			//�擪�ԗ��ƍŌ���ԗ��̋������X�V
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
	//���݂̈ʒu���g���Ď���position��������Z���Ă���
	//�ő�ړ��ʒu�ɂȂ��Ă��O���ԗ��ɒǂ����Ȃ���΁C�ő�ړ��ł���
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
	else still_room_for_movement[ID] = false;	//�ő�ړ���B��
	if (next_position >= constant.lane_length) next_position -= constant.lane_length;
	if (next_position < 0) next_position += constant.lane_length;
	_check_clash(ID, lane_number, next_position, clash);	//�N���b�V�����Ă��Ȃ����`�F�b�N
	int v = next_position - car.position.previous[ID];
	if (v < 0) v += constant.lane_length;
	canditate_velocity[ID] = v;
	_check_velocity(ID, _v, &check_position, next_position, clash);	//�X�s�[�h�����������Ȃ����`�F�b�N
	map_information.update_position.existence[lane_number][next_position] = true;
	map_information.update_position.ID[lane_number][next_position] = ID;
	car.position.current[ID] = next_position;
	//���ԂƑO���ԗ��̋������X�V
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
