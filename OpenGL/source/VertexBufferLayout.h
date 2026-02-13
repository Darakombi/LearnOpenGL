#pragma once

#include "glad/glad.h"

#include <vector>

struct VertexBufferElement {
	unsigned int type;
	unsigned int count;

	static const unsigned int getTypeSize(unsigned int type) {
		switch (type) {
		case GL_FLOAT:
			return sizeof(float);
			break;
		case GL_INT:
			return sizeof(int);
			break;
		case GL_UNSIGNED_INT:
			return sizeof(unsigned int);
			break;
		case GL_BYTE:
			return sizeof(char);
			break;
		case GL_UNSIGNED_BYTE:
			return sizeof(unsigned char);
			break;
		}
	};
};

class VertexBufferLayout {
private:
	std::vector<VertexBufferElement> elements;
	unsigned int stride = 0;
public:
	template<typename T>
	void Push(unsigned int count) {
		static_assert(false);
	}

	template<>
	void Push<float>(unsigned int count) {
		elements.push_back({ GL_FLOAT, count });
		stride += count * sizeof(float);
	}

	inline const std::vector<VertexBufferElement>& getElements() const { return elements; }
	inline const unsigned int getStride() const { return stride; }
};