#include "pch.h"
#include "AnimationManager.h"
#include "AnimationSet.h"


// AnimationManager�� �⺻ ������
AnimationManager::AnimationManager()
{
}

// AnimationSet�� ������ �Լ� const ����
const AnimationSet& AnimationManager::GetAnimationSet(const std::string& object_name) const
{
    auto it = object_animations.find(object_name);
    if (it != object_animations.end()) {
        return it->second;
    }

    static const AnimationSet empty_animation_set;
    return empty_animation_set;
}

// AnimationSet�� ������ �Լ� ���� ���� ����
AnimationSet& AnimationManager::GetAnimationSet(const std::string& object_name)
{
    auto it = object_animations.find(object_name);
    if (it != object_animations.end()) {
        return it->second;
    }

    static AnimationSet empty_animation_set;
    return empty_animation_set;
}
