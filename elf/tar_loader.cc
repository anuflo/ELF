#include "tar_loader.h"
#include <memory.h>

bool file_is_tar(const std::string& filename) {
  return filename.substr(filename.find_last_of(".") + 1) == "tar";
}

TarLoader::TarLoader(const std::string &tar_filename) {
  mtar_open(&tar, tar_filename.c_str(), "r");
}

std::vector<std::string> TarLoader::List() {
  std::vector<std::string> v;
  mtar_header_t h;
  while ((mtar_read_header(&tar, &h)) != MTAR_ENULLRECORD) {
    v.push_back(std::string(h.name));
    mtar_next(&tar);
  }
  return v;
}

std::string TarLoader::Load(const std::string &filename) {
  mtar_header_t h;
  mtar_find(&tar, filename.c_str(), &h);
  char *s = new char[h.size + 1];
  ::memset(s, 0, h.size + 1);
  mtar_read_data(&tar, s, h.size);
  std::string res(s);
  delete [] s;
  return res;
}

TarLoader::~TarLoader() {
    mtar_close(&tar);
}
