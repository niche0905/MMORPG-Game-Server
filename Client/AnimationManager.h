#pragma once

class Animation;
class AnimationSet;


class AnimationManager
{
private:
	std::unordered_map<std::string, AnimationSet> object_animations;

public:
	AnimationManager();

	const AnimationSet& GetAnimationSet(const std::string& object_name) const;
	AnimationSet& GetAnimationSet(const std::string& object_name);

};

