
#include <algorithm>
#include <functional>
#include <memory>
#include <client.h>
#include <utilities/macros.h>

using asio::ip::udp;

LocalClient::LocalClient(IChannel *remote) {
  _remote = remote;
}

LocalClient::LocalClient() {
  _remote = NULL;
}

void LocalClient::Send(MessagePtr pmsg) {
  if (_remote == NULL)
    return;
  _remote->QueueMessage(pmsg);
}

NetClient::NetClient(ConnectionParams p) :
  _socket(_io_service,
          udp::endpoint(udp::v4(), p.local_port)),
  _network_thread(std::bind(&NetClient::_Run, this)) {
  _connection_params = p;
}

NetClient::NetClient() :
  _connection_params({
    "localhost",
    "64000",
    60000 + (rand() % 4000),
  }),
  _socket(_io_service,
          udp::endpoint(udp::v4(),
                        _connection_params.local_port)),
  _network_thread(std::bind(&NetClient::_Run, this)) {
}

NetClient::~NetClient() {
  _io_service.stop();
  _network_thread.join();
}

void NetClient::Send(MessagePtr payload) {
  uint8_t msg_buffer[MAX_MSG_LENGTH];
  ByteBufferPtr buffer;
  payload->ToBuffer(buffer);
  if (buffer->size() >= MAX_MSG_LENGTH) {
    Log::Print("%s: Message too long\n", __func__);
    return;
  }
  std::copy(std::begin(*buffer),
            std::end(*buffer),
            &msg_buffer[0]);
  _socket.send_to(asio::buffer(msg_buffer, buffer->size()),
                  _client_endpoint);

  _sent_bytes += buffer->size();
  _sent_messages += 1;
}

void NetClient::Send(std::string payload) {
  uint8_t msg_buffer[MAX_MSG_LENGTH];
  if (payload.length() >= MAX_MSG_LENGTH) {
    Log::Print("%s: Message too long\n", __func__);
    return;
  }
  std::copy(std::begin(payload),
            std::end(payload),
            &msg_buffer[0]);
  _socket.send_to(asio::buffer(buffer, payload.length()),
                  _client_endpoint);

  _sent_bytes += payload.length();
  _sent_messages += 1;
}

bool NetClient::Connect(IChannel *remote) {
  UNUSED(remote);
  return Connect();
}

bool NetClient::Connect() {
  _received_bytes = _sent_bytes = _received_messages = _sent_messages = 0;
  udp::resolver resolver(_io_service);
  udp::resolver::query query(udp::v4(),
                             _connection_params.server_name,
                             _connection_params.server_port);
  _client_endpoint = *resolver.resolve(query);
  return true; //TODO: Check connection status/handshake
}

bool NetClient::Connected() {
  return true; //TODO: Heartbeat/keepalive
}

void NetClient::_Start() {
  _socket.async_receive_from(asio::buffer(_message_buffer, MAX_MSG_LENGTH),
                             _remote_endpoint,
                             std::bind(&NetClient::_OnReceive,
                                       this,
                                       std::placeholders::_1));
}

void NetClient::_OnReceive(std::size_t bytes_transferred) {
  //TODO Flat buffer integration
  MessagePtr message = std::make_shared<ByteMessage>();
  ByteBufferPtr msg_buff = std::make_shared<byte_buffer>();

  std::copy(&_message_buffer[0],
            &_message_buffer[bytes_transferred],
            std::begin(*msg_buff));

  message->FromBuffer(msg_buff);
  QueueMessage(message);
  _received_bytes += bytes_transferred;
  _received_messages++;
}

void NetClient::_Run() {
  _Start();
  while (!_io_service.stopped()) {
    try {
      _io_service.run();
    }
    catch (const std::exception& e) {
      printf("Network exception %s\n", e.what());
    }
    catch (...) {
      printf("Unknown network exception\n");
    }
  }
}

