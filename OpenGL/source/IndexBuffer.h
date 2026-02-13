#pragma once

class IndexBuffer
{

private:
	unsigned int ID = 0;
public:
	IndexBuffer(const unsigned int* data, const unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

};