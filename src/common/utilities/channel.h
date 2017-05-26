#ifndef CHANNEL_H
#define CHANNEL_H

#include <queue>

class Messagable {
 public:
  virtual void ToBuffer(uint8_t *buffer, uint32_t* length) = 0;
  virtual void FromBuffer(uint8_t *buffer, uint32_t length) = 0;
};

typedef std::smart_ptr<Messagable> message_ptr;

class IChannel {
 public:
  virtual message_ptr NextMessage() {
    if (_messages.empty()) {
      Log::Error("No next element", __func__);
      return NULL;
    }
    message_ptr ret = _messages.front();
    _messages.pop();
    return ret;
  }

  virtual void QueueMessage(message_ptr pmsg) {
    _messages.push(pmsg);
  }

  virtual uint32_t MessageCount() {
    return _messages.size();
  }

  virtual void Send(message_ptr pmsg) = 0;
  virtual bool Connect(IChannel *remote) = 0;
  virtual bool Connected() = 0;

 protected:
  std::queue<message_ptr> _messages;
};

enum MessageType {
  // Information
  MESSAGE = 0,
  // Stream States, stream ID, payload
  STREAM_START,
  STREAM_DATA,
  STREAM_END,
  // Data structures
  ARRAY,
  // Move or set the client viewport
  VIEWPORT,
  // Create an object on the map
  CREATE,
  // Update an object on the map
  UPDATE,
  // Remove an object on the map
  DELETE,
  // Describe an object on the map
  QUERY,
  // State stream
  SYNC_STATE,
  INVALID_MESSAGE = 255,
};

class Message {
 public:
  Message(uint8_t type, uint32_t id, uint8_t *bytes, uint32_t len);

 protected:
  uint8_t _type;
  uint32_t _id;
  const HEADER_LENGHT = 40;
  uint8_t *_bytes;
  uint32_t length;
};

#endif /* CHANNEL_H */
