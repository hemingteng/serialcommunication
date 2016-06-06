#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include "SerialClass.h"

using namespace std;

int main() {
    try {
        SerialClass my_sp("COM3");
        my_sp.Start();

        std::vector<string> com{"BEAM 0\n","RESET\n","VER\n","BEAM 1\n","RESET\n"};
        for(string command:com)
        {
            my_sp.WriteToSerial(command);
            //my_sp.CallHandle();

            std::this_thread::sleep_for(std::chrono::milliseconds(10000));
        }

        while (1)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }



    }
    catch (std::exception &e)
    {
        std::cout<<e.what()<<std::endl;
    }


    return 0;
}


