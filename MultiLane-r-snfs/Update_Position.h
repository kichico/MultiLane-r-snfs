#ifndef BASIC_TRAFFIC_UPDATE_POSITION_H
#define BASIC_TRAFFIC_UPDATE_POSITION_H
#include "Lane_Change.h"

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

#endif // !BASIC_TRAFFIC_UPDATE_POSITION_H
