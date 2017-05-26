#ifndef SKYFULL_SERVER_H
#define SKYFULL_SERVER_H

#include <string>
#include <utility/channel.h>
#define ASIO_STANDALONE 1
#define ASIO_HEADER_ONLY 1
#include <asio/include/asio.hpp>

// Simple local server
class LocalServer : public IChannel {
 public:
  LocalServer();

  void Send(message_ptr pmsg) {
    if (_remote == NULL)
      return;
    _remote->QueueMessage(pmsg);
  }

  bool Connect(IChannel *remote) override { _remote = remote; }
  bool Connected() override { return _remote != NULL; }
};

// ASIO network server
class NetServer : public IChannel {
 public:
  explicit NetServer(uint16_t port);

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
  udp::endpoint _server_endpoint;
  udp::endpoint _remote_endpoint;
  const MAX_MSG_LENGTH = 1024;
  uint8_t _message_buffer[MAX_MSG_LENGTH];
  std::thread _network_thread;
  std::mutex _messages_lock;
};

#endif