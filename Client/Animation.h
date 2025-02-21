#pragma once


class Animation
{
private:
	int offset_x, offset_y;				// Texture���� ���۵Ǵ� �ش� �ִϸ��̼� �������� ���� ��ġ
	int frame_width, frame_height;		// �� �������� ����, ���� ��
	int number_of_frames;				// �������� ��
	float frame_flip_time;				// �������� ��ȯ �ð�

public:
	Animation();
	Animation(sf::Sprite& sprite, int start_x, int start_y, int frame_w, int frame_h, int num_frames, float frame_time);

};

