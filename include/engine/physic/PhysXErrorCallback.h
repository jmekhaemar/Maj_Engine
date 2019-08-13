#ifndef _PHYSX_ERROR_CALLBACK_H_
#define _PHYSX_ERROR_CALLBACK_H_

#include <PxPhysicsAPI.h>
#include <Logging.h>

namespace maj::physic
{
	class PhysXErrorCallback final : public physx::PxErrorCallback
	{
	public:
		virtual void reportError(physx::PxErrorCode::Enum code, const char* kMessage, const char* kFile, int line) override;
	};
}

LOGGING_CREATE_LEVEL(PhysicInfo, maj::log::Severity::INFO, "[ INFO ][ PHYSIC ] ")
LOGGING_CREATE_LEVEL(PhysicDebug, maj::log::Severity::DEBUG, "[ DEBUG ][ PHYSIC ] ")
LOGGING_CREATE_LEVEL(PhysicWarning, maj::log::Severity::WARNING, "[ WARNING ][ PHYSIC ] ")
LOGGING_CREATE_LEVEL(PhysicError, maj::log::Severity::ERR, "[ ERROR ][ PHYSIC ] ")

#endif