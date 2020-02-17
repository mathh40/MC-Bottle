#pragma once
class IThreadedFileIO
{
public:
	virtual ~IThreadedFileIO() = default;
	virtual bool writeNextIO() = 0;
};