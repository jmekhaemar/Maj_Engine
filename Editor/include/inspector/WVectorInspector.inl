#ifndef _WVECTOR_INSPECTOR_INL_
#define _WVECTOR_INSPECTOR_INL_

#include <QPushButton>
#include <QDialog>
#include <QLabel>

#include "Application.h"
#include "WVectorInspector.h"

namespace maj::editor
{
	template<typename T>
	WVectorInspector<T>::WVectorInspector(T& value, QWidget *parent, std::function<void()> callback)
		: QGroupBox{ parent }, IInspectorComponent{ callback }, mValue{ value }, mTemp{ nullptr }
	{
		QFormLayout* layout{ new QFormLayout{ this } };
		setLayout(layout);

		load();

		connect(&mTimer, &QTimer::timeout, this, &WVectorInspector<T>::update);
	}

	template<typename T>
	WVectorInspector<T>::~WVectorInspector()
	{
		if (mTemp)
			delete mTemp;
	}

	template<typename T>
	void WVectorInspector<T>::add()
	{
		if (mTemp)
			delete mTemp;

		Application* app{ static_cast<Application*>(QApplication::instance()) };
		QDialog* dial{ app->mainWindow()->createDialog([this]() 
		{
			mValue.emplace_back(*mTemp);
			clear(layout());
			load();
		}) };
		dial->setWindowTitle(( std::string{ "Add " } + title().toStdString() ).c_str());

		if constexpr (!std::is_pointer<ValueType>::value)
		{
			JsonNode* dummy{ new JsonNode{ nullptr } };
			mTemp = new ValueType{ dummy };
			delete dummy;
			if (WInspectorComponent::registeredInspector()[meta::Class<ValueType>::hash()])
				static_cast<QFormLayout*>(dial->layout())->insertRow(0, WInspectorComponent::registeredInspector()[meta::Class<ValueType>::hash()](mTemp, dial, nullptr));
		}
		else
		{
			JsonNode* dummy{ new JsonNode{ nullptr } };
			mTemp = &(new typename std::remove_pointer<ValueType>::type{});
			delete dummy;
			if (WInspectorComponent::registeredInspector()[meta::Class<typename std::remove_pointer<ValueType>::type>::hash()])
				static_cast<QFormLayout*>(dial->layout())->insertRow(0, WInspectorComponent::registeredInspector()[meta::Class<typename std::remove_pointer<ValueType>::type>::hash()](*mTemp, dial, nullptr));
		}
		
		dial->show();
	}

	template<typename T>
	void WVectorInspector<T>::remove()
	{
		Application* app{ static_cast<Application*>(QApplication::instance()) };
		QDialog* dial{ app->mainWindow()->createDialog([this]() 
		{
			if (mValue.size() == 0u)
				return;

			if constexpr (std::is_pointer<ValueType>::value)
				delete mValue.back();

			mValue.pop_back();
			clear(layout());
			load();
		}) };
		dial->setWindowTitle((std::string{ "Remove " } + title().toStdString()).c_str());

		static_cast<QFormLayout*>(dial->layout())->insertRow(0, new QLabel{ (std::string{"Are you sure you want to remove the last "} + title().toStdString() + '?').c_str(), dial });

		dial->show();
	}

	template<typename T>
	void WVectorInspector<T>::load()
	{
		QFormLayout* formLayout{ static_cast<QFormLayout*>(layout()) };
		mSize = static_cast<uint32_t>(mValue.size());

		for (uint32_t index{ 0u }; index < mSize; ++index)
		{
			if constexpr (!std::is_pointer<ValueType>::value)
			{
				if (WInspectorComponent::registeredInspector()[mValue[index].hash()])
				{
					WInspectorComponent* inspector{ WInspectorComponent::registeredInspector()[mValue[index].hash()](&mValue[index], this, formLayout) };
					formLayout->addRow(inspector);
				}
			}
			else
			{
				if (WInspectorComponent::registeredInspector()[mValue[index]->hash()])
				{
					WInspectorComponent* inspector{ WInspectorComponent::registeredInspector()[mValue[index]->hash()](mValue[index], this, formLayout) };
					formLayout->addRow(inspector);
				}
			}

			if (index != mSize - 1u)
			{
				QFrame* line = new QFrame(this);
				line->setFrameShape(QFrame::HLine);
				line->setFrameShadow(QFrame::Sunken);
				formLayout->addRow(line);
			}
		}

		QHBoxLayout* buttonLayout{ new QHBoxLayout{} };
		formLayout->addRow(buttonLayout);

		if constexpr ((std::is_pointer<ValueType>::value && !std::is_abstract<typename std::remove_pointer<ValueType>::type>::value) || !std::is_pointer<ValueType>::value)
		{
			QPushButton* addButton{ new QPushButton{ "+", this } };
			addButton->setMaximumWidth(120);
			addButton->connect(addButton, &QPushButton::clicked, this, &WVectorInspector<T>::add);
			buttonLayout->addWidget(addButton);
		}

		QPushButton* removeButton{ new QPushButton{ "-", this } };
		removeButton->setMaximumWidth(120);
		removeButton->connect(removeButton, &QPushButton::clicked, this, &WVectorInspector<T>::remove);
		buttonLayout->addWidget(removeButton);
	}

	template<typename T>
	void WVectorInspector<T>::clear(QLayout* layout)
	{
		QLayoutItem *item;
		while (layout->count() != 0) 
		{
			item = layout->takeAt(0);
			if (item->layout())
				clear(item->layout());
			if (item->widget())
			{
				item->widget()->setParent(nullptr);
				delete item->widget();
			}
			delete item;
		}
	}

	template<typename T>
	void WVectorInspector<T>::update()
	{
		if (mValue.size() != mSize)
		{
			clear(layout());
			load();
		}
	}

	template<typename T>
	void WVectorInspector<T>::hideEvent(QHideEvent *event)
	{
		QGroupBox::hideEvent(event);
		mTimer.stop();
	}

	template<typename T>
	void WVectorInspector<T>::showEvent(QShowEvent *event)
	{
		QGroupBox::showEvent(event);
		mTimer.start(50);
	}
}

#endif