#pragma once

#include <limits>
#include <memory>
#include <string_view>
#include <stdexcept>
#include <vector>
#include <cstring>

namespace data {
  template <class T>
  class Trie {
    using Self = Trie;

    template <class T>
    using limits = std::numeric_limits<T>;
    template <class T>
    using un_ptr = std::unique_ptr<T>;
    using str_view = std::string_view;

    constexpr static uint8_t CHILD_COUNT = limits<uint8_t>::max();

    class Node {
      using Self = Node;

      public:
        un_ptr<T> data;

        ~Node(void) {
          for (uint8_t i = 0; i < CHILD_COUNT; ++i)
            delete _children[i];
        }

        Self & addChild(uint8_t i) {
          using std::invalid_argument;

          if (_children[i])
            throw invalid_argument("DuplicateKey");
          _children[i] = new Node;
          _children[i]->_parent = this;
          return *this;
        }

        Self & removeChild(uint8_t i) {
          using std::invalid_argument;

          if (!_children[i])
            throw invalid_argument("KeyNotFound");
          delete _children[i];
          _children[i] = nullptr;
        }

        inline bool hasChildren(void) const {
          return _childCount;
        }

        inline Self * parent(void) const {
          return _parent;
        }

        inline size_t childCount(void) const {
          return _childCount;
        }

        inline Self * getChild(uint8_t idx) const {
          return _children[idx];
        }

        constexpr Self ** children(void) const {
          return _children;
        }

      private:

        Node * _parent;
        Node * _children[CHILD_COUNT];
        size_t _childCount;
    };

    public:
      inline T & get(const str_view & key, const T & def = T()) {
        using std::exception;
        
        T * value;

        try {
          value = getNode()->data;
        } catch (const exception & e) {
          return def;
        }
        return *value;
      }

      Self & set(const str_view & key, const T & value) {
        using std::make_unique, std::invalid_argument;

        if (!root)
          root = make_unique<Node>();
        Node * node = root;
        for (const auto & c : key) {
          if (!node->getChild(c))
            node->addChild(c);
          node = node->getChild(c);
        }
        if (node->data)
          throw invalid_argument("DuplicateKey");
        node->data = make_unique<T>(value);
        return *this;
      }

      Self & unset(const str_view & key) {
        auto node = getNode(key);
        node->data.reset();
        auto it = key.rbegin();
        do {
          node = node->parent();
          ++it;
        } while (node->parent() && node->parent()->childCount() == 1);
        if (!node->parent())
          return clear();
        node->parent()->removeChild(*it);
        return *this;
      }

      constexpr Self & clear(void) {
        root.reset();
        return *this;
      }

    private:
      Node & getNode(const str_view & key) const {
        using std::invalid_argument;
        
        if (!root)
          throw invalid_argument("KeyNotFound");
        const Node * node = root;
        for (const auto & c : key) {
          if (!node->getChild(c))
            throw invalid_argument("KeyNotFound");
          node = node->getChild(c);
        }
        if (!node->data)
          throw invalid_argument("KeyNotFound");
        return node;
      }

      un_ptr<Node> root;
  };
}
