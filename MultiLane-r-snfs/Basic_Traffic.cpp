#include "Basic_Traffic.h"

void Basic_Traffic::master_process() {

}

void Basic_Traffic::_proceed() {
	/*����
	���[��1����4��K��
	�Ԑ��ύX�̈ӎv��₤
	��U�ʒu�X�V
	�Ԑ��ύX
	������x�ʒu�X�V
	�f�[�^�A�b�v�f�[�g
	*/
	apply_rules_1_to_4();
	try_lane_change();
	//�ʒu���X�V�̑O�ɁC���ݏ���ۑ�
	car.update_previous_information();	//�ꊇ���X�V (�ʒu���C���Ԏ�����)
	map_information.map.existence.previous = map_information.map.existence.current;
	map_information.update_position.existence = std::vector<std::vector<bool>>(constant.number_of_lanes, std::vector<bool>(constant.lane_length, false));
	map_information.map.ID.previous = map_information.update_position.ID = map_information.map.ID.current;
	canditate_velocity = std::vector<int>(N, 0);
	still_room_for_movement = std::vector<bool>(N, true);
	//�ʒu���X�V
	update_position();	//���ڈʒu�X�V
	bool lane_change_flag = lane_change();		//�����������Ƃ���ŎԐ��ύX
	if (lane_change_flag) update_position();	//�Ԑ��ύX�����ԂƁC���̎��ӂ̎Ԃ̑��x��(����)	�Ԑ��ύX���Ȃ���΁C����update�͍s��Ȃ�
	//�S���X�V
	car.velocity = canditate_velocity;
	map_information.map.existence.current = map_information.update_position.existence;
	map_information.map.ID.current = map_information.update_position.ID;
	information.q = double(N) / (double(constant.number_of_lanes) * double(constant.lane_length)) * double(information.sum_velocity) / double(N);
	//CHECK(map_information.map.ID.current, map_information.map.existence.current);
	//getchar();
}

void Basic_Traffic::_calculation_neglect_C_and_D() {
	for (int n = 0; n < constant.ensemble; n++) {
		double _q = 0;
		initialize();
		for (int i = 0; i < constant.run_up_steps; i++) {
			_proceed();
		}
		for (int i = 0; i < constant.mesurement_steps; i++) {
			_proceed();
			_q += information.q;
		}
		std::cout<<
		//std::cout << "n: " << n + 1 << " car: " << number_of_cars << " C: " << C << " D: " << D << " q: " << _q / double(constant.mesurement_steps);
		//std::cout << " left: " << double(information.turn_left) / double(10000 + constant.mesurement_steps) << " right: " << double(information.turn_right) / double(10000 + constant.mesurement_steps) << std::endl;
		//q += _q / double(constant.mesurement_steps);
	}
	//q /= double(constant.ensemble);
}