#pragma once

#include "sims.h"
#include "core/vfs.h"
using namespace sims;

inline void gen_ue4_stdlib()
{
	VFS::Init();

	auto inputS = VFS::GetVFS().OpenInputStream("G:\\ue4_stdlib.metal");
	Buffer buffer = inputS->Read();
	LOG_INFO("buffer size:%d", buffer.GetSize());

	buffer.RemoveIf([](char ch) { return ch == 0x0D; }); // remove CR, keep LF
	LOG_INFO("buffer size after remove CR:%d", buffer.GetSize());

	auto outputS = VFS::GetVFS().OpenOutputStream("G:\\ue4_stdlib.h");
	{
		outputS->Write("unsigned char ue4_stdlib_metal[] = {\r\n  ");
		const int32 NEXTLINE = 12;
		int32 bufferSize = buffer.GetSize();
		for (int32 index = 0; index < bufferSize; ++index)
		{
			bool bLF = ((index + 1) % NEXTLINE == 0);
			bool bLast = (index + 1 == bufferSize);

			const char* s = bLast ? str_format("0x%02x", buffer[index]) : str_format("0x%02x,", buffer[index]);
			outputS->Write(s);
			if (bLF)
				outputS->Write("\r\n  ");
			else if (!bLast)
				outputS->Write(" ");
		}
		outputS->Write("\r\n};\r\n");
		outputS->Write(str_format("unsigned int ue4_stdlib_metal_len = %u;\r\n", buffer.GetSize()));
	}
	outputS->Close();

	VFS::Shutdown();
}
