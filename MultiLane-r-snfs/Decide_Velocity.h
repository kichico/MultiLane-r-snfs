#ifndef BASIC_TRAFFIC_DECIDE_VELOCITY_H
#define BASIC_TRAFFIC_DECIDE_VELOCITY_H
#include "Initialize.h"

class Decide_Velocity : public Initialize {
	/*
		一つ目の継承クラス
		このクラスは，Rule1からRule4に沿って，次ステップの候補速度を求める
	*/
public:
	void apply_rules_1_to_4() { _apply_rules_1_to_4(); }	//ルール1から4を各車両に適用
private:
	void _apply_rules_1_to_4();	//ルール適用の実装
	//↓以下に実装のUnitになる関数の列挙
	void _rule1(int ID);
	void _rule2(int ID);
	void _rule3(int ID);
	void _rule4(int ID);
};
#endif // !BASIC_TRAFFIC_DECIDE?VELOCITY_H
