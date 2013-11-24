#include "common.h"

#include "playlist.h"


Playlist::Iterator::Iterator(const std::vector<std::string>* entries)
	: _entries(entries), _current_index(-1)
{
}

std::string Playlist::Iterator::Next()
{
	// Return empty string if we have reached the end.
	if(End())
		return "";

	return (*_entries)[++_current_index]; // Return the current entry and increment the current index.
}
bool Playlist::Iterator::End() const
{
	return ((_current_index+1) == _entries->size());
}
std::string Playlist::Iterator::SkipTo(int index)
{
	if(index >= 0 && index < _entries->size())
	{
		_current_index = index;
		return (*_entries)[_current_index];
	}

	// No entry found, set iterator to end
	_current_index = _entries->size() - 1;
	return "";
}

int Playlist::Iterator::CurrentIndex() const
{
	return _current_index;
}




Playlist::Playlist()
{
}
Playlist::~Playlist()
{
}

void Playlist::AddEntry(const std::string& file_path)
{
	_entries.push_back(file_path);
}
void Playlist::RemoveEntry(const std::string& file_path)
{
	std::vector<std::string>::iterator it = std::find(_entries.begin(), _entries.end(), file_path);
	if(it != _entries.end())
		_entries.erase(it);
}

void Playlist::Clear()
{
	_entries.clear();
}

std::string Playlist::GetEntry(int index) const
{
	if(index >= 0 && index < _entries.size())
		return _entries[index];

	return ""; // No entry found
}

Playlist::Iterator Playlist::CreateIterator() const
{
	return Iterator(&_entries);
}

