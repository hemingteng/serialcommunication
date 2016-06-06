//
// Created by heming on 16-6-1.
//
#include "SerialClass.h"
#include <iostream>

SerialClass::SerialClass(const std::string device_name)
{

    _is_requested = true;
    _is_running = false;
    _is_processed=true;
    _Pserial_port = new serial_port(_m_ios);


    if (_pthread == nullptr)
        _pthread = std::make_shared<std::thread>(&SerialClass::ReadAsyn,this);

    if (_Pserial_port)
    {

        _Pserial_port->open(device_name);
        if (_err) {
            std::cout << "error in port open " << _err.message() << std::endl;
        }

        _Pserial_port->set_option(boost::asio::serial_port::baud_rate(115200));
        _Pserial_port->set_option(boost::asio::serial_port::parity(boost::asio::serial_port::parity::none));
        _Pserial_port->set_option(boost::asio::serial_port::stop_bits(boost::asio::serial_port::stop_bits::one));
        _Pserial_port->set_option(boost::asio::serial_port::character_size(boost::asio::serial_port::character_size(8)));
        _Pserial_port->set_option(boost::asio::serial_port::flow_control(boost::asio::serial_port::flow_control::none));

    }


}

SerialClass::~SerialClass() {
    if (_Pserial_port) {
        delete _Pserial_port;
    }
    _is_requested = false;
    _pthread->join();
}

bool SerialClass::init_port(const std::string port, const unsigned int char_size)
{
    if (!_Pserial_port)
        return false;

    _Pserial_port->open(port);
    if (_err) {
        std::cout << "error in port open " << _err.message() << std::endl;
        return false;
    }
    _Pserial_port->set_option(serial_port::baud_rate(115200), _err);//
    _Pserial_port->set_option(serial_port::flow_control(serial_port::flow_control::none), _err);
    _Pserial_port->set_option(serial_port::parity(serial_port::parity::none), _err);
    _Pserial_port->set_option(serial_port::stop_bits(serial_port::stop_bits::one), _err);
    _Pserial_port->set_option(serial_port::character_size(8), _err);


    if (_err) {
        std::cout << "error in set_option" << _err.message() << std::endl;

        return false;
    }


    return true;
}

bool SerialClass::WriteToSerial(const std::string &data) {

    if(IsOpen())
    {
        std::string temp_data=data;
        std::cout<<"write to serial: "<<data<<std::endl;
        if (data.find("\n") == std::string::npos)
        {
            std::cout<<"not find n"<<std::endl;
            temp_data = data + "\n";
        }

        write(*_Pserial_port, buffer(temp_data.c_str(), temp_data.size()));

        return true;
    }

    return false;
}

bool SerialClass::IsOpen() {
    return _Pserial_port->is_open();
}




void SerialClass::ReadAsyn()
{
    while(_is_requested)
    {
        if(_is_running) {
            if(_is_processed) {

                _m_ios.reset();
                
                async_read_until(*_Pserial_port, _buf, "Enter your CMD:",
                                              boost::bind(&SerialClass::HandleRead, this,
                                                          boost::asio::placeholders::error));
                _is_processed=false;

                _m_ios.run();
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));

        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
}

void SerialClass::HandleRead(const boost::system::error_code &err)
{
    if(!err)
    {
        auto n=_buf.size();

        boost::asio::streambuf::const_buffers_type bufs_sb = _buf.data();
        std::string mes(boost::asio::buffers_begin(bufs_sb), boost::asio::buffers_begin(bufs_sb) + n);

        _buf.consume(n);

        std::size_t pos = mes.find("angle:");
        if (pos != std::string::npos) {

            std::string angle_from = mes.substr(pos + 5, pos + 8);
            if (cb != nullptr) {
                cb(angle_from);
            }
            std::cout << "##########################" << std::endl;
            std::cout << "wake up angle:" << angle_from << std::endl;
            std::cout << "##########################" << std::endl;
        }

        //std::string mes = message;
        if (mes.find("SUCESS") != std::string::npos) {
            std::cout << "command is ok" << std::endl;
        }
        if (mes.find("ERROR") != std::string::npos) {
            std::cout << "command is ok" << std::endl;
        }

        std::cout << "*************************************" << std::endl;
        std::cout << mes << std::endl;
        std::cout << "*************************************" << std::endl;

        _is_processed=true;

    }
}

void SerialClass::Close()
{
    if(IsOpen())
    {
        _Pserial_port->cancel(_err);
        _Pserial_port->close(_err);
    }

    _m_ios.reset();
    _m_ios.stop();
    
    return ;
}

void SerialClass::Start() {
    if (_is_running)
        return;
    _is_running = true;
}

void SerialClass::Stop() {
    if (!_is_running)
        return;
    _is_running = false;
}


