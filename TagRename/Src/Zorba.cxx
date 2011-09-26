#include <TagRename/Zorba.hxx>
#include <zorba/zorba.h>
#include <zorba/store_manager.h>

zorba::Zorba* xml::Zorba::m_zorba = (zorba::Zorba*) 0;
void* xml::Zorba::m_store = (void*) 0;

void
xml::Zorba::init()
{
  m_store = zorba::StoreManager::getStore();
  m_zorba = zorba::Zorba::getInstance(m_store);
}

void
xml::Zorba::terminate()
{
  m_zorba->shutdown();
  zorba::StoreManager::shutdownStore(m_store);
}

zorba::Zorba* 
xml::Zorba::instance()
{
  if (!m_zorba)
  {
    init();
  }
  return m_zorba;
}
