#ifndef LOG_H
#define LOG_H

#include <string>

class Log {
 public:
  static bool Initialize(const std::string &path);
  static void Debug(const std::string &format, ...);
  static void Print(const std::string &format, ...);

 private:
  static std::string _log_file_path;
  static FILE *_log_file;
};

#endif /* LOG_H */
