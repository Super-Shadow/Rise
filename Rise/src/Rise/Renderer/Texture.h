#pragma once

#include "Rise/Core/Core.h"
#include <string>

namespace Rise
{
	class Texture
	{
	public:
		Texture(const Texture&) = delete;
		Texture& operator= (const Texture&) = delete;

		Texture(Texture&&) = delete;
		Texture& operator=(Texture&&) = delete;

		virtual ~Texture() = default;

		[[nodiscard]] virtual int GetWidth() const abstract;
		[[nodiscard]] virtual int GetHeight() const abstract;

		virtual void SetData(void* data, std::size_t size) abstract;

		virtual void Bind(uint32_t slot = 0) const abstract;
	protected:
		Texture() = default;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(int width, int height);
		static Ref<Texture2D> Create(const std::string& path);

	};
}
