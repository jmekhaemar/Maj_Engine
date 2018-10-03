#include "inspector/WInspectorComponent.h"

namespace maj::editor
{
	typedef std::unordered_map<uint32_t, std::function<WInspectorComponent*(ISerializable*, QWidget*, QFormLayout*)>> InspectorMap;

	void WInspectorComponent::registerInspector(const InspectorMap& kInspectors)
	{
		InspectorMap& registered { registeredInspector() };

		// Remove unused symbols
		for (InspectorMap::iterator it { registered.begin() } ; it != registered.end() ;)
		{
			if (kInspectors.count(it->first) == 0u)
				it = registered.erase(it);
			else
				++it;
		}

		// Add used symbols
		for (const InspectorMap::value_type& kInspector : kInspectors)
		{
			if (registered.count(kInspector.first) != 0u)
				continue;

			registered.emplace(kInspector.first, kInspector.second);
		}
	}

	void WInspectorComponent::registerComponents(const std::unordered_map<uint32_t, ComponentInfo>& kComponents)
	{
		std::unordered_map<uint32_t, ComponentInfo>& registered { registeredComponents() };

		// Remove unused symbols
		for (std::unordered_map<uint32_t, ComponentInfo>::iterator it { registered.begin() } ; it != registered.end() ;)
		{
			if (kComponents.count(it->first) == 0u)
				it = registered.erase(it);
			else
				++it;
		}

		// Add used symbols
		for (const std::pair<const uint32_t, ComponentInfo>& kComponent : kComponents)
		{
			if (registered.count(kComponent.first) != 0u)
				continue;

			registered.emplace(kComponent.first, kComponent.second);
		}
	}

	WInspectorComponent::ComponentInfo::ComponentInfo(const std::string& kName, const std::function<ISerializable*(scene::Actor*)>& kFunc)
		: mName{ kName }, mCreateComponent{ kFunc }
	{}

	WInspectorComponent::WInspectorComponent(QWidget* parent, QFormLayout* layout, std::function<void()> callback)
		: QWidget{ parent }, IInspectorComponent{ callback }, mLayout{ layout }
	{
		if (!mLayout)
		{
			mLayout = new QFormLayout(this);
			setLayout(mLayout);
		}
	}

	WInspectorComponent::~WInspectorComponent()
	{}

	void WInspectorComponent::hideEvent(QHideEvent *event)
	{
		QWidget::hideEvent(event);
	}

	void WInspectorComponent::showEvent(QShowEvent *event)
	{
		QWidget::showEvent(event);
	}
}