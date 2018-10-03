#ifndef _WVECTOR_INSPECTOR_H_
#define _WVECTOR_INSPECTOR_H_

#include <QGroupBox>

#include "inspector/IInspectorComponent.h"

namespace maj::editor
{
	template<typename T>
	class WVectorInspector : public QGroupBox, public IInspectorComponent
	{
		typedef typename T::value_type ValueType;

	public:
		WVectorInspector(T& value, QWidget* parent, std::function<void()> callback = nullptr);
		~WVectorInspector();

	private:
		void add();
		void remove();

		void load();
		void clear(QLayout* layout);

		virtual void update();
		virtual void hideEvent(QHideEvent *event);
		virtual void showEvent(QShowEvent *event);

		T& mValue;
		uint32_t mSize;
		ValueType* mTemp;
	};
}

#include "inspector/WVectorInspector.inl"

#endif