#include "pch.h"
#include "AnimationSet.h"
#include "Animation.h"


// AnimationSet 기본 생성자 (아직 아무것도 할 게 없음)
AnimationSet::AnimationSet()
{

}

// Animation을 가져오는 함수 const 버전
const Animation& AnimationSet::GetAnimation(const std::string& animation_name) const
{
    auto it = animation_set.find(animation_name);
    if (it != animation_set.end()) {
        return it->second;
    }

    static const Animation empty_animation;
    return empty_animation;
}

// Animation을 가져오는 함수 수정 가능한 버전
Animation& AnimationSet::GetAnimation(const std::string& animation_name)
{
    auto it = animation_set.find(animation_name);
    if (it != animation_set.end()) {
        return it->second;
    }

    static Animation empty_animation;
    return empty_animation;
}

void AnimationSet::AddAnimation(const std::string& animation_name, int start_x, int start_y, int frame_width, int frame_height, int frame_number, float frame_duration)
{
    auto it = animation_set.find(animation_name);
    if (it != animation_set.end()) {
        return;
    }

    animation_set.emplace(animation_name, Animation(start_x, start_y, frame_width, frame_height, frame_number, frame_duration));
}
