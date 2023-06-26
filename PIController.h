#ifndef PICONTROLLER_H
#define PICONTROLLER_H

class PIController{
  private:
    float Kp = 0.0f;
    float Ki = 0.0f;
    float dt = 0;
    float err = 0.0f;
    float u = 0.0f;
    float u_max = 0.0f;
  public:
  PIController(float Kp, float Ki, int dt, float u_max){
    this->Kp = Kp;
    this->Ki = Ki;
    this->dt = float(dt);
    this->u = 0.0f;
    this->err = 0.0f;
    this->u_max = u_max;
  }

  float update(float setpoint, float y){
    float e = setpoint - y;
    this->u += this->Kp * (e - this->err) + this->Ki*this->err*this->dt/1000.0f;
    this->err = e;
    if (this->u>this->u_max){
      this->u=this->u_max;
    }
    else if (this->u<0.0f){
      this->u=0.0f;
    }
    return this->u;
  }

  void setKp(float Kp){
    this->Kp = Kp;
  }
  
  void setKi(float Ki){
    this->Ki = Ki;
  }
};

#endif