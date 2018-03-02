
#ifndef JSPY_TYPES__H_
#define JSPY_TYPES__H_

#include "./cparse/shunting-yard.h"
#include "./global-setup.h"

/**
 * Each tag has a given name that is associated with
 * a static tag_id.
 *
 * Declaring 2 tags with a same name imply they also have
 * the same tag_id.
 */
class HashTag : public TokenBase {
  typedef std::map<uint64_t, std::string> tagMap_t;
  typedef std::map<std::string, uint64_t> idMap_t;

  static tagMap_t& tags() {
    static tagMap_t tags;
    return tags;
  }

  static idMap_t& ids() {
    static idMap_t ids;
    return ids;
  }

  static uint64_t newTagId() {
    static uint64_t curr_id = 0;
    return ++curr_id;
  }

  static uint64_t getTagId(const std::string& name) {
    auto it = ids().find(name);
    if (it != ids().end()) {
      return it->second;
    } else {
      uint64_t id = newTagId();
      // Register the new tag name and id:
      tags()[id] = name;
      ids()[name] = id;
      return id;
    }
  }

  static std::string getTagName(uint64_t id) {
    return tags().at(id);
  }

 public:
  uint64_t tag_id;
  packToken data;

  const std::string name() const { return getTagName(tag_id); }

 public:
  HashTag() : TokenBase(TAG) {}

  HashTag(std::string tag)
    : TokenBase(TAG), tag_id(getTagId(tag)) {}

 public:
  TokenBase* clone() const {
    return new HashTag(*this);
  }
};

// Declare specialization prototype:
template<> HashTag& packToken::as<HashTag>() const;

#endif
