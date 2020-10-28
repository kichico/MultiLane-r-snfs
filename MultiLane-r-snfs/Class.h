#ifndef BASIC_TRAFFIC_CLASS_H
#define BASIC_TRAFFIC_CLASS_H
#include <iostream>
#include <vector>
#include "random.h"
#include "String.h"

#pragma region �v�Z����
struct Constant_Information {
#pragma region ���ʒ萔
	const int ensemble = 5;			//�A���T���u����
	const int run_up_steps = 3000;		//�������Ԃ̃X�e�b�v��
	const int mesurement_steps = 100;	//�v�����Ԃ̃X�e�b�v��
	const int lane_length = 500;		//���[���̒���
	const int number_of_lanes = 2;		//���[����
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

class Initialize {
/*
	���N���X
	���̃N���X���p�����āC���f����g��ł���
*/
public:
	struct Lead_Car_ID {
		//�e���[���̐擪�ԗ������i�[����
		//�擪�ԗ��̒�`�́C�ł��O���ԗ��Ƃ̋���������ԗ��ł���
		bool existence;
		int ID;
		int maximum_gap;
	};

	Car car;
	Information information;
	Map_Information map_information;
	Constant_Information constant;	//�v�Z�����̒萔��ϐ�
	int N;		//�ԗ���
	double C;	//C�헪�ԗ���
	double D;	//D�헪�ԗ���
	std::vector<Lead_Car_ID> lead_car_ID;	//lead_car_ID[���[���ԍ�] = ���̃��[���̐擪�ԗ���ID
	void initialize() { _initialize(); }	//���f���̏�����

	//�ȉ��́C���_�Ǝ����ɊԈႢ���Ȃ���ΕK�v�Ȃ��p�[�g
	void CHECK(std::vector<std::vector<int>>& ID, std::vector<std::vector<bool>>& existence) { _CHECK(ID, existence); }
private:
	void _initialize();	//���f���̏������̎���
	//���ȉ��Ɏ�����Unit�ɂȂ�֐��̗�
	void _initialize_cars_strategy();
	void _initialize_cars_position();
	void _initial_search();

	void _CHECK(std::vector<std::vector<int>>& map, std::vector<std::vector<bool>>& existence);
};

class Decide_Velocity : public Initialize {
/*
	��ڂ̌p���N���X
	���̃N���X�́CRule1����Rule4�ɉ����āC���X�e�b�v�̌�⑬�x�����߂�
*/
public:
	void apply_rules_1_to_4() { _apply_rules_1_to_4(); }	//���[��1����4���e�ԗ��ɓK�p
private:
	void _apply_rules_1_to_4();	//���[���K�p�̎���
	//���ȉ��Ɏ�����Unit�ɂȂ�֐��̗�
	void _rule1(int ID);
	void _rule2(int ID);
	void _rule3(int ID);
	void _rule4(int ID);
};

class Lane_Change : public Decide_Velocity {
/*
	��ڂ̌p���N���X
	���̃N���X�́CRule4�K�p���̑��x�ɂ����āC�E�C���J�[���o������p�[�g�C
	���ۂɎԐ��ύX���s���p�[�g�̓�̃p�[�g�ō\������Ă���C
	�Ԑ��ύX�S�ʂ��i��
*/
#pragma region ����̎ԗ����
	struct Side {
		//�œ_�ԗ�ID����̏����i�[����
		int ID;
		int distance;
	};
	struct Side_Information {
		//��L�̍\���̂�O���ƌ���Ŏ�������
		Side front_side;
		Side rear_side;
	};
#pragma endregion
	struct Winker {
		bool left;
		bool right;
	};

	struct Canditate {
		int ID;
		Side_Information side_information;
		Winker winker;
	};

public:
	void try_lane_change() { _try_lane_change(); }	//�E�C���J�[���o������
	bool lane_change() { return _lane_change(); }		//���ۂɎԐ��ύX���s��
	std::vector<bool> still_room_for_movement;	//�܂�������ő�ړ������ɑ΂��C��Ԃ����邩�ǂ���
	std::vector<int> canditate_velocity;	//�ŏI�I�ȑ��x�̌��l�i�Ⴆ�΁C�Փ˂�������邽�߂Ɍ�������ƁC���̎ԗ��̑��x��v4���͒x���Ȃ�j
private:
	std::vector<Canditate> canditate_lane_change;	//�Ԑ��ύX���s���Ԃ̃��X�g
	void _try_lane_change(); //���ۂɎԐ��ύX���s���p�[�g�̎���
	//���ȉ��Ɏ�����Unit�ɂȂ�֐��̗�
/*
	//����							//����
	//1. �C���Z���e�B�u�`�F�b�N		//�C���Z���e�B�u�`�F�b�N
	//2. �E�C���J�[���o������		//�E�C���J�[���o������
									//�Ԑ��ύX�Ȃ��ň�U�ʒu�X�V
	//3. ���S�m�F					
	//4. �Ԑ��ύX					//���S�m�F�C�Ԑ��ύX
*/
	int _decide_top_car();		//�Ԑ��ύX���s���擪�ԗ���S���[������1��I��
	void _create_canditate_of_lane_change(int leader);	//�Ԑ��ύX���s��D�헪�ԗ���擪�ԗ����珇�ɔ����o���C���X�g�����
	void _check_insentive(int canditate_num, int signal);	//�C���Z���e�B�u�`�F�b�N
	void _get_side_information(int canditate_num, int signal);	//�ԗ��ԍ�ID����̏����擾����
	void _decide_which_signal(int canditate_num);	//���E�ǂ���̃E�C���J�[���o�������肷��
	bool _lane_change();	//���S�m�F���s������C�Ԑ��ύX���s��
	void _search_around(int lane_number, int position);
};

class Update_Position : public Lane_Change {
/*
	�O�ڂ̌p���N���X
	���̃N���X�́C�e�ԗ��̈ʒu�𑬓x�ɏ]���čX�V����
	���̍ہC�e���[���̗��ʂ��ő�ɂȂ�悤�Ɉʒu���X�V����
*/
public:
	void update_position() { _update_position(); }	//�ԗ��ʒu���X�V����
private:
	std::vector<Lead_Car_ID> LCI;		//LCI[���[���ԍ�]���X�e�b�v�̐擪�ԗ����
	void _update_position();				//�ԗ��ʒu�X�V�̎���
	//���ȉ��Ɏ�����Unit�ɂȂ�֐��̗�
	bool _update_position_front_to_back(int lane_number);	//�擪�ԗ����珇�Ɉʒu���X�V����
	void _move_forward_car(int lane_number, int ID);		//��L�֐��̏ڍׂȒ��g

	//���ȉ��C�G���[�`�F�b�N�D���_�Ǝ����ɊԈႢ���Ȃ���ΕK�v�Ȃ��p�[�g
	void _check_clash(int ID, int lane_number, int next_position, bool clash);
	void _check_velocity(int ID, int _v, std::vector<int>* check_position, int next_position, bool clash);
};

class Basic_Traffic : public Update_Position {
/*
	���f���S�̂��i��N���X
	�S�ẴN���X���p�����Ă���
*/
public:
	double q;	//����
	double rho;	//���x
	void calculation(int number_of_cars, double C, double D) { _calculation(number_of_cars, C, D); }	//���f���̌v�Z
private:
	void _calculation(int number_of_cars, double C, double D);	//����
	void _proceed();
};

#endif // !BASIC_TRAFFIC_CLASS_H

