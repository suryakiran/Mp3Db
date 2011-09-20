#ifndef XQueryResultExtractor_hxx_INCLUDED
#define XQueryResultExtractor_hxx_INCLUDED

#include <string>

class XQueryResultExtractor
{
  public:
    XQueryResultExtractor ()
    {
    }

    XQueryResultExtractor (const std::string& p_input)
      : m_input(p_input)
    {
    }

    void extractResult(const std::string& p_input = std::string("")) {
      if (!p_input.empty()) {
        m_input = p_input;
      }
      extractResultImp() ;
    }

    virtual bool extractResultFromXml() {
      return false;
    }

  protected:
    virtual void extractResultImp() = 0;

  protected:
    std::string m_input;
};

#endif
