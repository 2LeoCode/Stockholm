#pragma once

#include <unordered_map>
#include <string_view>
#include <string>
#include <optional>
#include <vector>

namespace cli {
  class ArgumentParser {
    using Self = ArgumentParser;

    template <class KeyType, class ValueType>
    using dict = std::unordered_map<KeyType, ValueType>;
    template <class T>
    using view = std::basic_string_view<T>;
    template <class T>
    using optional = std::optional<T>;
    template <class T>
    using vec = std::vector<T>;
    template <class ...T>
    using tuple = std::tuple<T...>;
    using str = std::string;
    using str_view = std::string_view;

    public:
      struct Option {
        const char letter;
        const str_view & description;

        constexpr bool operator==(const Option & other) const {
          return letter == other.letter;
        }
      };

      constexpr ArgumentParser(
        int argc,
        const char * const * argv,
        bool help = true,
        const str_view & description = ""
      ) : m_argc(argc), m_argv(argv), m_help(help) {};

      Self & addOption(
        const str_view & name,
        char letter,
        const str_view & description
      );
      Self & addOptions(const view<Option> & options);
      dict<str, optional<str>> parse(void);

    private:
      const int m_argc;
      const char * const * const m_argv;
      const bool m_help;
      vec<Option> m_options;
  };
}
