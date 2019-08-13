#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include <vector>

#include "Key.h"

namespace maj::ww
{
	class Controller final : public NonCopyable
	{
	public:
		static const Controller skInvalid;

	public:
		Controller(uint8_t index);
		Controller(Controller&& other);
		~Controller();

		bool update();

		const char* name() const;
		const Key& key(uint8_t i) const;
		float axis(uint8_t i) const;

		uint8_t keyCount() const;
		uint8_t axisCount() const;

	private:
		Controller();

		uint8_t mIndex;
		const char* mkName;

		std::vector<Key> mKeys;
		const float* mAxes;
		uint8_t mAxisCount;
	};
}

#endif