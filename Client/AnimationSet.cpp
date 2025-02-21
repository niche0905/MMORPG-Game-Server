#include "pch.h"
#include "AnimationSet.h"
#include "Animation.h"


// AnimationSet �⺻ ������ (���� �ƹ��͵� �� �� ����)
AnimationSet::AnimationSet()
{

}

// Animation�� �������� �Լ� const ����
const Animation& AnimationSet::GetAnimation(const std::string& animation_name) const
{
    auto it = animation_set.find(animation_name);
    if (it != animation_set.end()) {
        return it->second;
    }

    static const Animation empty_animation;
    return empty_animation;
}

// Animation�� �������� �Լ� ���� ������ ����
Animation& AnimationSet::GetAnimation(const std::string& animation_name)
{
    auto it = animation_set.find(animation_name);
    if (it != animation_set.end()) {
        return it->second;
    }

    static Animation empty_animation;
    return empty_animation;
}
