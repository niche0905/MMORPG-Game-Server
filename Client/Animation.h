#pragma once


class Animation
{
private:
	int offset_x, offset_y;				// Texture에서 시작되는 해당 애니메이션 프레임의 시작 위치
	int frame_width, frame_height;		// 한 프레임의 넓이, 높이 값
	int number_of_frames;				// 프레임의 수
	float frame_flip_time;				// 프레임의 전환 시간

public:
	Animation();
	Animation(sf::Sprite& sprite, int start_x, int start_y, int frame_w, int frame_h, int num_frames, float frame_time);

};

