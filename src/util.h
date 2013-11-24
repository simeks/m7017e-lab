#ifndef __UTILITIES_H__
#define __UTILITIES_H__

/// Various smaller utilities
namespace util
{
	/// Returns the file name from the specified file path.
	inline std::string GetFileName(const std::string& file_path);

};


inline std::string util::GetFileName(const std::string& file_path)
{
	// Retrieve the file name from the full file path
	size_t pos = file_path.rfind("/") + 1;
	return file_path.substr(pos);
}

#endif // __UTILITIES_H__
