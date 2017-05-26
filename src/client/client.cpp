
#include <client.h>
#include <asio.hpp>

using asio::ip::udp;

LocalClient::LocalClient(IChannel *remote) {
  _connected = false;
  _remote_channel = remote;
}

LocalClient::LocalClient() {
  _connected = false;
  _remote_channel = NULL;
}

void LocalClient::Send(message_ptr payload) {
  _remote_channel->QueueMessage(payload);
}

NetClient::NetClient(ConnectionParams p) :
  _socket(_io_service,
          udp::endpoint(udp::v4(), p.local_port)),
  _network_thread(std::bind(&NetClient::RunService, this)) {
  _connection_params = p;
}
NetClient::NetClient() {
  _connection_params = {
    .server_name = "localhost",
    .server_port = "64000",
    .local_port = 60000 + (rand_r() % 4000);
  }
}

NetClient::~NetClient() {
  _io_service.stop();
  _network_thread.join();
}

void NetClient::Update() {}

void NetClient::Send(message_ptr payload) {
  uint32_t len;
  uint8_t *buffer;
  payload->ToBuffer(buffer, &len);
  _socket.send_to(asio::buffer(buffer, len),
                  _client_endpoint);

  _sent_bytes += bytestring.size();
  _sent_messages += 1;
}

bool NetClient::Connect() {
  _received_bytes = _sent_bytes = _received_messages = _sent_messages = 0;
  udp::resolver resolver(_io_service);
  udp::resolver::query query(udp::v4(),
                             _connection_params.server_name,
                             _connection_params.server_port);
  _server_endpoint = resolver.resolve(query);
  //Send("");
}

bool NecClient::Connected() {
  return socket.connected();
}

void NetClient::StartReceive() {
  _socket.async_receive_from(asio::buffer(_message_buffer, MAX_MSG_LENGTH),
                             _remote_endpoint,
                             std::bind(&NetClient::HandleReceive,
                                       this,
                                       asio::placeholders::bytes_transferred));
}

void NetClient::HandleReceive(std::size_t bytes_transferred) {
  //_message_buffer,
  //_message_buffer + bytes_transferred
  _incoming_queue.push(message);
  _received_bytes += bytes_transferred;
  _received_messages++;
}

void NetClient::RunService() {
  StartReceive();
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

