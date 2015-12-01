#ifndef BTDEVICE_H
#define BTDEVICE_H

#include <Arduino.h>


namespace BTCommand
{
	enum Command
	{
		AutoToggle, Forward, Reverse, TurnLeft, TurnRight, Stop, StopAll
	};
}

class BTDevice
{
public:
	SoftwareSerial bTSerial;
public:

	//Constructor
	BTDevice(int rxPin, int txPin):bTSerial(rxPin,txPin)
	{    
	}

    void init(long baud)
    {
      //  bTSerial.end();
        bTSerial.begin(baud);
    }

	bool getCommand(BTCommand::Command &command)
	{
        //Serial.println(bTSerial.available());
		String msg = "";
		bool beginRead = false;
		char c;
		//Drop symbols until we read a { for start of message
		while(bTSerial.available() > 0 && beginRead == false)
		{
			c = bTSerial.read();
            Serial.println(c);
            if(c == '!')
            {
                command = BTCommand::StopAll;
                return true;
            }
			if(c == '{')
				beginRead = true;
		}

		//Read until end of message symbol }
		while(bTSerial.available() > 0 && c != '}')
		{
			c = bTSerial.read();
            if(c != '}')
			    msg += c;
		}

        if(c != '}')
            msg = "5"; //bad data use stop
            
		//Properly formatted message received and 1 char long
		if(c == '}' && msg.length() == 1)
		{
			char cmsg = msg[0];
         
			switch(cmsg)
			{
				case '0':
                    command = BTCommand::AutoToggle;
					return true;
				case '1':
                    command = BTCommand::Forward;
					return true;
				case '2':
					command = BTCommand::Reverse;
                    return true;
				case '3':
					command = BTCommand::TurnLeft;
                  return true;
				case '4':
					command = BTCommand::TurnRight;
                    return true;
                case '5':
                    command = BTCommand::Stop;
                    return true;
			}
		} else
		{
			return false;
		}
	}

	int available()
	{
		return (bTSerial.available());
	}

};


#endif

