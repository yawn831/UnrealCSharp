#pragma once

#include "TMethodHelper.inl"

template <typename Return = void>
struct TMethodBuilder
{
	template <typename Class, typename... Args>
	static auto Invoke(Class* InObject, const FString& InName, Args&&... InArgs)
	{
		return TMethodHelper<std::tuple<Return, std::tuple<Args...>>>::template Call<Class>(
			InObject, InName, std::forward<Args>(InArgs)...);
	}
};
