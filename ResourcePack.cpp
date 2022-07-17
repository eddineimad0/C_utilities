#include "ResourcePack.h"
#include "../platform_interface/config.h"

using namespace pixelengine;
using namespace _resource_;
ResourceBuffer::ResourceBuffer(std::ifstream& ifs, uint32_t offset, uint32_t size)
{
	this->vMemory.resize(size);
	ifs.seekg(offset);
	ifs.read(vMemory.data(), vMemory.size());
	setg(vMemory.data(), vMemory.data(), vMemory.data() + size);
}


ResourcePack::~ResourcePack()
{
	baseResourceFile.close();
}

bool ResourcePack::addFile(const std::string& fileString)
{
	const std::string nFileString = makeposix(fileString);
	if (FS::exists(nFileString))
	{
		resourceFile f;
		f.size = (uint32_t)FS::file_size(nFileString);
		f.offset = 0;
		this->filesMap[nFileString] = f;
		return true;
	}
	return false;
}

bool ResourcePack::loadPack(const std::string& fileString, const std::string& keyString)
{
	this->filesMap.clear();
	this->baseResourceFile.open(fileString, std::ifstream::binary);
	if (!this->baseResourceFile.is_open())return false;
	uint32_t indexSize = 0;
	this->baseResourceFile.read((char*)&indexSize, sizeof(uint32_t));
	std::vector<char> buffer(indexSize);
	for (uint32_t i = 0; i < indexSize; i++)
		buffer[i] = this->baseResourceFile.get();
	std::vector<char> decoBuffer = scramble(buffer, keyString);
	size_t pos = 0;
	auto copyFromDecoded = [&decoBuffer, &pos](char* dst, size_t size)
	{
		memcpy((void*)dst, (const void*)(decoBuffer.data() + pos), size);
		pos += size;
	};
	auto getCharFromDecoded = [&copyFromDecoded]()->int {char c; copyFromDecoded(&c, 1); return c; };
	uint32_t entries = 0;
	copyFromDecoded((char*)&entries, sizeof(uint32_t));
	for (uint32_t i = 0; i < entries; i++)
	{
		uint32_t filePathSize = 0;
		copyFromDecoded((char*)filePathSize, sizeof(uint32_t));
		std::string fileName(filePathSize, '\0');
		for (uint32_t k = 0; k < filePathSize; k++)
			fileName[k] = getCharFromDecoded();
		resourceFile f;
		copyFromDecoded((char*)&f.size, sizeof(uint32_t));
		copyFromDecoded((char*)&f.offset, sizeof(uint32_t));
		this->filesMap[fileName] = f;
	}
	// Don't close base file! we will provide a stream
	// pointer when the file is requested
	return true;
}

bool ResourcePack::savePack(const std::string& fileString, const std::string& keyString)
{
	std::ofstream outFile(fileString, std::ofstream::binary);
	if (!outFile.is_open())return false;
	uint32_t indexSize = 0;
	outFile.write((char*)&indexSize, sizeof(uint32_t));
	uint32_t mapSize = uint32_t(this->filesMap.size());
	outFile.write((char*)&mapSize, sizeof(uint32_t));
	for (auto& element : this->filesMap)
	{
		size_t pathSize = element.first.size();
		outFile.write((char*)&pathSize, sizeof(uint32_t));
		outFile.write(element.first.c_str(), pathSize);
		outFile.write((char*)element.second.size, sizeof(uint32_t));
		outFile.write((char*)element.second.offset, sizeof(uint32_t));

	}
	std::streampos offset = outFile.tellp();
	indexSize = (uint32_t)offset;
	for (auto& element : this->filesMap)
	{
		element.second.offset = (uint32_t)offset;
		std::vector<uint8_t> buffer(element.second.size);

		std::ifstream inFile(element.first, std::ifstream::binary);
		inFile.read((char*)buffer.data(), element.second.size);
		inFile.close();

		outFile.write((char*)buffer.data(), element.second.size);
		offset += element.second.size;
	}
	// 3) Scramble Index
	std::vector<char> stream;
	auto appendBuffer = [&stream](const char* data, size_t size)
	{
		size_t sizeNow = stream.size();
		stream.resize(sizeNow, +size);
		memcpy(stream.data() + sizeNow, data, size);
	};

	appendBuffer((char*)&mapSize, sizeof(uint32_t));

	for (auto& element : filesMap)
	{

		size_t pathSize = element.first.size();
		appendBuffer((char*)&pathSize, sizeof(uint32_t));
		appendBuffer(element.first.c_str(), pathSize);

		appendBuffer((char*)&element.second.size, sizeof(uint32_t));
		appendBuffer((char*)&element.second.offset, sizeof(uint32_t));
	}
	std::vector<char> indexString = this->scramble(stream, keyString);
	uint32_t indexStringLength = uint32_t(indexString.size());


	outFile.seekp(0, std::ios::beg);
	outFile.write((char*)&indexStringLength, sizeof(uint32_t));
	outFile.write(indexString.data(), indexStringLength);
	outFile.close();
	return true;
}

pixelengine::_resource_::ResourceBuffer ResourcePack::getFileBuffer(const std::string& fileString)
{
	return ResourceBuffer(this->baseResourceFile, this->filesMap[fileString].offset, this->filesMap[fileString].size);
}

bool ResourcePack::OK()
{
	return this->baseResourceFile.is_open();
}

std::vector<char> ResourcePack::scramble(const std::vector<char>& data, const std::string& key)
{
	if (key.empty())return data;
	std::vector<char> out;
	size_t c = 0;
	for (auto s : data)out.push_back(s ^ key[(c++) % key.size()]);
	return out;
}

std::string ResourcePack::makeposix(const std::string& path)
{
	std::string nPath;
	for (auto c : path)nPath += std::string(1, c == '\\' ? '/' : c);
	return nPath;
}
