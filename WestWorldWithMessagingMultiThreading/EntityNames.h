#ifndef NAMES_H
#define NAMES_H

#include <string>

enum 
{
  ent_Miner_Bob,
  ent_Elsa,
  ent_Boozer
};

inline std::string GetNameOfEntity(int n)
{
  switch(n)
  {
  case ent_Miner_Bob:

    return "Miner Bob";

  case ent_Elsa:
    
    return "Elsa"; 

  case ent_Boozer:

	  return "Boozer";

  default:

    return "UNKNOWN!";
  }
}

#endif