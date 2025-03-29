#pragma once

class Command
{
public:
	Command() = default;
	virtual ~Command() = 0;

	virtual void record() = 0;
};

