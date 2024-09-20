// Copyright (c) 2024 Obiwan Medeiros.

#include "BigMoxiWidgetInterface.h"
#include "BigMoxiGameDebug.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BigMoxiWidgetInterface)

void IBigMoxiWidgetInterface::SetRootWidget(UWidget* Widget)
{
	if (Widget)
	{
		RootWidget = Widget;
	}
	else
	{
		UE_LOG(LogBigMoxiGame, Error, TEXT("Attempted to set Root as a widget that isn't valid"));
	}
}
