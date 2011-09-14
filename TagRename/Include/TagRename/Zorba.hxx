#ifndef Zorba_hxx_INCLUDED
#define Zorba_hxx_INCLUDED

namespace zorba
{
  class Zorba;
}

namespace xml
{
  class Zorba
  {
    public:
      static void init();
      static void terminate();
      static zorba::Zorba* instance();

    private:
      static void* m_store;
      static zorba::Zorba* m_zorba;
  };
}

#endif
