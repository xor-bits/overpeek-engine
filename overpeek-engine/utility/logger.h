#pragma once


namespace oe {
	//not in logger
	static enum outType {
		info, warning, critical, error
	};

	class Logger {
	private:
		static void *m_console;
		static bool m_initalized;

	public:

		static void setup();

		static void out(int type, const char *output);
		static void out(int type, double output);
		static void out(int type, const char *output, const char *output2);
		static void out(int type, const char *output, double output2);
		static void out(int type, const char *output, double output2, double output3);
		static void out(int type, const char *output, const char *output2, const char *output3);
		static void out(int type, const char *output, double output2, const char *output3, double output4);

	};
}