#ifdef Interface_hpp_INCLUDED
#define Interface_hpp_INCLUDED

template <class InterfaceType>
class Interface : public boost::property_tree::ptree
{
};

struct ProgramOptions
{
  ProgramOptions : m_label ("ProgramOptions") { }

  const std::string& operator() const
  {
    return m_label;
  }
};

class ProgramOptionsTree : public Interface<ProgramOptions>
{
};

#endif
