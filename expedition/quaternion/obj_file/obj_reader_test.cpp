#include "obj_reader.h"

#include <iostream>

int main(void) {
	const auto data = ObjReader::load("C:/Users/arjun/Downloads/sample_obj_files/sample_obj_files/space_shuttle/tinker.obj");
	if (data) {
		std::cout << "Group data ...\n";
		for (const auto& group : data->groups) {
			std::cout << group.name << " : " << group.vertex.size() << "\n";
		}
		std::cout << "Material data ...\n";
		for (const auto& mat : data->materials) {
			std::cout << mat.name << " Kd: " << mat.Kd.transpose() << " Ka: " << mat.Ka.transpose() << "\n";
		}
	}
	return 0;
}
