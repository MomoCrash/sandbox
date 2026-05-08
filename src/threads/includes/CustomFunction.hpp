#pragma once

#include "CustomFunction.h"

namespace US {

	template<typename Ret, typename ...Args>
	auto MakeFunction(Ret(*method)(Args...))
	{
		return CustomFunction<Ret, Args...>(method);
	}

	template<typename Ret, typename ...Args>
	inline CustomFunction<Ret, Args...>::CustomFunction(Ret(*pMethod)(Args...))
		: mpMethod(pMethod) { }

	template<typename Ret, typename ...Args>
	inline void* CustomFunction<Ret, Args...>::Ptr()
	{
		return mpMethod;
	}

	template<typename Ret, typename ...Args>
	inline Ret CustomFunction<Ret, Args...>::Call(Args ...args) const
	{
		return (*mpMethod)(args...);
	}

	template<typename Ret, typename ...Args>
	inline Ret CustomFunction<Ret, Args...>::operator()(Args ...args) const
	{
		return Call(args...);
	}
}