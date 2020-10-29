#ifndef BASIC_TRAFFIC_INITIALIZE_H
#define BASIC_TRAFFIC_INITIALIZE_H
#include "Class.h"

class Initialize {
	/*
		基底クラス
		このクラスを継承して，モデルを組んでいく
	*/
public:
	struct Lead_Car_ID {
		//各レーンの先頭車両情報を格納する
		//先頭車両の定義は，最も前方車両との距離がある車両である
		bool existence;
		int ID;
		int maximum_gap;
	};

	Car car;
	Information information;
	Map_Information map_information;
	Constant_Information constant;	//計算条件の定数や変数
	int N;		//車両数
	double C;	//C戦略車両数
	double D;	//D戦略車両数
	std::vector<Lead_Car_ID> lead_car_ID;	//lead_car_ID[レーン番号] = そのレーンの先頭車両のID
	void initialize() { _initialize(); }	//モデルの初期化

	//以下は，理論と実装に間違いがなければ必要ないパート
	void CHECK(std::vector<std::vector<int>>& ID, std::vector<std::vector<bool>>& existence) { _CHECK(ID, existence); }
private:
	void _initialize();	//モデルの初期化の実装
	//↓以下に実装のUnitになる関数の列挙
	void _initialize_cars_strategy();
	void _initialize_cars_position();
	void _initial_search();

	void _CHECK(std::vector<std::vector<int>>& map, std::vector<std::vector<bool>>& existence);
};


#endif // !BASIC_TRAFFIC_INITIALIZE_H

