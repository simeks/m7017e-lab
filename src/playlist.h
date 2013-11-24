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
		virtual const std::string& Next();
		
		/// Has the iterator hit the end of the playlist
		virtual bool End() const;

	protected:
		const std::vector<std::string>* _entries;
		uint32_t _current_index;

	};

	/// Class for randomly going through a playlist
	class RandomIterator : public Iterator
	{
	public:
		RandomIterator(const std::vector<std::string>* entries);

		virtual const std::string& Next();

		/// The random iterator can never reach the end so this will always return false.
		virtual bool End() const;

	};


public:
	Playlist();
	~Playlist();

	void AddEntry(const std::string& file_path);
	void RemoveEntry(const std::string& file_path);

	/// Clears all entries from the playlist
	void Clear();

	/// Creates an iterator for iterating through the playlist entries.
	Iterator CreateIterator() const;

	/// Creates an random iterator for randomly selecting entries from the playlist.
	RandomIterator CreateRandomIterator() const;

private:
	std::vector<std::string> _entries;

};


#endif // __PLAYLIST_H__

