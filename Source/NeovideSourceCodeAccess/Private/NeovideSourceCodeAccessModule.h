#pragma once

#include "Modules/ModuleInterface.h"
#include "NeovideSourceCodeAccessor.h"

class FNeovideSourceCodeAccessModule final : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	FNeovideSourceCodeAccessor Accessor;
};
