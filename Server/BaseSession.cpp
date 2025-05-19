#include "pch.h"
#include "BaseSession.h"


BaseSession::BaseSession()
	: BaseSession(-1)
{

}

BaseSession::BaseSession(int64 id)
	: _id{ id }
	, _basic_stats{}
	, _temp_stats{}
{

}

BaseSession::~BaseSession()
{

}

int64 BaseSession::GetID() const
{
	return _id;
}
