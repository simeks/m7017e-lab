#ifndef __CALLBACKHANDLER_H__
#define __CALLBACKHANDLER_H__

class ConfigValue;

/// @brief Helper class for handling method callbacks.
template<typename T> class MessageCallbackHandler
{
public:
	typedef std::string MessageType;
	typedef void (T::*MessageCallback)(const ConfigValue&);

	/// @param self This is the class instance that should receive the callbacks.
	MessageCallbackHandler(T* object) : _object(object) {}
	~MessageCallbackHandler() {}

	/// Registers a callback for the specified message type.
	/// @param msg_type The message type that should invoke this callback.
	/// @remark This will overwrite any previous callback for the specified message type.
	void RegisterCallback(MessageType msg_type, MessageCallback callback);

	/// Invokes the message callback for the specified type.
	/// @param msg_object The argument to pass to the callback.
	void InvokeCallback(MessageType msg_type, const ConfigValue& msg_object);

private:
	T* _object;
	std::map<MessageType, MessageCallback> _callbacks;
};



template<typename T>
void MessageCallbackHandler<T>::RegisterCallback(MessageType msg_type, MessageCallback callback)
{
	_callbacks[msg_type] = callback;
}

template<typename T>
void MessageCallbackHandler<T>::InvokeCallback(MessageType msg_type, const ConfigValue& msg_object)
{
	// Call any callbacks notifying them about the new message
	std::map<std::string, MessageCallback>::iterator it = _callbacks.find(msg_type);
	if (it != _callbacks.end())
	{
		// Callback found, invoke it
		(_object->*(it->second))(msg_object);
	}
}

#endif // __CALLBACKHANDLER_H__
