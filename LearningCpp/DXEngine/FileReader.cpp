#include <vector>
#include <crtdbg.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <time.h>
#include <windows.h>
#include <ppltasks.h>

using namespace concurrency;

class io_exception : public std::exception
{
	int m_error_code;
public:
	io_exception(int error_code) : m_error_code(error_code) {
		std::wcout << L"error " << error_code << std::endl;
	}
};

class FileReader
{
	static const int default_chunk_size = 32;

	HANDLE										m_Handle;
	PTP_IO										m_tpIo;
	std::vector<byte>							m_data;
	task_completion_event<std::vector<byte>>	m_done;
	size_t										m_chunk_size;
	std::unique_ptr<byte[]>						m_buffer;

	std::function<void(byte*, size_t)>			m_chunk_handler;

	struct io_context
	{
		OVERLAPPED  m_ov;
		FileReader *m_fileReader;
		io_context() {
			ZeroMemory(&m_ov, sizeof(OVERLAPPED));
		}
	}m_io_context;

	static void CALLBACK s_IoCompletionCallback(PTP_CALLBACK_INSTANCE Instance,
												PVOID Context,
												PVOID overlapped,
												ULONG error,
												ULONG_PTR NumberOfBytesTransferred,
												PTP_IO Io)
	{
		io_context* context = (io_context*)overlapped;
		if (error == NO_ERROR) {
			if (context->m_fileReader->m_chunk_handler) {
				context->m_fileReader->m_chunk_handler(context->m_fileReader->m_buffer.get(), NumberOfBytesTransferred);
			}
			// Grow the vector
			context->m_fileReader->m_data.insert(
				context->m_fileReader->m_data.end(),
				context->m_fileReader->m_buffer.get(),
				context->m_fileReader->m_buffer.get() + NumberOfBytesTransferred);
			context->m_ov.Offset += NumberOfBytesTransferred;
			context->m_fileReader->read_next_chunk(&context->m_ov);
		}
		else if (error == ERROR_HANDLE_EOF) {
			// Done reading
			context->m_fileReader->m_done.set(std::move(context->m_fileReader->m_data));
		}
		else {
			auto exp = io_exception(GetLastError());
			std::exception_ptr exp_ptr = std::make_exception_ptr(exp);
			context->m_fileReader->m_done.set_exception(exp_ptr);
		}
	}

	void read_next_chunk(LPOVERLAPPED overlapped)
	{
		StartThreadpoolIo(m_tpIo);
		DWORD numBytesRead;
		if (ReadFile(m_Handle, m_buffer.get(), m_chunk_size, &numBytesRead, overlapped) == FALSE) {
			DWORD error = GetLastError();
			if (error != ERROR_IO_PENDING)
			{
				CancelThreadpoolIo(m_tpIo);
				throw io_exception(error);
			}
		}
	}
public:
	FileReader(std::wstring path, int chunk_size = default_chunk_size)
		: m_chunk_size(chunk_size),
		m_buffer(std::unique_ptr<byte[]>(new byte[chunk_size]))
	{
		m_Handle = CreateFileW(path.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, nullptr);
		if (m_Handle == INVALID_HANDLE_VALUE) {
			throw io_exception(GetLastError());
		}

		m_tpIo = CreateThreadpoolIo(m_Handle, s_IoCompletionCallback, nullptr, nullptr);
		if (m_tpIo == nullptr) {
			throw io_exception(GetLastError());
		}
		m_io_context.m_fileReader = this;
	}

	~FileReader()
	{
		WaitForThreadpoolIoCallbacks(m_tpIo, TRUE);
		CloseThreadpoolIo(m_tpIo);
		CloseHandle(m_Handle);
	}

	task<std::vector<byte>> read_bytes()
	{
		read_next_chunk((LPOVERLAPPED)&m_io_context);
		return create_task(m_done);
	}

	void on_chunk(const std::function<void(byte*, size_t)> chunk_handler)
	{
		m_chunk_handler = chunk_handler;
	}
};

int wmain(int argc, wchar_t** argv)
{
	
		std::wcout << L"Asynchronous read" << std::endl;
		std::vector<task<void>> done;

		for (int i = 2; i<argc; ++i)
		{
			auto fileName = argv[i];
			std::wcout << L"reading " << fileName << std::endl;
			FileReader* reader = new FileReader(fileName);

			auto t = reader->read_bytes().then([=](std::vector<byte> v) {
				std::wcout << L"done reading " << v.size() << L" bytes from '" << fileName << "'." << std::endl;
				delete reader;
			});
			done.push_back(t);
		}
		when_all(done.begin(), done.end()).wait();
	
	return 0;
}