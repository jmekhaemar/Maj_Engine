#ifndef _SERVICE_H_
#define _SERVICE_H_

#include <unordered_map>

#include "NonCopyable.h"

namespace maj
{
	class Service : public NonCopyable
	{
	public:
		template<typename T>
		static T* get();

		static const std::unordered_map<std::string, Service*>& services();
		static void services(const std::unordered_map<std::string, Service*>& kServices);

	private:
		static bool exist(const char* kName);

		static std::unordered_map<std::string, Service*> sServices;

	public:
		Service(const char* kName);
		~Service();

	private:
		const char* mkName;
	};
}

#include "Service.inl"

#endif