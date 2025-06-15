#include "pch.h"
#include "Astar.h"
#include "Node.h"

std::vector<Position> Astar::FindPath(Position start, Position goal, std::function<bool(const Position&)> IsBlocked)
{
    std::priority_queue<Node*, std::vector<Node*>, std::function<bool(Node*, Node*)>> openList(
        [](Node* a, Node* b) { return a->fCost() > b->fCost(); }
    );

    std::unordered_map<int32, Node*> allNodes; // key = (y << 16) | x

    auto key = [](const Position& pos) {
        return (static_cast<int32>(pos.y) << 16) | pos.x;
        };

    Node* startNode = new Node{ start, 0, Heuristic(start, goal), nullptr };
    openList.push(startNode);
    allNodes[key(start)] = startNode;

    std::unordered_set<int32> closedSet;

    const Position dirs[] = { {1,0}, {-1,0}, {0,1}, {0,-1} };

    while (!openList.empty()) {
        Node* current = openList.top();
        openList.pop();

        if (current->pos == goal) {
            // 목표 도달 (경로 찾음)
            std::vector<Position> path;
            while (current) {
                path.push_back(current->pos);
                current = current->parent;
            }
            std::reverse(path.begin(), path.end());
            for (auto& [_, node] : allNodes) delete node;
            return path;
        }

        closedSet.insert(key(current->pos));

        for (const Position& dir : dirs) {  // 상하좌우 탐색
            Position next = current->pos + dir;

            if (IsBlocked(next) || closedSet.count(key(next))) continue;

            int gCost = current->g_cost + 1;
            int hCost = Heuristic(next, goal);

            int32 k = key(next);
            Node* neighbor;

            if (allNodes.count(k)) {
                neighbor = allNodes[k];
                if (gCost < neighbor->g_cost) {
                    neighbor->g_cost = gCost;
                    neighbor->parent = current;
                    openList.push(neighbor);
                }
            }
            else {
                neighbor = new Node{ next, gCost, hCost, current };
                allNodes[k] = neighbor;
                openList.push(neighbor);
            }
        }
    }

    for (auto& [_, node] : allNodes) delete node;
    return {}; // 경로 없음
}

int32 Astar::Heuristic(const Position& a, const Position& b)
{
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}
