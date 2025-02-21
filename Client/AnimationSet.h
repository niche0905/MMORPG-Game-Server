#pragma once

class Animation;


class AnimationSet
{
	std::unordered_map<std::string, Animation> animation_set;

	const Animation& GetAnimation(const std::string animation_name) const;
	Animation& GetAnimation(const std::string animation_name);
};

