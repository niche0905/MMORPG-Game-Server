#pragma once
#include "Animation.h"
#include "AnimationSet.h"

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

	void AddAnimationObject(const std::string& object_name);

};

