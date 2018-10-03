#ifndef _WASSET_INSPECTOR_H_
#define _WASSET_INSPECTOR_H_

#include "..\WInspectorComponent.h"

#include <QDropEvent>
#include <QDragEnterEvent>
#include <QDragMoveEvent>

#include <QtGui/qevent.h>

namespace maj::editor
{
	class WAssetInspector : public WInspectorComponent
	{
	public:
		WAssetInspector(QWidget* parent, QFormLayout* layout, std::function<void()> callback = nullptr);

		template<typename T>
		void load(T* component);

	private:
		std::string mName;
	};
}

#include "WAssetInspector.inl"

#endif