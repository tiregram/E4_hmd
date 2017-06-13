#pragma once


class OpenHmdWrapper
{
public:
  //! Default constructor
  OpenHmdWrapper();

  //! Copy constructor
  OpenHmdWrapper(const OpenHmdWrapper &other);

  //! Move constructor
  OpenHmdWrapper(OpenHmdWrapper &&other) noexcept;

  //! Destructor
  virtual ~OpenHmdWrapper() noexcept;

  //! Copy assignment operator
  OpenHmdWrapper& operator=(const OpenHmdWrapper &other);

  //! Move assignment operator
  OpenHmdWrapper& operator=(OpenHmdWrapper &&other) noexcept;

  


protected:
private:
  ohmd_context* ctx;
  ohmd_device_settings* settings;
  ohmd_device* hmd;
};
