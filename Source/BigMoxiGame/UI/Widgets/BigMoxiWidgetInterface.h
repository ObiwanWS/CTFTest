// Copyright (c) 2024 Obiwan Medeiros.

#pragma once

#include <UObject/Interface.h>

#include "BigMoxiWidgetInterface.generated.h"

class UWidget;

UINTERFACE(MinimalAPI)
class UBigMoxiWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

class IBigMoxiWidgetInterface
{
	GENERATED_BODY()

public:
	//
	void SetRootWidget(UWidget* Widget);

	//
	UWidget* GetRootWidget()
	{
		return RootWidget;
	}

	virtual void Activate() {}
	virtual void Deactivate() {}

private:
	TObjectPtr<UWidget> RootWidget;
};
