#pragma once

#include "Windows.h"

#include "cstdint"
#include "string"

#ifndef NDEBUG
#define DBG_INIT		US::Debugger::Init
#define LOG_INFO		*US::Debugger::Get(US::DBG_INFO)			<< "[INFO       ] "
#define LOG_SYSTEM		*US::Debugger::Get(US::DBG_SYSTEM)		<< "[SYSTEM     ] "
#define LOG_WARNING		*US::Debugger::Get(US::DBG_WARNING)		<< "[WARNING    ] "
#define LOG_PERFORMANCE *US::Debugger::Get(US::DBG_PERFORMANCE)	<< "[PERFORMANCE] "
#define LOG_ERROR		US::Debugger::Get(US::DBG_ERROR)->RegisterError(__LINE__, __FUNCTION__, __FILE__)	<< "[ERROR      ] "
#define DBG_UNINIT		US::Debugger::UnInit
#define ASSERT(condition)	if((condition) == false)
#else
#define DBG_INIT		if(true) {} else US::Debugger::Init
#define LOG_INFO		if(true) {} else *US::Debugger::Get(US::DBG_INFO)			<< "[INFO       ] "
#define LOG_SYSTEM		if(true) {} else *US::Debugger::Get(US::DBG_SYSTEM)		<< "[SYSTEM     ] "
#define LOG_WARNING		if(true) {} else *US::Debugger::Get(US::DBG_WARNING)		<< "[WARNING    ] "
#define LOG_PERFORMANCE if(true) {} else *US::Debugger::Get(US::DBG_PERFORMANCE)	<< "[PERFORMANCE] "
#define LOG_ERROR		if(true) {} else US::Debugger::Get(US::DBG_ERROR)->RegisterError(__LINE__, __FUNCTION__, __FILE__)	<< "[ERROR      ] "
#define DBG_UNINIT		if(true) {} else US::Debugger::UnInit
#define ASSERT(condition) if(false)
#endif // !NDEBUG

#define ENDL			US::GPC_ENDL()

namespace US {
	enum DEBUGGER_FLAGS
	{
		DBG_NONE = 0b00000000,

		DBG_INFO		= 0b00000001,
		DBG_SYSTEM		= 0b00000010,
		DBG_WARNING		= 0b00000100,
		DBG_PERFORMANCE = 0b00001000,
		DBG_ERROR		= 0b00010000,

		DBG_ALL = 0b11111111,
	};

	enum DEBUGGER_OUTPUT
	{
		OUTPUT_CONSOLE	= 0b00000001,
		OUTPUT_DEBUGGER = 0b00000010, 
		OUTPUT_LOGS		= 0b00000100,

		OUTPUT_ALL		= 0b11111111,
	};

	struct DEBUGGER_DESCRIPTOR
	{
		uint32_t Flags;
		uint32_t Output;
	};

	struct GPC_ENDL {
		static const char Symbol = '\n';
	};

	class Debugger {

		struct ERROR_DESCRIPTOR {
			std::string ErrorMsg = {};
			uint64_t Line = 0;
			const char* FunctionName = "";
			const char* FileName = "";
		};

		bool mIsInit = false;

		bool mCurrentValid = false;
		DEBUGGER_FLAGS mCurrentFlag = DBG_NONE;

		DEBUGGER_DESCRIPTOR mDescriptor;

		FILE* pFile = nullptr;

		ERROR_DESCRIPTOR* mErrors;
		uint8_t mErrorIndex = 0;

		Debugger();
		~Debugger();

		void DisplayError(uint32_t index);

		void OutputString(const char* txt);
		void OutputString(const char* txt, uint64_t length);

		static bool CheckBitMask(uint64_t mask, uint64_t submask);

	public:
		static Debugger* Get(DEBUGGER_FLAGS flags);
		static bool Init(uint32_t flags, uint32_t output);
		static bool Init(DEBUGGER_DESCRIPTOR& descriptor);
		static void UnInit();

		Debugger& operator << (const char* other);
		Debugger& operator << (const std::string& other);
		Debugger& operator << (const std::string_view& other);
		Debugger& operator << (char other);

		Debugger& operator << (float other);
		Debugger& operator << (double other);

		Debugger& operator << (uint8_t other);
		Debugger& operator << (uint16_t other);
		Debugger& operator << (uint32_t other);
		Debugger& operator << (uint64_t other);

		Debugger& operator << (int8_t other);
		Debugger& operator << (int16_t other);
		Debugger& operator << (int32_t other);
		Debugger& operator << (int64_t other);

		Debugger& operator << (GPC_ENDL other);

		Debugger& RegisterError(uint64_t line, const char* functionName, const char* fileName);
	};
}