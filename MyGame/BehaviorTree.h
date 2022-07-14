#ifndef BEHAVIOR_TREE_H_
#define BEHAVIOR_TREE_H_

#include <string>

class EnemyActionBase;
class ExecJudgmentBase;
class NodeBase;
class BehaviorDatas;
class Enemy;

// ビヘイビアツリー
class BehaviorTree
{
public:
	// 選択ルール
	enum SELECT_RULE
	{
		NON,
		PRIORITY,			// 優先順位
		SEQUENCE,			// シーケンス
		SEQUENTIAL_LOOPING,	// シーケンシャルルーピング
		RANDOM,				// ランダム
		ON_OFF,				// オン・オフ
	};

public:
	BehaviorTree() :
		m_Root(NULL)
	{
	}

	// 実行ノードを推論する
	NodeBase* Inference(Enemy* enemy, BehaviorDatas* data);

	// シーケンスノードから推論開始
	NodeBase* SequenceBack(NodeBase* sequence_node, Enemy* enemy, BehaviorDatas* data);

	// ノード追加
	void AddNode(std::string search_name, std::string entry_name, int priority, SELECT_RULE select_rule, ExecJudgmentBase* judgment, EnemyActionBase* action);

	// ツリー構造を表示
	void PrintTree();

	// 実行
	NodeBase* Run(Enemy* enemy, NodeBase* action_node, BehaviorDatas* data);
private:
	// ルートノード
	NodeBase* m_Root;
};

#endif
