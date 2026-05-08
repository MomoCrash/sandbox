#include "Debugger.h"

#include <iostream>


namespace US {
	Debugger::Debugger()
	{
		mErrors = new ERROR_DESCRIPTOR[32];
		mDescriptor = {};
		mIsInit = false;
	}

	Debugger::~Debugger()
	{
		if (mIsInit) {
			OutputDebugStringA("[/!\\] Forgot to Uninit Debugger !");
		}
	}

	void Debugger::DisplayError(uint32_t index)
	{
		if (index != 0) *Debugger::Get(DBG_ERROR) << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n";
		*Debugger::Get(DBG_ERROR) << "= Line     : " << mErrors[index].Line << '\n';
		*Debugger::Get(DBG_ERROR) << "= Function : " << mErrors[index].FunctionName << '\n';
		*Debugger::Get(DBG_ERROR) << "= File     : " << mErrors[index].FileName << '\n';
		*Debugger::Get(DBG_ERROR) << "= Msg      : " << '\n';
		*Debugger::Get(DBG_ERROR) << mErrors[index].ErrorMsg.c_str() << '\n';
	}

	void Debugger::OutputString(const char* txt)
	{
		OutputString(txt, strlen(txt));
	}

	void Debugger::OutputString(const char* txt, uint64_t length)
	{
		if (mCurrentValid == false) return;

		if (mCurrentValid && (mCurrentFlag & DBG_ERROR)) {
			mErrors[mErrorIndex].ErrorMsg += txt;
		}
		if (CheckBitMask(mDescriptor.Output, OUTPUT_CONSOLE)) {
			std::cout << txt;
		}
		if (CheckBitMask(mDescriptor.Output, OUTPUT_DEBUGGER)) {
			OutputDebugStringA(txt);
		}
		if (CheckBitMask(mDescriptor.Output, OUTPUT_LOGS)) {
			if (fwrite(txt, sizeof(char), length, pFile) != length) {
				mDescriptor.Output ^= OUTPUT_LOGS;
				OutputString("Failed to write in logs : stopping the log feature");
			}
		}
	}

	bool Debugger::CheckBitMask(uint64_t mask, uint64_t submask) {
		return (mask & submask) == submask;
	}

	Debugger* Debugger::Get(DEBUGGER_FLAGS flag)
	{
		static Debugger debugger;
		debugger.mCurrentFlag = flag;
		debugger.mCurrentValid = CheckBitMask(debugger.mDescriptor.Flags, flag);
		return &debugger;
	}

	bool Debugger::Init(uint32_t flags, uint32_t output)
	{
		Debugger* pDbg = Get(DBG_NONE);
		pDbg->mDescriptor.Flags = flags;
		pDbg->mDescriptor.Output = output;

		if (CheckBitMask(pDbg->mDescriptor.Output, OUTPUT_LOGS)) {
			if (fopen_s(&pDbg->pFile, "Logs.txt", "wb") != 0) {
				pDbg->OutputString("Failed to open Logs");
				return false;
			}
		}

		pDbg->mIsInit = true;
		return true;
	}

	bool Debugger::Init(DEBUGGER_DESCRIPTOR& descriptor)
	{
		Debugger* pDbg = Get(DBG_NONE);
		pDbg->mDescriptor.Flags = descriptor.Flags;
		pDbg->mDescriptor.Output = descriptor.Output;

		if (CheckBitMask(pDbg->mDescriptor.Output, OUTPUT_LOGS)) {
			if (fopen_s(&pDbg->pFile, "Logs.txt", "wb") != 0) {
				pDbg->OutputString("Failed to open Logs");
				return false;
			}
		}

		pDbg->mIsInit = true;
		return true;
	}

	void Debugger::UnInit()
	{
		Debugger* pDbg = Get(DBG_NONE);

		if (pDbg->mErrorIndex > 0) {
			*Debugger::Get(DBG_ERROR) << "============================\n\n";
			for (uint8_t i = 0; i < pDbg->mErrorIndex; ++i) {
				pDbg->DisplayError(i);
			}
			*Debugger::Get(DBG_ERROR) << "============================\n";
		}

		if (CheckBitMask(pDbg->mDescriptor.Output, OUTPUT_LOGS)) {
			fclose(pDbg->pFile);
		}
		delete[] pDbg->mErrors;
		pDbg->mIsInit = false;
	}

	Debugger& Debugger::operator<<(const char* other)
	{
		OutputString(other);
		return *this;
	}

	Debugger& Debugger::operator<<(const std::string& other)
	{
		OutputString(other.c_str());
		return *this;
	}

	Debugger& Debugger::operator<<(const std::string_view& other)
	{
		OutputString(other.data());
		return *this;
	}

	Debugger& Debugger::operator<<(char other)
	{
		CHAR txt[2]{ other, '\0' };
		OutputString(txt, 1);
		return *this;
	}

	Debugger& Debugger::operator<<(float other)
	{
		OutputString(std::to_string(other).c_str());
		return *this;
	}

	Debugger& Debugger::operator<<(double other)
	{
		OutputString(std::to_string(other).c_str());
		return *this;
	}

	Debugger& Debugger::operator<<(uint8_t other)
	{
		OutputString(std::to_string(other).c_str());
		return *this;
	}

	Debugger& Debugger::operator<<(uint16_t other)
	{
		OutputString(std::to_string(other).c_str());
		return *this;
	}

	Debugger& Debugger::operator<<(uint32_t other)
	{
		OutputString(std::to_string(other).c_str());
		return *this;
	}

	Debugger& Debugger::operator<<(uint64_t other)
	{
		OutputString(std::to_string(other).c_str());
		return *this;
	}

	Debugger& Debugger::operator<<(int8_t other)
	{
		OutputString(std::to_string(other).c_str());
		return *this;
	}

	Debugger& Debugger::operator<<(int16_t other)
	{
		OutputString(std::to_string(other).c_str());
		return *this;
	}

	Debugger& Debugger::operator<<(int32_t other)
	{
		OutputString(std::to_string(other).c_str());
		return *this;
	}

	Debugger& Debugger::operator<<(int64_t other)
	{
		OutputString(std::to_string(other).c_str());
		return *this;
	}

	Debugger& Debugger::operator<<(GPC_ENDL other)
	{
		char txt[2]{ other.Symbol, '\0' };
		OutputString(txt, 1);
		if (mCurrentValid && (mCurrentFlag & DBG_ERROR)) {
			++mErrorIndex;
			if (mErrorIndex >= 32) mErrorIndex = 31;
		}
		return *this;
	}

	Debugger& Debugger::RegisterError(uint64_t line, const char* functionName, const char* fileName)
	{
		if (mCurrentValid && CheckBitMask(mDescriptor.Flags, DBG_ERROR)) {
			mErrors[mErrorIndex].Line = line;
			mErrors[mErrorIndex].FunctionName = functionName;
			mErrors[mErrorIndex].FileName = fileName;
		}
		return *this;
	}
}
