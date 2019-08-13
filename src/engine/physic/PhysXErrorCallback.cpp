#include "physic/PhysXErrorCallback.h"

namespace maj::physic
{
	void PhysXErrorCallback::reportError(physx::PxErrorCode::Enum code, const char* kMessage, const char* kFile, int line)
	{
		switch (code)
		{
			case physx::PxErrorCode::Enum::eABORT:
			case physx::PxErrorCode::Enum::eOUT_OF_MEMORY:
			case physx::PxErrorCode::Enum::eINTERNAL_ERROR:
			THROW(std::string(kMessage) + " ( " + kFile + ": " + std::to_string(line) + " )")

			case physx::PxErrorCode::Enum::eINVALID_OPERATION:
			case physx::PxErrorCode::Enum::eINVALID_PARAMETER:
			LOG(LogPhysicError, std::string(kMessage) << " ( " << kFile << ": " << std::to_string(line) << " )")
			break;

			case physx::PxErrorCode::Enum::eDEBUG_WARNING:
			case physx::PxErrorCode::Enum::ePERF_WARNING:
			LOG(LogPhysicWarning, std::string(kMessage) << " ( " << kFile << ": " << std::to_string(line) << " )")
			break;

			case physx::PxErrorCode::Enum::eDEBUG_INFO:
			LOG(LogPhysicDebug, std::string(kMessage) << " ( " << kFile << ": " << std::to_string(line) << " )")
			break;

			default:
			LOG(LogPhysicInfo, std::string(kMessage) << " ( " << kFile << ": " << std::to_string(line) << " )")
			break;
		}
	}
}