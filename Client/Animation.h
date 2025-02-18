#pragma once


class Animation
{
private:
	int frame_width, frame_height;
	int number_of_frames;
	float frame_flip_time;

	int current_frame;
	float elapsed_time;

public:
	Animation(sf::Sprite& sprite, int frame_w, int frame_h, int num_frames, float frame_time);

	// TODO : 애니메이션에 필요한 함수들 만들기
	// TODO : 구조를 어떻게 가져갈지 생각하기

};

