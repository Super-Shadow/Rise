#pragma once

namespace Rise
{
	enum class ShaderDataType
	{
		None = 0,
		Float,
		Float2,
		Float3,
		Float4,
		Mat3,
		Mat4,
		Int,
		Int2,
		Int3,
		Int4,
		Bool
	};

	static int ShaderDataTypeSize(const ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:
				return 4;
			case ShaderDataType::Float2:
				return 4 * 2;
			case ShaderDataType::Float3:
				return 4 * 3;
			case ShaderDataType::Float4:
				return 4 * 4;
			case ShaderDataType::Mat3:
				return 4 * 3 * 3;
			case ShaderDataType::Mat4:
				return 4 * 4 * 4;
			case ShaderDataType::Int:
				return 4;
			case ShaderDataType::Int2:
				return 4 * 2;
			case ShaderDataType::Int3:
				return 4 * 3;
			case ShaderDataType::Int4:
				return 4 * 4;
			case ShaderDataType::Bool:
				return 1;
			default:
			{
				RS_CORE_ASSERT(false, "Unknown ShaderDataType!");
				return 0;
			}
		}
	}

	struct BufferElement
	{
		std::string Name;
		ShaderDataType Type;
		std::size_t Offset;
		int Size;
		bool Normalised;

		//BufferElement() = default;

		BufferElement(const ShaderDataType type, std::string name, const bool normalised = false) : Name(std::move(name)), Type(type), Offset(0), Size(ShaderDataTypeSize(type)), Normalised(normalised) {}

		[[nodiscard]] int GetComponentCount() const
		{
			switch (Type)
			{
				case ShaderDataType::Float:
					return 1;
				case ShaderDataType::Float2:
					return 2;
				case ShaderDataType::Float3:
					return 3;
				case ShaderDataType::Float4:
					return 4;
				case ShaderDataType::Mat3:
					return 3; // Was 3 * 3 and should be, but OpenGL only supports size from 1-4
				case ShaderDataType::Mat4:
					return 4; // Was 4 * 4 and should be, but OpenGL only supports size from 1-4
				case ShaderDataType::Int:
					return 1;
				case ShaderDataType::Int2:
					return 2;
				case ShaderDataType::Int3:
					return 3;
				case ShaderDataType::Int4:
					return 4;
				case ShaderDataType::Bool:
					return 1;
				default:
				{
					RS_CORE_ASSERT(false, "Unknown ShaderDataType!");
					return 0;
				}
			}
		}
	};


	class BufferLayout
	{
	public:

		BufferLayout() = default;

		BufferLayout(const std::initializer_list<BufferElement>& elements) : m_Elements(elements)
		{
			CalculateOffsetsAndStride();
		}

		[[nodiscard]] int GetStride() const { return m_Stride; }
		[[nodiscard]] const std::vector<BufferElement>& GetElements() const { return m_Elements; }

		[[nodiscard]] std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		[[nodiscard]] std::vector<BufferElement>::iterator end() { return m_Elements.end(); }

		[[nodiscard]] std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		[[nodiscard]] std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

	private:
		void CalculateOffsetsAndStride()
		{
			size_t offset = 0;
			m_Stride = 0;
			for (auto& element : m_Elements)
			{
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}
		std::vector<BufferElement> m_Elements;
		int m_Stride = 0;
	};

	class VertexBuffer
	{
	public:
		VertexBuffer(VertexBuffer const&) = delete;
		VertexBuffer& operator=(VertexBuffer const&) = delete;

		VertexBuffer(VertexBuffer&&) = delete;
		VertexBuffer& operator=(VertexBuffer&&) = delete;

		virtual void Bind() const abstract;
		virtual void Unbind() const abstract;

		[[nodiscard]] virtual const BufferLayout& GetLayout() const abstract;
		virtual void SetLayout(const BufferLayout& layout) abstract;

		virtual void SetData(const void* quadVertex, size_t size) abstract;

		static Ref<VertexBuffer> Create(std::size_t size);
		static Ref<VertexBuffer> Create(const float* vertices, std::size_t size);

	protected:
		VertexBuffer() = default;
		virtual ~VertexBuffer() = default;
	};

	class IndexBuffer
	{
	public:
		IndexBuffer(IndexBuffer const&) = delete;
		IndexBuffer& operator=(IndexBuffer const&) = delete;

		IndexBuffer(IndexBuffer&&) = delete;
		IndexBuffer& operator=(IndexBuffer&&) = delete;

		virtual void Bind() const abstract;
		virtual void Unbind() const abstract;

		[[nodiscard]] virtual std::size_t GetCount() const abstract;

		static Ref<IndexBuffer> Create(const uint32_t* indices, std::size_t count);

	protected:
		IndexBuffer() = default;
		virtual ~IndexBuffer() = default;
	};
}
