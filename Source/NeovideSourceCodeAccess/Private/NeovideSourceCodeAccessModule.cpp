#include "NeovideSourceCodeAccessModule.h"

#include "Features/IModularFeatures.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_MODULE(FNeovideSourceCodeAccessModule, NeovideSourceCodeAccess)

void FNeovideSourceCodeAccessModule::StartupModule()
{
	Accessor.RefreshAvailability();
	IModularFeatures::Get().RegisterModularFeature(TEXT("SourceCodeAccessor"), &Accessor);
}

void FNeovideSourceCodeAccessModule::ShutdownModule()
{
	IModularFeatures::Get().UnregisterModularFeature(TEXT("SourceCodeAccessor"), &Accessor);
}
