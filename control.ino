void AfterParseAction(xPL_Message * message)
{
    if (xpl.TargetIsMe(message))
    {
      if (message->IsSchema_P(PSTR("control"), PSTR("basic")))
      {
        int device, device_index;
        int level;
        
        for (int i = 0; i < message->command_count; i++) {
          if (strcmp_P(message->command[i].name, PSTR("device")) == 0) {
            sscanf_P(message->command[i].value, PSTR("%d"), &device);
            device_index = i;
          }
          if (strcmp_P(message->command[i].name, PSTR("current")) == 0)
            level = strcmp_P(message->command[i].value, PSTR("high")) ? 0 : 1;
        }

        if (device > 0 && device < 5) {

          xPL_Message msg;
          
          // Write new state to output
          digitalWrite(9 - device, level);
               
          // Send corresponding xpl-stat message
          msg.hop = 1;
          msg.type = XPL_STAT;

          msg.SetTarget_P(PSTR("*"));
          msg.SetSchema_P(PSTR("sensor"), PSTR("basic"));

          msg.AddCommand("device", message->command[device_index].value);
          msg.AddCommand_P(PSTR("type"), PSTR("output"));
          msg.AddCommand_P(PSTR("current"), digitalRead(9 - device) == 0 ? PSTR("low") : PSTR("high"));
    
          xpl.SendMessage(&msg, true);
        }
      }

      if (message->IsSchema_P(PSTR("sensor"), PSTR("request")))
      {
        int device, device_index;
        int level;
        
        for (int i = 0; i < message->command_count; i++) {
          if (strcmp_P(message->command[i].name, PSTR("device")) == 0) {
            sscanf_P(message->command[i].value, PSTR("%d"), &device);
            device_index = i;
          }
        }

        if (device > 0 && device < 5) {

          xPL_Message msg;
          
          // Send corresponding xpl-stat message
          msg.hop = 1;
          msg.type = XPL_STAT;

          msg.SetTarget_P(PSTR("*"));
          msg.SetSchema_P(PSTR("sensor"), PSTR("basic"));

          msg.AddCommand("device", message->command[device_index].value);
          msg.AddCommand_P(PSTR("type"), PSTR("input"));
          msg.AddCommand_P(PSTR("current"), digitalRead(20 - device) == 0 ? PSTR("low") : PSTR("high"));
    
          xpl.SendMessage(&msg, true);
        }
      }
    }
}

