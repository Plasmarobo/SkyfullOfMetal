#ifndef CLIENT_H
#define CLIENT_H

#include <mutex>
#include <string>
#include <thread>
#include <utilities/channel.h>
#include <utilities/macros.h>

using asio::ip::udp;

// Simple local client
class LocalClient : public IChannel {
 public:
  LocalClient();
  explicit LocalClient(IChannel *remote);

  void Send(MessagePtr pmsg) override;

  bool Connect(IChannel *remote) override { _remote = remote; return true; }
  bool Connected() override { return _remote != NULL; }
 protected:
  IChannel *_remote;
};

typedef struct {
  std::string server_name;
  std::string server_port;
  uint16_t local_port;
} ConnectionParams;

class NetClient : public IChannel {
 public:
  NetClient();
  explicit NetClient(ConnectionParams p);
  ~NetClient();

  bool Connect(IChannel *remote) override;
  bool Connect();
  bool Connected() override;

  void Send(MessagePtr pmsg) override;
  void Send(std::string buffer);

 protected:
  void _Start();
  void _Run();
  void _OnReceive(std::size_t bytes_transferred);
  ConnectionParams _connection_params;
  asio::io_service _io_service;
  uint16_t _port;
  udp::socket _socket;
  udp::endpoint _client_endpoint;
  udp::endpoint _remote_endpoint;
  static const uint32_t MAX_MSG_LENGTH = 1024;
  uint8_t _message_buffer[MAX_MSG_LENGTH];
  std::thread _network_thread;
  std::mutex _message_lock;

  uint64_t _received_bytes;
  uint32_t _received_messages;
  uint64_t _sent_bytes;
  uint32_t _sent_messages;
};

#endif /* CLIENT_H */
