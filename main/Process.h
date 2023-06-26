#ifndef PROCESS_H
#define PROCESS_H

class Process{
  private:
  float a= random(100)/100.0f;
  float k= random(100)/20.0f;
  float x=0.0f;
  float y=0.0f;
  public: 
  void update(float x){
    this->x = x;
    this->y = this->k*this->a*this->x+(1-this->a)*this->y;
  }
  float read(){
    return this->y;
  }

};

#endif