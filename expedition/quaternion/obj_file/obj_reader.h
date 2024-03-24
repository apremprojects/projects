#pragma once

#include <list>
#include <vector>
#include <Eigen/Core>

namespace ObjReader {

	struct Material {
		std::string name;
		float d = 0.0f;
		float illum = 0.0f;
		Eigen::Vector3f Ka = Eigen::Vector3f::Zero();
		Eigen::Vector3f Kd = Eigen::Vector3f(0.5f, 0.5f, 0.5f);
	};

	struct Group {
		std::string name;
		std::string material;
		std::vector<Eigen::Vector3f> vertex;
		std::vector<Eigen::Vector3f> normal;
	};

	struct Data {
		std::string name;
		Eigen::Vector3f max_v = Eigen::Vector3f::Zero();
		std::list<Group> groups;
		std::list<Material> materials;
	};

	std::unique_ptr<Data> load(const std::string& file_path);

};
