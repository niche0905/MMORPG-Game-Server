#pragma once
#include "Animation.h"

class Animation;


class AnimationSet
{
private:
	std::unordered_map<std::string, Animation> animation_set;

public:
	AnimationSet();

	const Animation& GetAnimation(const std::string& animation_name) const;
	Animation& GetAnimation(const std::string& animation_name);

	void AddAnimation(const std::string& animation_name, int start_x, int start_y, int frame_width, int frame_height, int frame_number, float frame_duration);

};

