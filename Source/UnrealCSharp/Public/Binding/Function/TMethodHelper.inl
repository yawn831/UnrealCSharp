#pragma once

#include "Environment/FCSharpEnvironment.h"
#include "Binding/Property/TPropertyBuilder.inl"
#include "Reflection/FReflectionRegistry.h"

template <typename>
struct TMethodHelper
{
};

template <typename Result, typename... Args>
struct TMethodHelper<std::tuple<Result, std::tuple<Args...>>>
{
public:
	template <typename Class>
	static auto Call(Class* InObject, const FString& InName, Args&&... InArgs)
	{
		MonoObject* Object{};

		FClassReflection* ClassReflection{};

		if (InObject == nullptr)
		{
			ClassReflection = FReflectionRegistry::Get().GetClass<Class>();
		}
		else if constexpr (TIsUObject<Class>::Value)
		{
			Object = FCSharpEnvironment::GetEnvironment().GetObject(InObject);

			if (Object == nullptr)
			{
				return DefaultReturn();
			}

			ClassReflection = FReflectionRegistry::Get().GetClass(InObject->GetClass());
		}
		else
		{
			Object = FCSharpEnvironment::GetEnvironment().GetBinding(InObject);

			if (Object == nullptr)
			{
				return DefaultReturn();
			}

			ClassReflection = FReflectionRegistry::Get().GetClass<Class>();
		}

		if (ClassReflection == nullptr)
		{
			return DefaultReturn();
		}

		const auto Method = ClassReflection->GetParentMethod(InName, static_cast<int32>(sizeof...(Args)));

		if (Method == nullptr)
		{
			return DefaultReturn();
		}

		const auto ReturnValue = Method->Runtime_Invoke_Array(
			Object, ArgumentToArray(std::index_sequence_for<Args...>{},
			                        std::tuple<Args...>(std::forward<Args>(InArgs)...)));

		if constexpr (!std::is_void_v<Result>)
		{
			return TPropertyValue<Result, Result>::Get(ReturnValue);
		}
	}

private:
	template <auto... Index>
	static auto ArgumentToArray(std::index_sequence<Index...>, std::tuple<std::decay_t<Args>...>&& InArgs)
	{
		if constexpr (constexpr auto Size = static_cast<int32>(sizeof...(Args));
			Size > 0)
		{
			const auto Array = FReflectionRegistry::Get().GetObjectClass()->NewArray(Size);

			(FDomain::Array_Set(Array, static_cast<int32>(Index),
			                    TPropertyBuilder<std::decay_t<Args>*, nullptr>::Get(std::get<Index>(InArgs))), ...);

			return Array;
		}
		else
		{
			return nullptr;
		}
	}

	static auto DefaultReturn()
	{
		if constexpr (!std::is_void_v<Result>)
		{
			return Result{};
		}
	}
};
