// Copyright (c) 2024 Obiwan Medeiros.

#pragma once

#include <Modules/ModuleManager.h>

class FBigMoxiGameModule : public FDefaultGameModuleImpl
{
public:
	//~ Begin of IModuleInterface interface.
	void StartupModule() override;
	void ShutdownModule() override;
	//~ End of IModuleInterface interface.
};
