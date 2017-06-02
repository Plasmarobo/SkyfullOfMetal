#ifndef CHANNEL_H
#define CHANNEL_H

#include <utilities/log.h>

#include <memory>
#include <queue>

typedef std::vector<uint8_t> ByteBuffer;
typedef std::shared_ptr<ByteBuffer> ByteBufferPtr;

class Messagable {
 public:
  virtual void ToBuffer(ByteBufferPtr buffer) = 0;
  virtual void FromBuffer(ByteBufferPtr buffer) = 0;
};

class ByteMessage : public Messagable {
 public:
  ByteMessage();
  explicit ByteMessage(ByteBufferPtr buffer);

  void ToBuffer(ByteBufferPtr buffer) override;
  void FromBuffer(ByteBufferPtr buffer) override;

 protected:
  ByteBufferPtr _bytes;
};

typedef std::shared_ptr<Messagable> MessagePtr;

class IChannel {
 public:
  virtual MessagePtr NextMessage();

  virtual void QueueMessage(MessagePtr pmsg);

  virtual uint32_t MessageCount();

  virtual void Send(MessagePtr pmsg) = 0;
  virtual bool Connect(IChannel *remote) = 0;
  virtual bool Connected() = 0;

 protected:
  std::queue<MessagePtr> _messages;
};

#endif /* CHANNEL_H */
