#include "ArduinoSQLite.hpp"
#include "ArduinoSQLiteEXTMEM.hpp"

int T41SQLite::begin(FS* io_filesystem, bool in_useEXTMEM)
{
  if (in_useEXTMEM)
  {
    if (int result = sqlite3_use_extmem(); result != SQLITE_OK)
    {
      return result;
    }
  }

  m_filesystem = io_filesystem;
  return sqlite3_initialize();
}

int T41SQLite::end()
{
  int result = sqlite3_shutdown();
  m_filesystem = nullptr;
  return result;
}

FS* T41SQLite::getFilesystem()
{
  return m_filesystem;
}

void T41SQLite::setDBDirFullPath(const String& in_dbDirFullpath)
{
  m_dbDirFullpath = in_dbDirFullpath;
}

const String& T41SQLite::getDBDirFullPath() const
{
  return m_dbDirFullpath;
}

int T41SQLite::setLogCallback(LogCallback in_callback, void* in_forUseInCallback)
{
  return sqlite3_config(SQLITE_CONFIG_LOG, in_callback, in_forUseInCallback);
}
