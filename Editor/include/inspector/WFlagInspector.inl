#ifndef _WFLAG_INSPECTOR_INL_
#define _WFLAG_INSPECTOR_INL_

#include "WFlagInspector.h"

#include <QStandardItemModel>
#include <QStandardItem>

namespace maj::editor
{
	template<typename T>
	WFlagInspector<T>::WFlagInspector(T& value, QWidget *parent, std::function<void()> callback)
		: QComboBox{ parent }, IInspectorComponent{ callback }, mValue{ value }
	{
		setAutoFillBackground(false);
		setMaximumWidth(180);
		setEditable(true);
		lineEdit()->setReadOnly(true);

		std::string dataValue {};
		QStandardItemModel* model { new QStandardItemModel { static_cast<int32_t>(meta::Enum<T>::size()), 1 } };
		for (uint32_t r { 0u } ; r < meta::Enum<T>::size() ; ++r)
		{
			std::string name { meta::Enum<T>::getName(r) };
			QStandardItem* item { new QStandardItem(name.substr(name.rfind(':') + 1u).c_str()) };
			item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
			item->setData((static_cast<uint32_t>(value & meta::Enum<T>::getValue(r)) != 0u) ? Qt::Checked : Qt::Unchecked, Qt::CheckStateRole);
			dataValue += (static_cast<uint32_t>(value & meta::Enum<T>::getValue(r)) != 0u) ? name.substr(name.rfind(':') + 1u) + ';' : "";
			model->setItem(r, 0, item);
		}
		setModel(model);

		setEditText(dataValue.c_str());

		connect(model, &QStandardItemModel::dataChanged, this, &WFlagInspector<T>::onValueChanged);
		connect(&mTimer, &QTimer::timeout, this, &WFlagInspector::update);
	}

	template<typename T>
	void WFlagInspector<T>::onValueChanged(const QModelIndex& kTopLeft, const QModelIndex& kBottomRight)
	{
		(void) kBottomRight;

		if((static_cast<uint32_t>(mValue & meta::Enum<T>::getValue(kTopLeft.row())) == 0u))
			mValue |= meta::Enum<T>::getValue(kTopLeft.row());
		else
			mValue &= ~meta::Enum<T>::getValue(kTopLeft.row());

		std::string dataValue {};
		for (uint32_t r { 0u } ; r < meta::Enum<T>::size(); ++r)
		{
			std::string name{ meta::Enum<T>::getName(r) };
			if (static_cast<QStandardItemModel*>(model())->item(r)->checkState() == Qt::Checked)
				dataValue += name.substr(name.rfind(':') + 1u) + ';';
		}
		setEditText(dataValue.c_str());

		if (mCallback)
			mCallback();
	}

	template<typename T>
	void WFlagInspector<T>::update()
	{
		for (uint32_t r { 0u } ; r < meta::Enum<T>::size() ; ++r)
		{
			Qt::CheckState isChecked{ (static_cast<uint32_t>(mValue & meta::Enum<T>::getValue(r)) != 0u) ? Qt::Checked : Qt::Unchecked };
			if (isChecked != static_cast<QStandardItemModel*>(model())->item(r)->checkState() && !hasFocus())
				static_cast<QStandardItemModel*>(model())->item(r)->setData(isChecked, Qt::CheckStateRole);
		}
	}

	template<typename T>
	void WFlagInspector<T>::hideEvent(QHideEvent *event)
	{
		QComboBox::hideEvent(event);
		mTimer.stop();
	}

	template<typename T>
	void WFlagInspector<T>::showEvent(QShowEvent *event)
	{
		QComboBox::showEvent(event);
		mTimer.start(50);
	}
}

#endif