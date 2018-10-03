#ifndef _WMAP_INSPECTOR_H_
#define _WMAP_INSPECTOR_H_

#include <QGroupBox>

#include "IInspectorComponent.h"

namespace maj::editor
{
	template<typename T>
	class WMapInspector : public QGroupBox, public IInspectorComponent
	{
		typedef typename T::key_type KeyType;
		typedef typename T::mapped_type MappedType;

	public:
		WMapInspector(T& value, QWidget *parent, std::function<void()> callback = nullptr);
		~WMapInspector();

	private:
		void add();
		void remove(const KeyType& key);

		void load();
		void clear(QLayout* layout);

		virtual void update();
		virtual void hideEvent(QHideEvent *event);
		virtual void showEvent(QShowEvent *event);

		T& mValue;
		uint32_t mSize;
		KeyType* mKeyTemp;
		MappedType* mMappedTemp;
	};
}

#include "WMapInspector.inl"

#endif