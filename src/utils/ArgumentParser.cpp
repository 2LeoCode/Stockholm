#include <algorithm>
#include <stdexcept>
#include <ArgumentParser.hpp>

#define Self ArgumentParser

namespace cli {
  namespace {
    using std::find;
  };

  Self & Self::addOption(
    const str & name,
    char letter,
    const str_view & description
  ) {
    return *this;
  }

  Self & Self::addOptions(const view<Option> & options) {
    for (const auto & option : options) {
      if (find(m_options.begin(), m_options.end(), option) != m_options.end())
        throw std::invalid_argument("Duplicate option");  
      m_options.push_back(option);
    }
    return *this;
  } 
}
