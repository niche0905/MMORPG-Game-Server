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

	// TODO : �ִϸ��̼ǿ� �ʿ��� �Լ��� �����
	// TODO : ������ ��� �������� �����ϱ�

};

