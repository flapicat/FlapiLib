#pragma once
#include <glm/glm.hpp>

namespace FL
{
	class Shader
	{
	public:
		Shader(const std::string& vertexPath, const std::string& fragmentPath);
		~Shader();

		void Use();

		static Ref<Shader> Create(const std::string& vertexPath, const std::string& fragmentPath);

		void setBool(const std::string& name, bool value) const;
		void setInt(const std::string& name, int value) const;
		void setFloat(const std::string& name, float value) const;

		void setVec2(const std::string& name, const glm::vec2 value) const;
		void setVec2(const std::string& name, float x, float y) const;

		void setVec3(const std::string& name, const glm::vec3 value) const;
		void setVec3(const std::string& name, float x, float y, float z) const;

		void setVec4(const std::string& name, const glm::vec4 value) const;
		void setVec4(const std::string& name, float x, float y, float z, float w) const;

		void setMat2(const std::string& name, const glm::mat2& mat) const;
		void setMat3(const std::string& name, const glm::mat3& mat) const;
		void setMat4(const std::string& name, const glm::mat4& mat) const;

	private:
		void checkCompileErrors(unsigned int shader, std::string type);
	private:
		uint32_t m_RendererID;
	};

}