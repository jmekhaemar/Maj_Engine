#ifndef _WMAP_INSPECTOR_INL_
#define _WMAP_INSPECTOR_INL_

#include <QPushButton>
#include <QDialog>
#include <QLabel>

#include "Application.h"
#include "WMapInspector.h"

namespace maj::editor
{
	template<typename T>
	WMapInspector<T>::WMapInspector(T& value, QWidget *parent, std::function<void()> callback)
		: QGroupBox{ parent }, IInspectorComponent{ callback }, mValue{ value }, mKeyTemp{ nullptr }, mMappedTemp{ nullptr }
	{
		QFormLayout* layout{ new QFormLayout{ this } };
		setLayout(layout);

		load();

		connect(&mTimer, &QTimer::timeout, this, &WMapInspector<T>::update);
	}

	template<typename T>
	WMapInspector<T>::~WMapInspector()
	{
		if (mKeyTemp)
			delete mKeyTemp;

		if (mMappedTemp)
			delete mMappedTemp;
	}

	template<typename T>
	void WMapInspector<T>::add()
	{
		if(mKeyTemp)
			delete mKeyTemp;

		if(mMappedTemp)
			delete mMappedTemp;

		Application* app{ static_cast<Application*>(QApplication::instance()) };
		QDialog* dial{ app->mainWindow()->createDialog([this]()
		{
			mValue.emplace(*mKeyTemp, *mMappedTemp);
			clear(layout());
			load();
		}) };
		dial->setWindowTitle((std::string{ "Add " } + title().toStdString()).c_str());

		QHBoxLayout* boxLayout{ new QHBoxLayout{} };

		if constexpr (meta::Class<KeyType>::value)
		{
			if (WInspectorComponent::registeredInspector()[meta::Class<KeyType>::hash()])
			{
				JsonNode* dummy{ new JsonNode{ nullptr } };
				mKeyTemp = new KeyType{ dummy };
				delete dummy;
				QWidget* widget{ WInspectorComponent::registeredInspector()[meta::Class<KeyType>::hash()](mKeyTemp , dial, nullptr) };
				boxLayout->addWidget(widget);
			}
		}
		else if constexpr (std::is_enum<KeyType>::value)
		{
			if constexpr (meta::Enum<KeyType>::size() != 0u)
			{
				mKeyTemp = new KeyType{ meta::EnumValue<KeyType, 0u>::value() };
				if constexpr(meta::Enum<KeyType>::isFlag())
					boxLayout->addWidget(new WFlagInspector<KeyType>{ *mKeyTemp, this });
				else
					boxLayout->addWidget(new WEnumInspector<KeyType>{ *mKeyTemp, this });
			}
		}
		else
		{
			mKeyTemp = new KeyType{ 0u };
			QWidget* widget{ new WIntSpinBox(mKeyTemp , dial) };
			boxLayout->addWidget(widget);
		}

		QFrame* vLine = new QFrame(dial);
		vLine->setFrameShape(QFrame::VLine);
		vLine->setFrameShadow(QFrame::Sunken);
		boxLayout->addWidget(vLine);

		if constexpr (meta::Class<MappedType>::value)
		{
			if (WInspectorComponent::registeredInspector()[meta::Class<MappedType>::hash()])
			{
				JsonNode* dummy{ new JsonNode{ nullptr } };
				mMappedTemp = new MappedType{ dummy };
				delete dummy;
				boxLayout->addWidget(WInspectorComponent::registeredInspector()[meta::Class<MappedType>::hash()](mMappedTemp, dial, nullptr));
			}
		}
		else if constexpr (std::is_enum<MappedType>::value)
		{
			if constexpr (meta::Enum<MappedType>::size() != 0u)
			{
				if constexpr(meta::Enum<MappedType>::isFlag())
				{
					mMappedTemp = new MappedType{ 0u };
					boxLayout->addWidget(new WFlagInspector<MappedType>{ *mMappedTemp, dial });
				}
				else
				{
					mMappedTemp = new MappedType{ meta::EnumValue<MappedType, 0u>::value() };
					boxLayout->addWidget(new WEnumInspector<MappedType>{ *mMappedTemp, dial });
				}
			}
		}
		else
		{
			mMappedTemp = new MappedType{ "" };
			boxLayout->addWidget(new WStringInspector(*mMappedTemp, dial));
		}
		static_cast<QFormLayout*>(dial->layout())->insertRow(0, boxLayout);
		dial->show();
	}

	template<typename T>
	void WMapInspector<T>::remove(const KeyType& key)
	{
		Application* app{ static_cast<Application*>(QApplication::instance()) };
		QDialog* dial{ app->mainWindow()->createDialog([this, key]()
		{
			mValue.erase(key);
			clear(layout());
			load();
		}) };
		dial->setWindowTitle((std::string{ "Remove " } + title().toStdString()).c_str());

		static_cast<QFormLayout*>(dial->layout())->insertRow(0, new QLabel{ (std::string{ "Are you sure you want to remove this " } + title().toStdString() + '?').c_str(), dial });

		dial->show();
	}

	template<typename T>
	void WMapInspector<T>::load()
	{
		QFormLayout* formLayout{ static_cast<QFormLayout*>(layout()) };
		mSize = static_cast<uint32_t>(mValue.size());

		for (auto iterator = mValue.begin() ; iterator != mValue.end() ; )
		{
			QHBoxLayout* boxLayout{ new QHBoxLayout{} };

			if constexpr (meta::Class<KeyType>::value)
			{
				if (WInspectorComponent::registeredInspector()[meta::Class<KeyType>::hash()])
				{
					QWidget* widget{ WInspectorComponent::registeredInspector()[meta::Class<KeyType>::hash()](&const_cast<KeyType&>(iterator->first), this, nullptr) };
					widget->setDisabled(true);
					boxLayout->addWidget(widget);
				}
			}
			else if constexpr (std::is_enum<KeyType>::value)
			{
				if constexpr (meta::Enum<KeyType>::size() != 0u)
				{
					if constexpr(meta::Enum<KeyType>::isFlag())
					{
						QWidget* widget{ new WFlagInspector<KeyType>{ const_cast<KeyType&>(iterator->first), this } };
						widget->setDisabled(true);
						boxLayout->addWidget(widget);
					}
					else
					{ 
						QWidget* widget{ new WEnumInspector<KeyType>{ const_cast<KeyType&>(iterator->first), this } };
						widget->setDisabled(true);
						boxLayout->addWidget(widget);
					}
				}
			}
			else
			{
				QWidget* widget{ new WIntSpinBox(&const_cast<KeyType&>(iterator->first), this) };
				widget->setDisabled(true);
				boxLayout->addWidget(widget);
			}

			QFrame* vLine = new QFrame(this);
			vLine->setFrameShape(QFrame::VLine);
			vLine->setFrameShadow(QFrame::Sunken);
			boxLayout->addWidget(vLine);

			if constexpr (meta::Class<MappedType>::value)
			{
				if (WInspectorComponent::registeredInspector()[meta::Class<MappedType>::hash()])
				{
					boxLayout->addWidget(WInspectorComponent::registeredInspector()[meta::Class<MappedType>::hash()](&iterator->second, this, nullptr));
				}
			}
			else if constexpr (std::is_enum<MappedType>::value)
			{
				if constexpr (meta::Enum<MappedType>::size() != 0u)
				{
					if constexpr(meta::Enum<MappedType>::isFlag())
						boxLayout->addWidget(new WFlagInspector<MappedType>{ iterator->second, this });
					else
						boxLayout->addWidget(new WEnumInspector<MappedType>{ iterator->second, this });
				}
			}
			else
			{
				boxLayout->addWidget(new WStringInspector(iterator->second, this));
			}

			QPushButton* removeButton{ new QPushButton{ "-", this } };
			removeButton->setMaximumWidth(40);
			removeButton->connect(removeButton, &QPushButton::clicked, this, [this, iterator]() -> void
			{
				remove(iterator->first);
			});
			boxLayout->addWidget(removeButton);

			formLayout->addRow(boxLayout);

			iterator++;

			if (iterator != mValue.end())
			{
				QFrame* line = new QFrame(this);
				line->setFrameShape(QFrame::HLine);
				line->setFrameShadow(QFrame::Sunken);
				formLayout->addRow(line);
			}
		}

		QPushButton* addButton{ new QPushButton{ "+", this } };
		addButton->setMaximumWidth(120);
		addButton->connect(addButton, &QPushButton::clicked, this, &WMapInspector<T>::add);
		formLayout->addRow(addButton);
	}

	template<typename T>
	void WMapInspector<T>::clear(QLayout* layout)
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
	void WMapInspector<T>::update()
	{
		if (mValue.size() != mSize)
		{
			clear(layout());
			load();
		}
	}

	template<typename T>
	void WMapInspector<T>::hideEvent(QHideEvent *event)
	{
		QGroupBox::hideEvent(event);
		mTimer.stop();
	}

	template<typename T>
	void WMapInspector<T>::showEvent(QShowEvent *event)
	{
		QGroupBox::showEvent(event);
		mTimer.start(50);
	}
}

#endif