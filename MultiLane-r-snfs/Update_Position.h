#ifndef BASIC_TRAFFIC_UPDATE_POSITION_H
#define BASIC_TRAFFIC_UPDATE_POSITION_H
#include "Lane_Change.h"

class Update_Position : public Lane_Change {
	/*
		三つ目の継承クラス
		このクラスは，各車両の位置を速度に従って更新する
		この際，各レーンの流量が最大になるように位置を更新する
	*/
public:
	void update_position() { _update_position(); }	//車両位置を更新する
private:
	std::vector<Lead_Car_ID> LCI;		//LCI[レーン番号]次ステップの先頭車両候補
	void _update_position();				//車両位置更新の実装
	//↓以下に実装のUnitになる関数の列挙
	bool _update_position_front_to_back(int lane_number);	//先頭車両から順に位置を更新する
	void _move_forward_car(int lane_number, int ID);		//上記関数の詳細な中身

	//↓以下，エラーチェック．理論と実装に間違いがなければ必要ないパート
	void _check_clash(int ID, int lane_number, int next_position, bool clash);
	void _check_velocity(int ID, int _v, std::vector<int>* check_position, int next_position, bool clash);
};

#endif // !BASIC_TRAFFIC_UPDATE_POSITION_H
