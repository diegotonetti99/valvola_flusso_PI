#ifndef MOTOR_H
#define MOTOR_H

class Motor
{
private:
  int dir;
  int step;
  int en;
  int steps;
  int microstepping;
  long pos;
  int dt; // in microseconds -> subtime interval
  long DT; // in microseconds -> overall time delta
public:
  Motor(int dir, int step, int en, int microstepping, int steps, float rpm, int DT)
  {
    this->dir = dir;
    this->step = step;
    this->en = en;
    this->steps = steps;
    this->microstepping = microstepping;
    this->pos = 0;
    this->DT = DT * 1000l;
    setSpeed(rpm);
    pinMode(this->dir, OUTPUT);
    pinMode(this->step, OUTPUT);
    pinMode(this->en, OUTPUT);
  }
  move(long target)
  {
    long delta = target - this->pos;
    int s = sign(delta);
    delta = abs(delta);
    long time = 0;
    while (time < this->DT)
    {
      if (delta > 0)
      {
        digitalWrite(this->dir, s);
        digitalWrite(this->step, 0);
        delayMicroseconds(this->dt);
        digitalWrite(this->step, 1);
        delayMicroseconds(this->dt);
        delta -= 1;
        this->pos += -pow(-1, s);
        /*
        Serial.print(target);
        Serial.print(",");
        Serial.println(this->pos);
        */
      }
      else
      {
        delayMicroseconds(2 * this->dt);
      }
      time += 2 * dt;
    }
  }
  void setSpeed(float rpm)
  {
    this->dt = int(60.0f * 500000.0f / float(this->microstepping * this->steps) / rpm); // microsec/step
  }
  void enable()
  {
    digitalWrite(this->en, 0);
  }
  void disable()
  {
    digitalWrite(this->en, 1);
  }
  int sign(int x)
  {
    if (x >= 0)
    {
      return 1;
    }
    return 0;
  }
  long getPos(){
    return this->pos;
  }
};

#endif
