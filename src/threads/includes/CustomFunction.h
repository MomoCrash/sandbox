#pragma once

namespace US {
	template<typename Ret, typename... Args>
	class CustomFunction {
		Ret(*mpMethod)(Args...);
	public:
		CustomFunction(Ret(* pMethod)(Args...));

		void* Ptr();
		Ret Call(Args... args) const;
		Ret operator()(Args... args) const;
	};

	template <typename Ret, typename... Args>
	auto MakeFunction(Ret(*method)(Args...));
}

#include <CustomFunction.hpp>
