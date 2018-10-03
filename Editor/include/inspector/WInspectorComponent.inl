#ifndef _WINSPECTOR_COMPONENT_INL_
#define _WINSPECTOR_COMPONENT_INL_

#include "WInspectorComponent.h"

#include "WFloatSpinBox.h"
#include "WIntSpinBox.h"
#include "WCheckBox.h"
#include "WStringInspector.h"
#include "WEnumInspector.h"
#include "WFlagInspector.h"
#include "WVectorInspector.h"
#include "WMapInspector.h"

#include <ISerializable.h>
#include <string>

namespace maj::editor
{
	std::unordered_map<uint32_t, std::function<WInspectorComponent*(ISerializable*, QWidget*, QFormLayout*)>>& WInspectorComponent::registeredInspector()
	{
		static std::unordered_map<uint32_t, std::function<WInspectorComponent*(ISerializable*, QWidget*, QFormLayout*)>> sRegisteredInspector{};
		return sRegisteredInspector;
	}

	std::unordered_map<uint32_t, WInspectorComponent::ComponentInfo>& WInspectorComponent::registeredComponents()
	{
		static std::unordered_map<uint32_t, ComponentInfo> sRegisteredComponents{};
		return sRegisteredComponents;
	}

	template <typename T>
	template<typename U>
	QWidget* WInspectorComponent::LoadComponent<T>::load(typename U::Type* obj, QWidget* parent, QFormLayout* layout)
	{
		typedef U CurrentField;
		typedef typename CurrentField::Type FieldType;

		if constexpr (CurrentField::isArray())
		{
			WVectorInspector<FieldType>* widget{ new WVectorInspector<FieldType>{ *obj, parent } };
			std::string subStr{ CurrentField::name() };
			widget->setTitle(subStr.substr(1).c_str());
			return widget;
		}
		else if constexpr (CurrentField::isMap())
		{
			WMapInspector<FieldType>* widget{ new WMapInspector<FieldType>{ *obj, parent } };
			std::string subStr{ CurrentField::name() };
			widget->setTitle(subStr.substr(1).c_str());
			return widget;
		}
		else if constexpr (std::is_pointer<FieldType>::value)
		{
			if constexpr (meta::Class<typename std::remove_pointer<FieldType>::type>::value)
			{
				if (WInspectorComponent::registeredInspector()[meta::Class<typename std::remove_pointer<FieldType>::type>::hash()])
				{
					std::string name{ CurrentField::name() };
					layout->setObjectName(name.substr(1).c_str());
					return WInspectorComponent::registeredInspector()[meta::Class<typename std::remove_pointer<FieldType>::type>::hash()](*obj, parent, layout);
				}
				else
					LOG(LogWarning, "Component " << meta::Class<typename std::remove_pointer<FieldType>::type>::name() << " doesn't have a registered inspector")
			}
		}
		else if constexpr (std::is_enum<FieldType>::value)
		{
			if constexpr (meta::Enum<FieldType>::size() != 0u)
			{
				if constexpr(meta::Enum<FieldType>::isFlag())
					return new WFlagInspector<FieldType>{ *obj, parent };
				else
					return new WEnumInspector<FieldType>{ *obj, parent };
			}
		}
		else if constexpr(std::is_same<FieldType, bool>::value)
		{
			return new WCheckBox(*obj, parent);
		}
		else if constexpr(std::is_integral<FieldType>::value)
		{
			return new WIntSpinBox(obj, parent);
		}
		else if constexpr(std::is_floating_point<FieldType>::value)
		{
			return new WFloatSpinBox(*obj, parent);
		}
		else if constexpr (meta::Class<FieldType>::value)
		{
			if (WInspectorComponent::registeredInspector()[meta::Class<FieldType>::hash()])
			{
				std::string name{ CurrentField::name() };
				layout->setObjectName(name.substr(1).c_str());
				return WInspectorComponent::registeredInspector()[meta::Class<FieldType>::hash()](obj, parent, layout);
			}
			else
				LOG(LogWarning, "Component " << meta::Class<FieldType>::name() << " doesn't have a registered inspector")
		}
		else
		{
			return new WStringInspector(const_cast<typename std::remove_const<FieldType>::type&>(*obj), parent);
		}
		return nullptr;
	}

	template <typename T>
	template <uint32_t i>
	void WInspectorComponent::LoadComponent<T>::invoke(T* obj, QWidget* parent, QFormLayout* layout)
	{
		typedef meta::Field<T, i> CurrentField;
		typedef typename CurrentField::Type FieldType;

		QWidget* widget{ load<CurrentField>(&CurrentField::get(obj), parent, layout) };
		if (widget)
		{
			if constexpr (std::is_enum<FieldType>::value)
			{
				std::string name{ CurrentField::name() };
				layout->addRow(name.substr(name.rfind(':') + 2u).c_str(), widget);
			}
			else if constexpr (CurrentField::isArray() || CurrentField::isMap() || meta::Class<FieldType>::value || std::is_pointer<FieldType>::value)
				layout->addRow(widget);
			else
			{
				std::string name{ CurrentField::name() };
				layout->addRow(name.substr(1).c_str(), widget);
			}
		}
	}

	template<typename T>
	void WInspectorComponent::load(T* component)
	{
		if (!component)
			return;

		if constexpr (meta::Class<T>::hasParent())
			loadParent<typename meta::Class<T>::Parent>(component);

		std::string name{ meta::Class<T>::name() };
		setObjectName(name.substr(name.rfind(':') + 1u).c_str());

		if constexpr (meta::Class<T>::size() > 0u)
			meta::staticFor<0u, meta::Class<T>::size() - 1u, LoadComponent<T>>(component, this, mLayout);
	}

	template<typename T>
	void WInspectorComponent::loadParent(T* component)
	{
		if (registeredInspector()[meta::Class<T>::hash()])
		{
			WInspectorComponent* inspector{ WInspectorComponent::registeredInspector()[meta::Class<T>::hash()](component, this, mLayout) };
			mLayout->addRow(inspector);
		}
	}

	template<typename T, typename Inspector>
	RegisterInspector<T, Inspector>::RegisterInspector()
	{
		WInspectorComponent::registeredInspector().try_emplace(meta::Class<T>::hash(), [](ISerializable* obj, QWidget* parent, QFormLayout* layout) -> WInspectorComponent*
		{
			Inspector* inspector{ new Inspector{ parent, layout } };
			inspector->template load<T>(static_cast<T*>(obj));
			return inspector;
		});
	}

	template<typename T, typename Inspector>
	RegisterComponent<T, Inspector>::RegisterComponent()
	{
		WInspectorComponent::registeredComponents().try_emplace(meta::Class<T>::hash(), meta::Class<T>::name(), [](scene::Actor* actor) -> ISerializable*
		{
			if (!actor)
				return nullptr;

			actor->addComponent<T>();
			return actor->components()[actor->componentCount() - 1u];
		});
		RegisterInspector<T, Inspector> dummy{};
	}
}

#endif