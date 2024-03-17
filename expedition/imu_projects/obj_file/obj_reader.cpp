#include "obj_reader.h"

#include <fstream>
#include <iostream>
#include <filesystem>
#include <cstring>

#include <Eigen/Geometry>

char* sub_str(const char* str, int start, int length) {
	int str_length = strlen(str);

	// Adjust start and length to stay within the bounds of the string
	if (start < 0)
		start = 0;
	if (start > str_length)
		return NULL;
	if (length < 0)
		length = 0;
	if (length > (str_length - start))
		length = str_length - start;

	// Allocate memory for the substring
	char* substr = (char*)malloc(length + 1);

	// Copy characters from the original string to the substring
	strncpy(substr, str + start, length);
	substr[length] = '\0'; // Null-terminate the substring

	return substr;
}

namespace ObjReader {

	using Eigen::Vector2f;
	using Eigen::Vector3f;

	std::vector<unsigned> parseFaces(const char *str) {
		int start = -1;
		int end = -1;
		std::vector<unsigned> res;
		res.reserve(6);

		std::cout << str << "\n";
		for (int i = 0; i < strlen(str); i++) {
			if (abs(str[i] - '0') < 10) { //if ith char a number
				if (start == -1) {
					start = i;
					std::cout << "START -> " << start << "\n";
				}
			}
			else {
				if (start != -1) {//if start was set earlier
					end = i - 1;
					std::cout << "END -> " << end << "\n";

					//push start to end to res
					char* substr = sub_str(str, start, end - start + 1);
					//std::cout << substr << "\n";
					res.push_back(atoi(substr));

					start = -1;
					end = -1;
				}
			}
		}
		if (start != -1) {
			end = strlen(str) - 1;
			std::cout << "END -> " << end << "\n";

			//push start to end to res
			char* substr = sub_str(str, start, end - start + 1);
			//std::cout << substr << "\n";
			res.push_back(atoi(substr));
		}
		return res;
	}

	std::list<ObjReader::Material> parseMaterialFile(const std::string& file_path) {
		std::ifstream file(file_path);
		if (!file.is_open()) {
			std::cout << "File: " << file_path << " failed\n";
			exit(1);
			return std::list<ObjReader::Material>();
		}
		std::string line;
		line.reserve(200);
		std::list<Material> materials;

		while (std::getline(file, line)) {
			if (line.empty()) {
				continue;
			}
			if (line[0] == '#') {
				continue;
			}
			if (line.find("newmtl") == 0) {
				Material mat;
				mat.name = line.substr(7);
				materials.emplace_back(std::move(mat));
				continue;
			}
			else if (line.find("Ka") == 0) {
				auto& mat = materials.back();
				size_t count = sscanf(line.c_str(), "Ka %f %f %f", &mat.Ka.x(), &mat.Ka.y(), &mat.Ka.z());
				if (count != 3) {
					std::cout << "Failed to parse Ka";
					exit(0);
				}
				continue;
			}
			else if (line.find("Kd") == 0) {
				auto& mat = materials.back();
				size_t count = sscanf(line.c_str(), "Kd %f %f %f", &mat.Kd.x(), &mat.Kd.y(), &mat.Kd.z());
				if (count != 3) {
					std::cout << "Failed to parse Kd";
					exit(0);
				}
				continue;
			}
			else if (line.find("illum") == 0) {
				auto& mat = materials.back();
				size_t count = sscanf(line.c_str(), "illum %f", &mat.illum);
				if (count != 1) {
					std::cout << "Failed to parse illum";
					exit(0);
				}
				continue;
			}
			else if (line[0] == 'd') {
				auto& mat = materials.back();
				size_t count = sscanf(line.c_str(), "d %f", &mat.d);
				if (count != 1) {
					std::cout << "Failed to parse d";
					exit(0);
				}
				continue;
			}
		}
		return materials;
	}

	std::unique_ptr<Data> load(const std::string& file_path) {
		std::filesystem::path fpath(file_path);
		const std::string file_dir_path = fpath.parent_path().string();
		auto obj_data = std::make_unique<Data>();

		std::ifstream file(file_path);
		if (!file.is_open()) {
			return obj_data;
		}

		std::string line;
		line.reserve(200);

		std::vector<Vector3f> vertex;

		while (std::getline(file, line)) {
			//std::cout << "LINE -> " << line << "\n";
			if (line.size() < 2) {
				continue;
			}
			if (line[0] == '#') {
				continue;
			}
			if (line[0] == 'v' && line[1] == ' ') {
				// Read vertex data
				std::istringstream is(line.substr(2));
				float x, y, z;
				is >> x >> y >> z;
				//std::cout << "READING Vertex data -> " << x << ", " << y << ", " << z << "\n";
				vertex.emplace_back(x, y, z);
				obj_data->max_v = obj_data->max_v.cwiseMax(vertex.back().cwiseAbs());//absolute value of vertex
				//exit(1);
			}
			else if (line[0] == 'f' && line[1] == ' ') {
				// Parse face data
				std::vector<unsigned> faces = parseFaces(line.c_str());
				if(faces.size() > 3) {
					std::cout << faces.size() << "\n";
				}
				for (int i = 0; i < faces.size() - 2; i++) { //3 faces, 1 loop
					const auto A = vertex[faces[0] - 1];
					const auto B = vertex[faces[i + 1] - 1];
					const auto C = vertex[faces[i + 2] - 1];
					//std::cout << "faces[0] - 1 -> " << faces[0] - 1 << "\n";
					//std::cout << "A -> " << A << "\n";
					//std::cout << "B -> " << B << "\n";
					//std::cout << "C -> " << C << "\n";
					const Vector3f normal = ((A-C).cross(B-C)).normalized();

					obj_data->groups.back().vertex.push_back(A);
					obj_data->groups.back().vertex.push_back(B);
					obj_data->groups.back().vertex.push_back(C);
					for (int j = 0; j < 3; j++) {
						obj_data->groups.back().normal.push_back(normal);
					}
				}
			}
			else if (line[0] == 'o') {
				obj_data->name = line.substr(2);
			}
			else if (line[0] == 'g') {
				// Create a new group with name [ line.substr(2) ] add it to obj_data->groups
				Group g;
				g.name = line.substr(2);
				obj_data->groups.emplace_back(std::move(g));
				//std::cout << "Creating group -> " << g.name << "\n";
			}
			else if (line.find("mtllib") == 0) {
				const std::string file_path = std::filesystem::path(file_dir_path).append(line.substr(7)).string();
				obj_data->materials = parseMaterialFile(file_path);
			}
			else if (line.find("usemtl") == 0) {
				// Set material name for the current group

				obj_data->groups.back().material = line.substr(7);
				//std::cout << "HERE -> " << obj_data->groups.back().name << "\n";
			}
			else {
				std::cout << line << "\n";
			}
		}
		return obj_data;
	}

}
