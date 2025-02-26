#pragma once

class BufferManager
{
public:
	static BufferManager* instance();

	void destory();

private:
	BufferManager();
	~BufferManager();

private:
	static BufferManager* g_instance;
};

