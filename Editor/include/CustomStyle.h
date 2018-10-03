#ifndef _CUSTOM_STYLE_H_
#define _CUSTOM_STYLE_H_

#include <QProxyStyle>

namespace maj::editor
{
	class CustomStyle : public QProxyStyle
	{
	public:
		CustomStyle();

		virtual int styleHint(StyleHint hint, const QStyleOption *option = 0,
			const QWidget *widget = 0, QStyleHintReturn *returnData = 0) const;
	};
}

#endif