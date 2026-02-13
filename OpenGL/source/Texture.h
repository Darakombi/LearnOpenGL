#pragma once

#include <iostream>

class Texture {
private:
	unsigned int ID = 0;
	int Width = 0, Height = 0, Channels = 0;
public:
	Texture(const std::string filepath);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;
};