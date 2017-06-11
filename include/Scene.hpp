#pragma once

class Scene
{
public:
  //! Default constructor
  Scene();

  //! Copy constructor
  Scene(const Scene &other);

  //! Move constructor
  Scene(Scene &&other) noexcept;

  //! Destructor
  virtual ~Scene() noexcept;

  //! Copy assignment operator
  Scene& operator=(const Scene &other);

  //! Move assignment operator
  Scene& operator=(Scene &&other) noexcept;

protected:
private:
};
