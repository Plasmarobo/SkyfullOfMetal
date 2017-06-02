//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//   Copyright(c) 2016 SCVNGR, Inc. d/b/a LevelUp. All rights reserved.
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include <utilities/log.h>
#include <utilities/macros.h>

#include <cstdio>
#include <cstdarg>
#include <iostream>

#include <unistd.h>
#include <sys/sysinfo.h>
#include <stdarg.h>


FILE *Log::_log_file;
std::string Log::_log_file_path;

void Log::Debug(const std::string &format, ...) {
#ifdef DEBUG
    va_list argptr;

    va_start(argptr, format);
    printf("DEBUG: ");
    vprintf(format.c_str(), argptr);
    va_end(argptr);
#else
    UNUSED(format);
#endif
}

void Log::Print(const std::string &format, ...) {
  va_list argptr;
  int r;
  struct sysinfo sys_info;
  static unsigned long long log_counter = 0;

  if (_log_file == NULL) {
    va_start(argptr, format);
    printf("Log: ");
    vprintf(format.c_str(), argptr);
    va_end(argptr);

    return;
  }

  log_counter++;
  sysinfo(&sys_info);

  fprintf(_log_file,
          "%llu\t"
          "%ld\t"
          "",
          log_counter,
          sys_info.uptime);
  va_start(argptr, format);
  r = vfprintf(_log_file,
               format.c_str(),
               argptr);
  va_end(argptr);

  if (r < 0) {
    /* There was a problem outputting the data, the file is probably
     * too big for the filesystem */
    fclose(_log_file);
    remove(_log_file_path.c_str());
    _log_file = fopen(_log_file_path.c_str(), "wt");
  }

  fflush(_log_file);
  sync();
}

bool Log::Initialize(const std::string &path) {
  _log_file_path = path;
  _log_file     = fopen(_log_file_path.c_str(), "a");

  return true;
}
