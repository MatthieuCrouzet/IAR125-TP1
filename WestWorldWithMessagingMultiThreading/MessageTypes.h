#ifndef MESSAGE_TYPES
#define MESSAGE_TYPES

#include <string>

enum message_type
{
  Msg_HiHoneyImHome,
  Msg_StewReady,
  Msg_ZZZZ,
  Msg_TooMuchNoise,
  Msg_CalmDown,
  Msg_Provocation
};


inline std::string MsgToStr(int msg)
{
  switch (msg)
  {
  case Msg_HiHoneyImHome:
    
    return "HiHoneyImHome"; 

  case Msg_StewReady:
    
    return "StewReady";

  case Msg_ZZZZ:

	  return "Msg_ZZZZ";

  case Msg_TooMuchNoise:

	  return "TooMuchNoise";

  case Msg_CalmDown:

	  return "CalmDown";

  case Msg_Provocation:

	  return "Provocation";

  default:

    return "Not recognized!";
  }
}

#endif