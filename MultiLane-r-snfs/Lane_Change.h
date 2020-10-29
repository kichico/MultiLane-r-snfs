#ifndef BASIC_TRAFFIC_LANE_CHANGE_H
#define BASIC_TRAFFIC_LANE_CHANGE_H
#include "Decide_Velocity.h"

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

#endif // !BASIC_TRAFFIC_LANE_CHANGE_H
