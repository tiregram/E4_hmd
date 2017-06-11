#pragma once

class Side
{
public:
  //! Default constructor
  Side();

  //! Copy constructor
  Side(const Side &other);

  //! Move constructor
  Side(Side &&other) noexcept;

  //! Destructor
  virtual ~Side() noexcept;

  //! Copy assignment operator
  Side& operator=(const Side &other);

  //! Move assignment operator
  Side& operator=(Side &&other) noexcept;

protected:
private:
};

