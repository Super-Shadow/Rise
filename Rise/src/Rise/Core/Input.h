#pragma once

#include "KeyCodes.h"
#include "MouseCodes.h"
#include "Rise/Core/Base.h"

namespace Rise
{
	class RISE_API Input
	{
	public:
		virtual ~Input() = default;

		// Delete these to make this class pure singleton
		Input(const Input&) = delete;
		Input& operator=(const Input&) = delete;

		Input(Input&&) = delete;
		Input& operator=(Input&&) = delete;

		static bool IsKeyPressed(const KeyCode keyCode) { return s_Instance->IsKeyPressedImpl(keyCode); }

		static bool IsMouseButtonPressed(const MouseCode button) { return s_Instance->IsMouseButtonPressedImpl(button); }
		static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
		static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
		static float GetMouseY() { return s_Instance->GetMouseYImpl(); }

		static Scope<Input> Create();
	protected:
		Input() = default;

		virtual bool IsKeyPressedImpl(KeyCode keyCode) abstract;

		virtual bool IsMouseButtonPressedImpl(MouseCode button) abstract;
		virtual std::pair<float, float> GetMousePositionImpl() abstract;
		virtual float GetMouseXImpl() abstract;
		virtual float GetMouseYImpl() abstract;
	private:
		static Scope<Input> s_Instance;
	};
}