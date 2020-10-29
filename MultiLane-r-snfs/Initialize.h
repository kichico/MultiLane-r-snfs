#ifndef BASIC_TRAFFIC_INITIALIZE_H
#define BASIC_TRAFFIC_INITIALIZE_H
#include "Class.h"

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


#endif // !BASIC_TRAFFIC_INITIALIZE_H

