#pragma once

class Compilable
{
public:
	Compilable();
	virtual ~Compilable();

	virtual void record() = 0;

	virtual void compile() = 0;

protected:
	bool m_compiled;
};

