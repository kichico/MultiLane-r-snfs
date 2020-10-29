#ifndef BASIC_TRAFFIC_CLASS_H
#define BASIC_TRAFFIC_CLASS_H
#include <iostream>
#include <vector>
#include "random.h"
#include "String.h"

#pragma region 計算条件
struct Constant_Information {
#pragma region 共通定数
	const int ensemble = 50;			//アンサンブル数
	const int run_up_steps = 1800;		//助走期間のステップ数
	const int mesurement_steps = 300;	//計測期間のステップ数
	const int lane_length = 300;		//レーンの長さ
	const int number_of_lanes = 1;		//レーン数
	const int Vmax = 5;					//最大速度
#pragma endregion
#pragma region Resived-S-NFSモデル用定数
	const int G = 15;					//速度計算用車間基準
	const int S = 2;					//見通す車両は何台前か
	const double r = 0.99;				//見通し確率
	const double q = 0.99;				//スロースタート確率
#pragma endregion
#pragma region ブレーキ確率
	const double p1 = 0.99;
	const double p2 = 0.02;
	const double p3 = 0.01;
	const double p4 = 0.001;
#pragma endregion
};
#pragma endregion

#pragma region ウインカー(signal)
struct Signal {
	static const int left = 0;
	static const int right = 1;
};
#pragma endregion

#pragma region 戦略(Strategy)
struct Strategy {
	static const int C = 0;	//レーンチェンジしない
	static const int D = 1;	//レーンチェンジする
};
#pragma endregion

//基本クラスは3つあり，Car，Information，Map_Informationである
//この3つのクラスは，基底クラスInitializeで実装される

class Car {
/*
	基本クラスの一つ
	カーナンバーIDを焦点に置いて，その車両の位置，速度，レーン番号など，諸情報を格納している
*/
	struct Old_and_New_Vector {
		std::vector<int> previous;	//現在
		std::vector<int> current;	//1ステップ前
	};
#pragma region Around_Cars_Information 自車周りの車の情報(IDと距離)
	//基本構造体
	struct ID_and_Distance {
		std::vector<int> ID;
		std::vector<int> distance;
	};
	//↓継承
	struct Old_and_New_Around_Cars {
		ID_and_Distance previous;
		ID_and_Distance current;
	};
	//↓継承
	struct Around_Cars_Information {
		Old_and_New_Around_Cars front;	//前方
		Old_and_New_Around_Cars rear;	//後方
	};
#pragma endregion
public:
	std::vector<int> strategy;		//strategy[ID] = 「ID」の戦略
	Old_and_New_Vector position;	//position.previous[ID] = 「ID」の1ステップ前の位置，position.current[ID] = 「ID」の現在の位置
	std::vector<int> lane_number;	//lane_number[ID] = 「ID」のレーン番号
	std::vector<int> velocity;		//velocity[ID] = 「ID」の速度
	std::vector<int> S;				//S[ID] = 「ID」の見通し台数，論文のSiにあたる
	Around_Cars_Information around;	//例）around.front.current.ID[ID] = 「ID」の現在の前方車両のID
	void initialize_car(int number_of_cars) { _initialize_car(number_of_cars); }	//Car関係の情報を全て初期化する関数
	void update_previous_information() { _update_previous_information(); }			//Car関係の情報をまとめてアップデートする
private:
	void _initialize_car(int number_of_cars) {
		//initialize_carの実装
		strategy = std::vector<int>(number_of_cars, Strategy::D);
		position.current = position.previous = std::vector<int>(number_of_cars);
		lane_number = std::vector<int>(number_of_cars);
		velocity = std::vector<int>(number_of_cars, 0);
		S = std::vector<int>(number_of_cars);
		around.front.current.ID = around.front.previous.ID = around.rear.current.ID = around.rear.previous.ID = std::vector<int>(number_of_cars);
		around.front.current.distance = around.front.previous.distance = around.rear.current.distance = around.rear.previous.distance = std::vector<int>(number_of_cars);
	}

	void _update_previous_information() {
		//update_previous_informationの実装
		//現在の情報を1ステップ前の情報に更新する
		position.previous = position.current;
		around.front.previous.ID = around.front.current.ID;
		around.rear.previous.ID = around.rear.current.ID;
		around.front.previous.distance = around.front.current.distance;
		around.rear.previous.distance = around.rear.current.distance;
		//このように代入だけで配列をコピーしたいから，positionとarroundの配列を一番下に持ってきている
		//もしも一番上に配列を実装すると，for文が必要となり，計算速度が遅くなる．ただし，コード自体は読みやすくなる
	}
};

class Information {
/*	
	基本クラスの一つ
	基本的に統計量と全体の情報を格納する
*/
public:
	std::vector<int> v0;				//v0[ID] = 「ID」の各ルールを適応する前の速度
	std::vector<int> try_turn;			//車線変更のインセンティブを満たした車両のIDを格納
	std::vector<int> try_turn_signal;	//車線変更のインセンティブを満たした車両のIDのウインカー（右か左か）を格納，上記の要素番号と対応している
	//以降，必要な統計量を記述
	int sum_velocity;					//現ステップの全車両の合計速度
	std::vector<int> lane_velocity;		//lane_velocity[レーン番号] = そのレーンの車両の，現ステップの合計速度
	double q;							//現ステップの流量
	int turn_left;						//左レーンに移動した車両数
	int turn_right;						//右レーンに移動した車両数
	void initialize_information(int number_of_lanes) { _initialize_information(number_of_lanes); }	//inofrmation関係の情報をまとめて初期化する
	int elapsedtime;
private:
	void _initialize_information(int number_of_lanes) {
		//initialize_informationの実装
		lane_velocity = std::vector<int>(number_of_lanes, 0);
		try_turn.clear();
		try_turn_signal.clear();
		turn_left = 0;
		turn_right = 0;
	}
};

class Map_Information {
/*
	基本クラスの一つ
	それぞれのMap（位置）を焦点に置いて考える．
	Mapは[レーン番号][0点からの相対位置]で構成される
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
		Old_and_New_Bool existence;	//↓詳細
		//existence.current[レーン番号][0点からの相対位置] = True or False，Trueなら，その位置（セル）に現在車両がいる
		//existence.current[レーン番号][0点からの相対位置] = True or False，Trueなら，その位置（セル）に1ステップ前に車両がいた

		Old_and_New_Int ID;	//↓詳細
		//ID.current[レーン番号][0点からの相対位置] = 現在存在する車両のID
		//ID.previous[レーン番号][0点からの相対位置] = 1ステップ前に存在した車両のID
	};
	struct Update_Position {
		//位置更新に関する情報
		std::vector<std::vector<bool>> existence;	//上と同じ
		std::vector<std::vector<int>> ID;			//上と同じ
	};
public:
	Existence_and_ID map;				//例）map.ID.current[レーン番号][0点からの相対位置]
	Update_Position update_position;	//例）update_position.existence.current[レーン番号][0点からの相対位置]
	std::vector<int> number_of_cars;	//number_of_cars[レーン番号] = そのレーンの車両数
	void initialize_Map_Information(int number_of_lane, int lane_length) { _initialize_Map_Information(number_of_lane, lane_length); }	//Map_Informationに関する情報を一括初期化
private:
	void _initialize_Map_Information(int number_of_lane, int lane_length) {
		//initialize_Map_Informationの実装
		map.ID.current = map.ID.previous = std::vector<std::vector<int>>(number_of_lane, std::vector<int>(lane_length));
		map.existence.current = map.existence.previous = std::vector<std::vector<bool>>(number_of_lane, std::vector<bool>(lane_length, false));
		number_of_cars = std::vector<int>(number_of_lane, 0);
	}
};








#endif // !BASIC_TRAFFIC_CLASS_H

