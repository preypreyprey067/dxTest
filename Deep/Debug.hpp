#ifndef DEEP_DEBUG_HPP
#define DEEP_DEBUG_HPP

#include <Windows.h>

#include <iostream>

namespace deep
{
	namespace Console
	{
		template<typename T>
		void Log(const T& msg)
		{
#if _DEBUG
			std::cout << "MESSAGE: " << msg << "\n";
#endif // _DEBUG
		}

		template<typename T>
		void LogWarning(const T& msg)
		{
#if _DEBUG
			static HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		
			SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			std::cout << "WARNING: ";
			SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			std::cout << msg;
#endif
		}

		template<typename T>
		void LogError(const T& msg)
		{
#if _DEBUG
			static HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

			SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_INTENSITY);
			std::cout << "ERROR: ";
			SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			std::cout << msg;
#endif
		}
	}
}

#endif // DEEP_DEBUG_HPP