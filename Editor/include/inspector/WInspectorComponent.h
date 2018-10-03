#ifndef _WINSPECTOR_COMPONENT_H_
#define _WINSPECTOR_COMPONENT_H_

#include <QBoxLayout>
#include <QFormLayout>

#include <unordered_map>

#include "ISerializable.h"
#include "IInspectorComponent.h"
#include "scene/Actor.h"

namespace maj::editor
{
	class WInspectorComponent : public QWidget, public IInspectorComponent
	{
	public:
		struct ComponentInfo final
		{
			ComponentInfo(const std::string& kName, const std::function<ISerializable*(scene::Actor*)>& kFunc);

			std::string mName;
			std::function<ISerializable* (scene::Actor*)> mCreateComponent;
		};

	protected:
		template<typename T>
		struct LoadComponent final
		{
			template<typename U>
			static QWidget* load(typename U::Type* obj, QWidget* parent, QFormLayout* layout);

			template<uint32_t i>
			static void invoke(T* obj, QWidget* parent, QFormLayout* layout);
		};

	public:
		inline static std::unordered_map<uint32_t, std::function<WInspectorComponent*(ISerializable*, QWidget*, QFormLayout*)>>& registeredInspector();
		static void registerInspector(const std::unordered_map<uint32_t, std::function<WInspectorComponent*(ISerializable*, QWidget*, QFormLayout*)>>& kInspectors);
		
		inline static std::unordered_map<uint32_t, ComponentInfo>& registeredComponents();
		static void registerComponents(const std::unordered_map<uint32_t, ComponentInfo>& kComponents);

	public:
		WInspectorComponent(QWidget* parent, QFormLayout* layout = nullptr, std::function<void()> callback = nullptr);
		virtual ~WInspectorComponent();

		template<typename T>
		void load(T* component);

	protected:
		template<typename T>
		void loadParent(T* component);

		virtual void hideEvent(QHideEvent *event);
		virtual void showEvent(QShowEvent *event);

		QFormLayout* mLayout;

	};

	template<typename T, typename Inspector = WInspectorComponent>
	struct RegisterInspector final
	{
		RegisterInspector();
	};

	template<typename T, typename Inspector = WInspectorComponent>
	struct RegisterComponent final
	{
		RegisterComponent();
	};
}

#define REGISTER_INSPECTOR(CLASS) maj::editor::RegisterInspector<CLASS> __ ## CLASS;
#define REGISTER_INSPECTOR_CUSTOM(CLASS, NAME) maj::editor::RegisterInspector<CLASS, NAME> __ ## CLASS;

#define REGISTER_COMPONENT_INSPECTOR(CLASS) maj::editor::RegisterComponent<CLASS> __ ## CLASS;
#define REGISTER_COMPONENT_INSPECTOR_CUSTOM(CLASS, NAME) maj::editor::RegisterComponent<CLASS, NAME> __ ## CLASS;

#include "WInspectorComponent.inl"

#endif