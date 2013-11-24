#ifndef __PLAYLIST_H__
#define __PLAYLIST_H__

class Playlist
{
public:
	/// Helper-class for iterating through a playlist
	class Iterator
	{
	public:
		Iterator(const std::vector<std::string>* entries);

		/// Returns the next entry in the playlist
		/// This will return an empty string if there's no more entries in the playlist.
		virtual std::string Next();
		
		/// Has the iterator hit the end of the playlist
		virtual bool End() const;

		/// Skips to the specified index
		/// @remark This will return "" and the index will be set to the end of the playlist if index was not found.
		std::string SkipTo(int index);
		
		/// Returns the index to the current entry, returns (-1) if there's no current entry.
		int CurrentIndex() const;


	protected:
		const std::vector<std::string>* _entries;
		int _current_index;

	};


public:
	Playlist();
	~Playlist();

	void AddEntry(const std::string& file_path);
	void RemoveEntry(const std::string& file_path);

	/// Clears all entries from the playlist
	void Clear();

	/// Returns the entry at the specified index.
	/// @remark Returns empty string if there was no entry at the specified index.
	std::string GetEntry(int index) const;

	/// Creates an iterator for iterating through the playlist entries.
	Iterator CreateIterator() const;

private:
	std::vector<std::string> _entries;

};


#endif // __PLAYLIST_H__

