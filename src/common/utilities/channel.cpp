
#include <algorithm>
#include <array>
#include <memory>
#include <queue>
#include <utilities/channel.h>

ByteMessage::ByteMessage() {
  _bytes = std::make_shared<ByteBuffer>();
}

ByteMessage::ByteMessage(ByteBufferPtr buffer) {
  this->FromBuffer(buffer);
}

void ByteMessage::ToBuffer(ByteBufferPtr buffer) {
  buffer = ByteBufferPtr(_bytes);
};


void ByteMessage::FromBuffer(ByteBufferPtr buffer) {
  _bytes = ByteBufferPtr(buffer);
}

MessagePtr IChannel::NextMessage() {
  if (_messages.empty()) {
    Log::Print("%s: No next element\n", __func__);
    return NULL;
  }
  MessagePtr ret = _messages.front();
  _messages.pop();
  return ret;
}

void IChannel::QueueMessage(MessagePtr pmsg) {
  _messages.push(pmsg);
}

uint32_t IChannel::MessageCount() {
  return _messages.size();
}
