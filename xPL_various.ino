boolean isXpl( uint8_t *buffer)
{
  return ( buffer[IP_PROTO_P] == IP_PROTO_UDP_V
            && buffer[UDP_DST_PORT_L_P] == XPL_PORT_L
            && buffer[UDP_DST_PORT_H_P] == XPL_PORT_H);
}

// convert byte* to char*
void ByteToChar(byte* bytes, char* chars, unsigned int count)
{
    int j = 0;
  
    for(unsigned int i = UDP_DATA_P; i < count && i < XPL_MESSAGE_BUFFER_MAX + UDP_DATA_P - 1; i++)
    {
    	chars[j++] = (char)bytes[i];
    }
    chars[j] = 0;
}

void SendUdPMessage(char *buffer)
{
    ether.sendUdp (buffer, strlen(buffer), xpl.udp_port, broadcast, xpl.udp_port);
}

void SensorChange(int reading, int *current, char *device)
{
     xPL_Message msg;

     msg.hop = 1;
     msg.type = XPL_TRIG;

     msg.SetTarget_P(PSTR("*"));
     msg.SetSchema_P(PSTR("sensor"), PSTR("basic"));

     msg.AddCommand("device", device);
     msg.AddCommand_P(PSTR("type"), PSTR("input"));
     msg.AddCommand_P(PSTR("current"), reading == 0 ? PSTR("low") : PSTR("high"));
    
     xpl.SendMessage(&msg, true);
     *current = reading;
}

