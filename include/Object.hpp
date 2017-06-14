#pragma once


class Object
{
public:
  virtual void draw()= 0;
  virtual void update(double t)=0;
};

