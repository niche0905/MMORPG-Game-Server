#include "pch.h"
#include "Animation.h"


Animation::Animation()
	: offset_x{  }
	, offset_y{  }
	, frame_width{  }
	, frame_height{  }
	, number_of_frames{  }
	, frame_flip_time{  }
{

}

Animation::Animation(int start_x, int start_y, int frame_w, int frame_h, int num_frames, float frame_time)
	: offset_x{ start_x }
	, offset_y{ start_y }
	, frame_width{ frame_w }
	, frame_height{ frame_h }
	, number_of_frames{ num_frames }
	, frame_flip_time{ frame_time }
{

}

