#include "pch.h"
#include "AnimationManager.h"
#include "AnimationSet.h"


// AnimationManager의 기본 생성자
AnimationManager::AnimationManager()
{
}

// AnimationSet을 얻어오는 함수 const 버전
const AnimationSet& AnimationManager::GetAnimationSet(const std::string& object_name) const
{
    auto it = object_animations.find(object_name);
    if (it != object_animations.end()) {
        return it->second;
    }

    static const AnimationSet empty_animation_set;
    return empty_animation_set;
}

// AnimationSet을 얻어오는 함수 수정 가능 버전
AnimationSet& AnimationManager::GetAnimationSet(const std::string& object_name)
{
    auto it = object_animations.find(object_name);
    if (it != object_animations.end()) {
        return it->second;
    }

    static AnimationSet empty_animation_set;
    return empty_animation_set;
}

void AnimationManager::AddAnimationObject(const std::string& object_name)
{
    auto it = object_animations.find(object_name);
    if (it != object_animations.end()) {
        return;
    }

    object_animations.emplace(object_name, AnimationSet());
}
