


class ObjectConnection
{
public:
  //! Default constructor
  ObjectConnection();

  //! Copy constructor
  ObjectConnection(const ObjectConnection &other);

  //! Move constructor
  ObjectConnection(ObjectConnection &&other) noexcept;

  //! Destructor
  virtual ~ObjectConnection() noexcept;

  //! Copy assignment operator
  ObjectConnection& operator=(const ObjectConnection &other);

  //! Move assignment operator
  ObjectConnection& operator=(ObjectConnection &&other) noexcept;



protected:
private:
};
