#ifndef BASIC_TRAFFIC_DECIDE_VELOCITY_H
#define BASIC_TRAFFIC_DECIDE_VELOCITY_H
#include "Initialize.h"

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
#endif // !BASIC_TRAFFIC_DECIDE?VELOCITY_H
