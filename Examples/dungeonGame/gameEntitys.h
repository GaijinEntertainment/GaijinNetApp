#pragma once


struct Player
{
  int hp = 100;
  int exp = 0;
  int kills = 0;
  int level = 0;
};


struct Orc
{
  int hp = 20;
  int gen_exp = 1;
  const char * description = "small orc";
};


struct SessionStats
{
  int exp = 0;
  int kills = 0;
};
