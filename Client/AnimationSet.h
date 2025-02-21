#pragma once

class Animation;


class AnimationSet
{
private:
	std::unordered_map<std::string, Animation> animation_set;

public:
	const Animation& GetAnimation(const std::string animation_name) const;
	Animation& GetAnimation(const std::string animation_name);

};

