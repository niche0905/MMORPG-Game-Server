#pragma once


class Animation
{
private:
	int offset_x, offset_y;
	int frame_width, frame_height;
	int number_of_frames;
	float frame_flip_time;

	int current_frame;
	float elapsed_time;

public:
	Animation(sf::Sprite& sprite, int start_x, int start_y, int frame_w, int frame_h, int num_frames, float frame_time);

	// TODO : �ִϸ��̼ǿ� �ʿ��� �Լ��� �����
	// TODO : ������ ��� �������� �����ϱ�

};

