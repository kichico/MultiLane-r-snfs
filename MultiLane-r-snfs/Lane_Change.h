#ifndef BASIC_TRAFFIC_LANE_CHANGE_H
#define BASIC_TRAFFIC_LANE_CHANGE_H
#include "Decide_Velocity.h"

class Lane_Change : public Decide_Velocity {
	/*
		二つ目の継承クラス
		このクラスは，Rule4適用時の速度において，ウインカーを出させるパート，
		実際に車線変更を行うパートの二つのパートで構成されており，
		車線変更全般を司る
	*/
#pragma region 周りの車両情報
	struct Side {
		//焦点車両ID周りの情報を格納する
		int ID;
		int distance;
	};
	struct Side_Information {
		//上記の構造体を前方と後方で実装する
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
	void try_lane_change() { _try_lane_change(); }	//ウインカーを出させる
	bool lane_change() { return _lane_change(); }		//実際に車線変更を行う
	std::vector<bool> still_room_for_movement;	//まだ動ける最大移動距離に対し，空間があるかどうか
	std::vector<int> canditate_velocity;	//最終的な速度の候補値（例えば，衝突を回避するために減速すると，その車両の速度はv4よりは遅くなる）
private:
	std::vector<Canditate> canditate_lane_change;	//車線変更を行う車のリスト
	void _try_lane_change(); //実際に車線変更を行うパートの実装
	//↓以下に実装のUnitになる関数の列挙
/*
	//流れ							//実装
	//1. インセンティブチェック		//インセンティブチェック
	//2. ウインカーを出させる		//ウインカーを出させる
									//車線変更なしで一旦位置更新
	//3. 安全確認
	//4. 車線変更					//安全確認，車線変更
*/
	int _decide_top_car();		//車線変更を行う先頭車両を全レーンから1台選ぶ
	void _create_canditate_of_lane_change(int leader);	//車線変更を行うD戦略車両を先頭車両から順に抜き出し，リストを作る
	void _check_insentive(int canditate_num, int signal);	//インセンティブチェック
	void _get_side_information(int canditate_num, int signal);	//車両番号ID周りの情報を取得する
	void _decide_which_signal(int canditate_num);	//左右どちらのウインカーを出すか決定する
	bool _lane_change();	//安全確認を行った後，車線変更を行う
	void _search_around(int lane_number, int position);
};

#endif // !BASIC_TRAFFIC_LANE_CHANGE_H
