#pragma once


#include<streambuf>
#include<vector>
#include<fstream>
#include<string>
#include<cstdint>
#include<map>

namespace pixelengine {
	namespace _resource_ {
		struct ResourceBuffer :public std::streambuf
		{
			ResourceBuffer(std::ifstream& ifs, uint32_t offset, uint32_t size);
			std::vector<char> vMemory;
		};
		class ResourcePack
		{
		private:
			std::ifstream baseResourceFile;
			//Mappig each file in the pack using their size and offset
			struct resourceFile { uint32_t size; uint32_t offset; };
			std::map<std::string, resourceFile>filesMap;
			std::vector<char> scramble(const std::vector<char>& data, const std::string& key);
			std::string makeposix(const std::string& path);
		public:
			ResourcePack() = default;
			~ResourcePack();
			//#FUNC: add a file to resources
			bool addFile(const std::string& fileString);
			//#FUNC: load a resourcepack file
			bool loadPack(const std::string& fileString, const std::string& keyString);
			//#FUNC: save a resourcepack to disk
			bool savePack(const std::string& fileString, const std::string& keyString);

			pixelengine::_resource_::ResourceBuffer getFileBuffer(const std::string& fileString);
			//#FUNC: check for errors
			bool OK();
		};
	};
	
};

