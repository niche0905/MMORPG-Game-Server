#include "pch.h"
#include "PM_RunState.h"
#include "PeaceMonster.h"


PM_RunState& PM_RunState::Instance()
{
	static PM_RunState instance;
	return instance;
}

void PM_RunState::Enter(Bot* bot)
{
    PeaceMonster* PM_bot = static_cast<PeaceMonster*>(bot);

    if (!PM_bot->GetTarget()) return;

    Position dir = PM_bot->GetPosition() - PM_bot->GetTarget()->GetPosition();
    int dx = (dir.x != 0) ? (dir.x / std::abs(dir.x)) : 0;
    int dy = (dir.y != 0) ? (dir.y / std::abs(dir.y)) : 0;

    const int DIST = 6;
    std::vector<Position> candidates;
    for (int dist = DIST; dist >= 1; -dist) {
        Position candidate = bot->GetPosition() + Position{ dx * dist, dy * dist };
        if (!server.IsBlock(candidate))
            candidates.push_back(candidate);
    }

    for (const Position& goal : candidates) {
        auto path = Astar::FindPath(bot->GetPosition(), goal, [](const Position& pos) { return server.IsBlock(pos); });

        if (not path.empty()) {
            bot->SetPath(std::move(path));
            return; // 첫 번째 성공한 후보로 도망
        }
    }
}

void PM_RunState::Execute(Bot* bot)
{
    bot->DoPathFollow();
}

void PM_RunState::Exit(Bot* bot)
{

}
