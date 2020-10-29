#ifndef BASIC_TRAFFIC_CLASS_H
#define BASIC_TRAFFIC_CLASS_H
#include <iostream>
#include <vector>
#include "random.h"
#include "String.h"

#pragma region �v�Z����
struct Constant_Information {
#pragma region ���ʒ萔
	const int ensemble = 50;			//�A���T���u����
	const int run_up_steps = 1800;		//�������Ԃ̃X�e�b�v��
	const int mesurement_steps = 300;	//�v�����Ԃ̃X�e�b�v��
	const int lane_length = 300;		//���[���̒���
	const int number_of_lanes = 1;		//���[����
	const int Vmax = 5;					//�ő呬�x
#pragma endregion
#pragma region Resived-S-NFS���f���p�萔
	const int G = 15;					//���x�v�Z�p�ԊԊ
	const int S = 2;					//���ʂ��ԗ��͉���O��
	const double r = 0.99;				//���ʂ��m��
	const double q = 0.99;				//�X���[�X�^�[�g�m��
#pragma endregion
#pragma region �u���[�L�m��
	const double p1 = 0.99;
	const double p2 = 0.02;
	const double p3 = 0.01;
	const double p4 = 0.001;
#pragma endregion
};
#pragma endregion

#pragma region �E�C���J�[(signal)
struct Signal {
	static const int left = 0;
	static const int right = 1;
};
#pragma endregion

#pragma region �헪(Strategy)
struct Strategy {
	static const int C = 0;	//���[���`�F���W���Ȃ�
	static const int D = 1;	//���[���`�F���W����
};
#pragma endregion

//��{�N���X��3����CCar�CInformation�CMap_Information�ł���
//����3�̃N���X�́C���N���XInitialize�Ŏ��������

class Car {
/*
	��{�N���X�̈��
	�J�[�i���o�[ID���œ_�ɒu���āC���̎ԗ��̈ʒu�C���x�C���[���ԍ��ȂǁC�������i�[���Ă���
*/
	struct Old_and_New_Vector {
		std::vector<int> previous;	//����
		std::vector<int> current;	//1�X�e�b�v�O
	};
#pragma region Around_Cars_Information ���Ԏ���̎Ԃ̏��(ID�Ƌ���)
	//��{�\����
	struct ID_and_Distance {
		std::vector<int> ID;
		std::vector<int> distance;
	};
	//���p��
	struct Old_and_New_Around_Cars {
		ID_and_Distance previous;
		ID_and_Distance current;
	};
	//���p��
	struct Around_Cars_Information {
		Old_and_New_Around_Cars front;	//�O��
		Old_and_New_Around_Cars rear;	//���
	};
#pragma endregion
public:
	std::vector<int> strategy;		//strategy[ID] = �uID�v�̐헪
	Old_and_New_Vector position;	//position.previous[ID] = �uID�v��1�X�e�b�v�O�̈ʒu�Cposition.current[ID] = �uID�v�̌��݂̈ʒu
	std::vector<int> lane_number;	//lane_number[ID] = �uID�v�̃��[���ԍ�
	std::vector<int> velocity;		//velocity[ID] = �uID�v�̑��x
	std::vector<int> S;				//S[ID] = �uID�v�̌��ʂ��䐔�C�_����Si�ɂ�����
	Around_Cars_Information around;	//��jaround.front.current.ID[ID] = �uID�v�̌��݂̑O���ԗ���ID
	void initialize_car(int number_of_cars) { _initialize_car(number_of_cars); }	//Car�֌W�̏���S�ď���������֐�
	void update_previous_information() { _update_previous_information(); }			//Car�֌W�̏����܂Ƃ߂ăA�b�v�f�[�g����
private:
	void _initialize_car(int number_of_cars) {
		//initialize_car�̎���
		strategy = std::vector<int>(number_of_cars, Strategy::D);
		position.current = position.previous = std::vector<int>(number_of_cars);
		lane_number = std::vector<int>(number_of_cars);
		velocity = std::vector<int>(number_of_cars, 0);
		S = std::vector<int>(number_of_cars);
		around.front.current.ID = around.front.previous.ID = around.rear.current.ID = around.rear.previous.ID = std::vector<int>(number_of_cars);
		around.front.current.distance = around.front.previous.distance = around.rear.current.distance = around.rear.previous.distance = std::vector<int>(number_of_cars);
	}

	void _update_previous_information() {
		//update_previous_information�̎���
		//���݂̏���1�X�e�b�v�O�̏��ɍX�V����
		position.previous = position.current;
		around.front.previous.ID = around.front.current.ID;
		around.rear.previous.ID = around.rear.current.ID;
		around.front.previous.distance = around.front.current.distance;
		around.rear.previous.distance = around.rear.current.distance;
		//���̂悤�ɑ�������Ŕz����R�s�[����������Cposition��arround�̔z�����ԉ��Ɏ����Ă��Ă���
		//��������ԏ�ɔz�����������ƁCfor�����K�v�ƂȂ�C�v�Z���x���x���Ȃ�D�������C�R�[�h���͓̂ǂ݂₷���Ȃ�
	}
};

class Information {
/*	
	��{�N���X�̈��
	��{�I�ɓ��v�ʂƑS�̂̏����i�[����
*/
public:
	std::vector<int> v0;				//v0[ID] = �uID�v�̊e���[����K������O�̑��x
	std::vector<int> try_turn;			//�Ԑ��ύX�̃C���Z���e�B�u�𖞂������ԗ���ID���i�[
	std::vector<int> try_turn_signal;	//�Ԑ��ύX�̃C���Z���e�B�u�𖞂������ԗ���ID�̃E�C���J�[�i�E�������j���i�[�C��L�̗v�f�ԍ��ƑΉ����Ă���
	//�ȍ~�C�K�v�ȓ��v�ʂ��L�q
	int sum_velocity;					//���X�e�b�v�̑S�ԗ��̍��v���x
	std::vector<int> lane_velocity;		//lane_velocity[���[���ԍ�] = ���̃��[���̎ԗ��́C���X�e�b�v�̍��v���x
	double q;							//���X�e�b�v�̗���
	int turn_left;						//�����[���Ɉړ������ԗ���
	int turn_right;						//�E���[���Ɉړ������ԗ���
	void initialize_information(int number_of_lanes) { _initialize_information(number_of_lanes); }	//inofrmation�֌W�̏����܂Ƃ߂ď���������
	int elapsedtime;
private:
	void _initialize_information(int number_of_lanes) {
		//initialize_information�̎���
		lane_velocity = std::vector<int>(number_of_lanes, 0);
		try_turn.clear();
		try_turn_signal.clear();
		turn_left = 0;
		turn_right = 0;
	}
};

class Map_Information {
/*
	��{�N���X�̈��
	���ꂼ���Map�i�ʒu�j���œ_�ɒu���čl����D
	Map��[���[���ԍ�][0�_����̑��Έʒu]�ō\�������
*/
	struct Old_and_New_Bool {
		std::vector<std::vector<bool>> current;
		std::vector<std::vector<bool>> previous;
	};
	struct Old_and_New_Int {
		std::vector<std::vector<int>> current;
		std::vector<std::vector<int>> previous;
	};
	struct Existence_and_ID {
		Old_and_New_Bool existence;	//���ڍ�
		//existence.current[���[���ԍ�][0�_����̑��Έʒu] = True or False�CTrue�Ȃ�C���̈ʒu�i�Z���j�Ɍ��ݎԗ�������
		//existence.current[���[���ԍ�][0�_����̑��Έʒu] = True or False�CTrue�Ȃ�C���̈ʒu�i�Z���j��1�X�e�b�v�O�Ɏԗ�������

		Old_and_New_Int ID;	//���ڍ�
		//ID.current[���[���ԍ�][0�_����̑��Έʒu] = ���ݑ��݂���ԗ���ID
		//ID.previous[���[���ԍ�][0�_����̑��Έʒu] = 1�X�e�b�v�O�ɑ��݂����ԗ���ID
	};
	struct Update_Position {
		//�ʒu�X�V�Ɋւ�����
		std::vector<std::vector<bool>> existence;	//��Ɠ���
		std::vector<std::vector<int>> ID;			//��Ɠ���
	};
public:
	Existence_and_ID map;				//��jmap.ID.current[���[���ԍ�][0�_����̑��Έʒu]
	Update_Position update_position;	//��jupdate_position.existence.current[���[���ԍ�][0�_����̑��Έʒu]
	std::vector<int> number_of_cars;	//number_of_cars[���[���ԍ�] = ���̃��[���̎ԗ���
	void initialize_Map_Information(int number_of_lane, int lane_length) { _initialize_Map_Information(number_of_lane, lane_length); }	//Map_Information�Ɋւ�������ꊇ������
private:
	void _initialize_Map_Information(int number_of_lane, int lane_length) {
		//initialize_Map_Information�̎���
		map.ID.current = map.ID.previous = std::vector<std::vector<int>>(number_of_lane, std::vector<int>(lane_length));
		map.existence.current = map.existence.previous = std::vector<std::vector<bool>>(number_of_lane, std::vector<bool>(lane_length, false));
		number_of_cars = std::vector<int>(number_of_lane, 0);
	}
};








#endif // !BASIC_TRAFFIC_CLASS_H

