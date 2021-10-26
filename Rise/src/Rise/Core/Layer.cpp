#include "rspch.h"
#include "Layer.h"

#include <utility>

namespace Rise
{
	Layer::Layer(std::string name) : m_DebugName(std::move(name))
	{
	}
}