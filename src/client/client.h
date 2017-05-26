#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <utility/channel.h>
#define ASIO_STANDALONE 1
#define ASIO_HEADER_ONLY 1
#include <asio/include/asio.hpp>

// Simple local client
class LocalClient : public IChannel {
 public:
  LocalClient();

  void Send(message_ptr pmsg) {
    if (_remote == NULL)
      return;
    _remote->QueMessage(pmsg);
  }

  bool Connect(IChannel *remote) override { _remote = remote; }
  bool Connected() override { return _remote != NULL; }
}

typedef struct {
  std::string server_name;
  std::string server_port;
  uint16_t local_port;
} ConnectionParams;

class NetClient : public IChannel {
 public:
  explicit NetClient(ConnectionParams p);

  bool Connect(IChannel *remote) override;
  bool Connected() override;

  void QueueMessage(message_ptr pmsg) override;
  void Send(message_ptr pmsg) override;
  void Send(std::string buffer);

 protected:
  void _Start();
  void _Run();
  void _End();
  void _OnReceive();
  asio::io_service _io_service;
  uint16_t _port;
  udp::socket _socket;
  upd::endpoint _client_endpoint;
  upd::endpoint _remote_endpoint;
  const MAX_MSG_LENGTH = 1024;
  uint8_t _message_buffer[MAX_MSG_LENGTH];
  std::thread _network_thread;
  std::mutex _message_lock;

  std::queue<Messagable> _incoming_queue;
  uint64_t _received_bytes;
  uint32_t _received_messages;
  uint64_t _sent_bytes;
  uint32_t _sent_messages;
}

#endif /* CLIENT_H */
