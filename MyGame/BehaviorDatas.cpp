#include "BehaviorDatas.h"
#include "NodeBase.h"
#include "Enemy.h"

void BehaviorDatas::ResetNodeUsed(std::vector<NodeBase*>* reset_hierachy)
{
	for (auto itr = reset_hierachy->begin(); itr != reset_hierachy->end(); itr++)
	{
		m_UsedNodeMap[(*itr)->GetName()] = false;
	}
}

