#pragma once

class audioManager {

public:

   audioManager() {}

  int initEngine(int id);
  void uninitEngine();
  void playMeow();
  void playRoost();
  void playCollect();
  void playPowerup();

private:

};