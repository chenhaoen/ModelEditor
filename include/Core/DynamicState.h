#pragma once

enum class DynamicStateType
{
	Scissor,
	Viewport,
	PolygonMode 
};

class Command;

class DynamicState
{
public:
	DynamicState(DynamicStateType type);

	std::shared_ptr<Command> getCommand() const;
private:
	DynamicStateType m_type;
};

