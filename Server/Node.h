#pragma once
#include "pch.h"


struct Node
{
    Position pos;
    int32 g_cost; // 시작점에서 현재 노드까지의 실제 거리
    int32 h_cost; // 현재 노드에서 목표 지점까지의 휴리스틱 추정치
    int32 fCost() const { return g_cost + h_cost; }

    Node* parent;

    bool operator>(const Node& other) const {
        return fCost() > other.fCost();
    }
};
