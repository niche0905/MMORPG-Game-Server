#include "pch.h"
#include "Button.h"

Button::Button()
	: Button{0}
{

}

Button::Button(uint32 b_id)
	: Object{}
	, button_id{ b_id }
{

}
