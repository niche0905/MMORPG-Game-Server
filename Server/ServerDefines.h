#pragma once


constexpr int16 VIEW_RANGE = 5;


enum IoOperation
{
	IO_START = 0,

	IO_RECV,
	IO_SEND,
	IO_ACCEPT,

	IO_NONE,

	IO_END
};
