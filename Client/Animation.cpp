#include "pch.h"
#include "Animation.h"


Animation::Animation(sf::Sprite& sprite, int start_x, int start_y, int frame_w, int frame_h, int num_frames, float frame_time)
	: offset_x{ start_x }
	, offset_y{ start_y }
	, frame_width{ frame_w }
	, frame_height{ frame_h }
	, number_of_frames{ num_frames }
	, frame_flip_time{ frame_time }
	, current_frame{ 0 }
	, elapsed_time{ 0.0f }
{
	sprite.setTextureRect(sf::IntRect(0, 0, frame_width, frame_height));
}

