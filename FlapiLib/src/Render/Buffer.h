#pragma once

namespace FL
{
	enum class ShaderType
	{
		Bool,
		Int, Int2,Int3,Int4,
		Float,Float2,Float3,Float4,
		Mat2,Mat3,Mat4
	};

	struct BufferElement
	{
		std::string Name;
		ShaderType Type;
		bool Normalized;
		uint32_t Offset;

		BufferElement(ShaderType type, const std::string name, bool normalized = false)
			:Type(type), Name(name), Normalized(normalized)
		{
		}
	};

	static uint32_t GetLayoutElementCount(ShaderType type)
	{
		switch (type)
		{
		case ShaderType::Bool:		return 1;
		case ShaderType::Int:		return 1;
		case ShaderType::Int2:		return 2;
		case ShaderType::Int3:		return 3;
		case ShaderType::Int4:		return 4;
		case ShaderType::Float:		return 1;
		case ShaderType::Float2:	return 2;
		case ShaderType::Float3:	return 3;
		case ShaderType::Float4:	return 4;
		case ShaderType::Mat2:		return 2 * 2;
		case ShaderType::Mat3:		return 3 * 3;
		case ShaderType::Mat4:		return 4 * 4;
		}
	}

	static uint32_t GetLayoutElementSize(ShaderType type)
	{
		switch (type)
		{
		case ShaderType::Bool:		return 1 * sizeof(bool);
		case ShaderType::Int:		return 1 * sizeof(int);
		case ShaderType::Int2:		return 2 * sizeof(int);
		case ShaderType::Int3:		return 3 * sizeof(int);
		case ShaderType::Int4:		return 4 * sizeof(int);
		case ShaderType::Float:		return 1 * sizeof(float);
		case ShaderType::Float2:	return 2 * sizeof(float);
		case ShaderType::Float3:	return 3 * sizeof(float);
		case ShaderType::Float4:	return 4 * sizeof(float);
		case ShaderType::Mat2:		return 2 * 2 * sizeof(float);
		case ShaderType::Mat3:		return 3 * 3 * sizeof(float);
		case ShaderType::Mat4:		return 4 * 4 * sizeof(float);
		}
	}

	class BufferLayout
	{
	public:
		BufferLayout() = default;
		BufferLayout(std::initializer_list<BufferElement>list)
		{
			for (auto& element : list)
			{
				m_Elements.emplace_back(element);
			}
			CalculateStrideOffset();
		}

		uint32_t GetStride() const { return Stride; }
		const std::vector<BufferElement>& GetElements() const { return m_Elements; }

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
	private:
		void CalculateStrideOffset()
		{
			for (auto& element : m_Elements)
			{
				element.Offset = Stride;
				Stride += GetLayoutElementSize(element.Type);
			}
		}
	private:
		uint32_t Stride = 0;
		std::vector<BufferElement> m_Elements;
	};


	class VertexBuffer
	{
	public:
		VertexBuffer(float* vertices, uint32_t size);
		~VertexBuffer();

		static Ref<VertexBuffer> Create(float* vertices, uint32_t size);

		const BufferLayout& GetLayout() { return m_Layout; }
		void SetLayout(const BufferLayout& layout) { m_Layout = layout; }

		void Bind();
		void UnBind();
	
		void SetBufferData(const std::vector<float> vertices);
	private:
		BufferLayout m_Layout;
		uint32_t m_RenderID;
	};


	class IndexBuffer
	{
	public:
		IndexBuffer(uint32_t* indices, uint32_t count);
		~IndexBuffer();

		static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t count);

		uint32_t GetCount() { return m_Count; }

		void Bind();
		void UnBind();

		void SetBufferData(const std::vector<uint32_t> indices);
	private:
		uint32_t m_RenderID;
		uint32_t m_Count;
	};


}