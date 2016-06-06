//
// Created by heming on 16-6-1.
//

#ifndef SERIALCOMMUNICATION_SERIALCLASS_H
#define SERIALCOMMUNICATION_SERIALCLASS_H

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <string>
#include <thread>
#include <functional>
#include <atomic>
#include <memory>
#include <chrono>

using namespace boost::asio;


class SerialClass {
private:
    bool init_port(const std::string port, const unsigned int char_size);

public:
    SerialClass(const std::string device_name);

    ~SerialClass();

    bool WriteToSerial(const std::string &data);

    void ReadFromSerial();

    void Close();

    void Start();

    void Stop();

    bool IsOpen();

    void HandleRead(const boost::system::error_code& err);

    void ReadAsyn();

    void CallHandle();
private:
    boost::asio::io_service _m_ios;

    boost::asio::serial_port* _Pserial_port;

    std::string _m_port;

    boost::system::error_code _err;

    std::shared_ptr<std::thread> _pthread;

    std::atomic_bool _is_running;

    std::atomic_bool _is_requested;

    std::function<void(std::string)> cb;

    boost::asio::streambuf _buf;

    std::atomic_bool _is_processed;


};


#endif //SERIALCOMMUNICATION_SERIALCLASS_H
