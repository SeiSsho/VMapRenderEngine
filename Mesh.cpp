#include "Mesh.h"

Mesh::Mesh(	std::unique_ptr<VertexAttributeObject> vao, 
			const int& size, 
			const GLCore::Primitive& primitive)
	: _vao(std::move(vao)), _size(size), _primitive(primitive)
{

}

void Mesh::draw() const {
	_vao->bind();
	glDrawArrays(_primitive, 0, _size);
	_vao->unbind();
}
